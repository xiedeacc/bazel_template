# C++20 模块：约定与各编译器的坑

bazel_template 的模块化整体从 tbox 移植而来：构建基础设施（`bazel/`、`.bazelrc`、
`MODULE.bazel`、lint 门禁 `tools/`）和 `src/async_grpc` 原样复制，其余库按 tbox 的
规则转换成命名模块。tbox 在四条工具链（Windows MSVC 14.52、macmini Apple clang 21、
dev 的 gcc 16、dev 上的 aarch64 musl gcc 交叉）上验证过这些约定；bazel_template 目前
只在 Windows 上跑过构建、lint 和测试。本文记录约定以及每条约定背后的编译器问题。
改模块代码之前先读一遍。

## 约定

1. **头文件里不写 `import`。** 需要模块里名字的内联代码搬到 .cc 里，头文件只
   保留声明。测试等普通翻译单元自己
   `import` 它用到的模块，不能指望头文件带进来。
2. **翻译单元里所有 `#include` 在前，所有 `import` 在后。** 模块实现单元同理：
   全局模块片段（`module;` 之后）放全部 include，`module bazel_template.x;` 之后只放
   `import`。`import` 不能出现在 `#if` 块里。
3. **API 是宏的库保持为头文件。** 宏无法从模块导出，而 `LOG()`/`CHECK()` 又
   要在头文件的内联代码里用，所以 `src/common/logging.h` 是普通头文件，模块单
   元把它放进自己的全局模块片段。它把 glog 的宏换成 spdlog 后端，所以代码里不
   直接包含 `glog/logging.h`。`socket_compat.h` 同理。
4. **第三方重头文件不进模块接口。** 模块接口只前置声明
   `google::protobuf::Message`（`src/util/util_fwd.h`）和生成的 proto 消息
   （`src/proto/config_fwd.h`、`service_fwd.h`），完整类型只出现在实现单元的
   全局模块片段里。
5. **有实现单元的目标关掉 clang 的 layering_check。** 见
   `src/util/BUILD` 里 `features = ["-layering_check"]` 的目标。
6. **每个 `src/*` 包都开 `cpp_modules` feature，`.bazelrc` 里全平台打开
   `--experimental_cpp_modules`。**
7. 改动模块代码后跑 `bazel test -- //src/... -//src/swig/...`，Windows 上还要过
   `bazel build --config=lint -- //src/... -//src/swig/...`。

## 背后的编译器问题

### gcc 16

- `import` 经由 `#include` 进入模块单元是硬错误：
  `post-module-declaration imports must not be from header inclusion`。
- 只要某个标准库头文件第一次出现在 `import` 之后，就报
  `conflicting declaration`（`std::filesystem::path`、`std::function`
  等）；先 include 再 import 就正常。dev 上的对照实验（同一编译命令，只调
  换 `<filesystem>` 和 `import bazel_template.util;` 的顺序）确认了这一点。这是约定 1
  和 2 的直接原因：头文件里的 `import` 会让后面被包含的头文件落到 import
  之后。
- 模块全局模块片段里只前置声明的类（`class Message;`），在导入方*随后*才见到
  真实声明时报 `conflicting declaration 'struct google::protobuf::Message'`；
  先见到真实声明再 import 则没问题（约定 2 保证了这一点）。反过来把真实的
  folly/protobuf 头文件放进全局模块片段更糟：生成的 BMI 和导入方自己包含的
  folly 合并失败（`failed to read compiled module cluster ...: Bad file
  data`、`failed to load pendings for 'folly::IPAddressV4'`）。所以三个编译
  器都只用 `util_fwd.h` 的前置声明。

### MSVC 14.44 / 14.52

- `folly/IPAddress.h` 或 `google/protobuf/message.h` 出现在全局模块片段里，
  写 BMI 时 C1001 内部错误。所以只前置声明。
- `<span>`（`std::dynamic_extent`，C2374）和 `<source_location>`（C2011）必须
  在 import 之前被文本包含；`logging.h` 因此预先包含这一组标准库头文件。
- 模块依赖扫描直接调用 cl.exe，`-isystem` 不认，Windows 上改用 `/I`。
- 只有开了 `parse_showincludes` Bazel 才知道模块编译动作的输入，否则崩溃
  “inputs still not known”。

### Apple clang 21

- 没有 `-fcxx-modules` 就不认识 `module;`；但 `-fcxx-modules` 同时打开 Clang
  自己的隐式头文件模块，补丁里配上 `-fno-implicit-module-maps` 和
  `-fno-implicit-modules`。
- `layering_check` 会传 `-fmodule-name=<label>`，clang 拿它和实现单元的模块名
  比较，不一致就报错，没有办法二选一，只能对有实现单元的目标关掉这个检查。
- clang 21（Apple 和上游都一样）会把 BMI 全局模块片段里 libc++ 的隐藏内联函数
  （`_LIBCPP_HIDE_FROM_ABI`，例如 `condition_variable_any::wait(stop_token)` 背后的
  `__atomic_unique_lock::__set_locked_bit`）当成“模块会提供定义”，导入方和实现单元
  都不再生成它，链接时未定义。apple_support 补丁给模块相关的编译都加了
  `-Xclang -fmodules-codegen`，让接口单元的目标文件把这些定义发出来；最小复现见
  提交说明。
- protobuf 的 `parse_context.h` 在头文件里声明了几个 `static` 函数，模块单元
  结束时实例化模板去找它们，找不到（`no matching function for call to
  'VarintParseSlowArm'`，且没有任何候选）。`bazel/protobuf-cxx-modules-arm-varint.patch`
  把这几个函数改成 `inline`。

## protobuf 能不能进模块接口（async_grpc 的前提）

tbox 的探针 `//src/test/modules_probe:grpc_probe`（本仓库没有复制）接口同时使用
`grpc::ServerContext` 和 `google::protobuf::Message`：

- gcc 16（主机和 aarch64 交叉）：通过。
- Apple clang 21：打上 `bazel/protobuf-cxx-modules-arm-varint.patch`（把
  `parse_context.h` 里两个 `static` 的 `VarintParseSlowArm` 改成 `inline`）后通过。
- MSVC 14.44 和 14.52：只要导出声明可达 `google::protobuf::Message`（哪怕只是一个
  `const Message&` 参数或 `Message*` 成员），cl.exe 写 BMI 时 C1001。二分结论：
  grpc++ 单独、`MessageLite`、`Descriptor`、`Arena` 以及 message.h 包含的每个头
  文件单独都正常；崩溃来自序列化 `Reflection`（`Message::GetReflection()` 让它
  可达，`MessageFactory` 同理），去掉 `Message` 的友元、`constexpr` 构造、继承
  构造和类外显式特化都无济于事。只用前置声明（`util_fwd.h` 的做法）不触发。

VS 2026（18.9，MSVC 14.52）也一样崩，所以这不是老工具集的问题。`.bazelrc` 里用
`--repo_env=BAZEL_VC` 把工具集钉在 VS 2026 的 14.52 上（VS 2022 仍然装着，Bazel 的
vswhere 自动探测会在两者之间悄悄切换并触发全量重编）；要试别的版本时在命令行覆盖
这个变量，在 tbox 里跑 `bazel test //src/test/modules_probe:grpc_probe_test` 即可，
一分钟出结论。给微软报 bug 的最小复现就是探针本身：全局模块片段里
`#include "google/protobuf/message.h"`，再导出一个 `void f(const
google::protobuf::Message&);` 声明。

绕过办法是**接口只前置声明，完整类型只进实现单元**。实现单元不产生 BMI，所以
它的全局模块片段里可以放 `message.h`、`proto_utils.h`、生成的 `*.pb.h`、AWS SDK、
proxygen/folly，四条工具链都验证过。按这个思路现在的模块是：

| 模块 | 接口里有什么 | 实现单元 |
| --- | --- | --- |
| `bazel_template.async_grpc` | 服务端全部类（`Rpc`、`RpcHandler<>`、`Server`、`Service`……）和异步客户端；`Message`/`Descriptor`/`DescriptorPool` 只用 `util_fwd.h` 的前置声明，`Rpc` 的析构搬到 rpc.cc | rpc.cc、server.cc、service.cc 等（与 tbox 相同） |
| `bazel_template.async_grpc.client` | 阻塞客户端模板 | 无 |
| `bazel_template.common.defs` | 常量 | 无 |
| `bazel_template.util` | `Util` 的哈希/SHA256/JSON/文件辅助声明；openssl 在全局模块片段，protobuf 只前置声明 | util.cc |
| `bazel_template.util.config_manager` | `ConfigManager` 的访问器声明；`proto::BaseConfig` 只前置声明（`src/proto/config_fwd.h`），成员是 `unique_ptr` | config_manager.cc 定义全部访问器和构造/析构 |
| `bazel_template.server.grpc_handler` | `RegisterHandlers(Server::Builder&)` | registry.cc 调用模块内部链接的 `RegisterEC2Handler()`/`RegisterRoute53Handler()`；handler 类各在 ec2_handler.cc、route53_handler.cc 里（proto、AWS SDK 都在那里） |
| `bazel_template.server.http_handler` | `CreateHandlerFactory()`；proxygen 类只前置声明 | handler_factory.cc（把 `/api/v1/folder/load` 路由到头文件里的 `WebSocketUpgradeHandler`） |
| `bazel_template.server.context` / `grpc_server` / `http_server` | `ServerContext`、`GrpcServer`、`HttpServer` 的声明 | 用到 `ConfigManager`、proxygen、handler 模块的部分 |
| `bazel_template.client.websocket_client` | `WebSocketClient` 声明；boost/folly 成员进 pimpl `Impl` | websocket_client.cc |

模块接口里每一个内联函数体都搬到了实现单元（包括一行的访问器），这是脚本化迁移
的统一规则：只有 `constexpr`/`consteval` 函数和 `= default`/`= delete` 留在接口里。

仍然是头文件的只剩这些，各有理由：`logging.h`、`socket_compat.h`（宏 API）、
`util_fwd.h`、`config_fwd.h`、`service_fwd.h`（前置声明）、`grpc_handler/meta.h`
（包含 `service.pb.h` 的 trait，只进实现单元的全局模块片段）、
`common/blocking_queue.h`（模板，只在 websocket 实现单元里用）、
`http_handler/websocket_handler.h`/`websocket_upgrade_handler.h`（proxygen/folly 内联类，只进
factory 实现单元的全局模块片段）、
`async_grpc/common/*.h` 和 `type_traits.h`/`rpc_service_method_traits.h`/`retry.h`
（多个模块的全局模块片段共享）、生成的 `error.h`/`version_info.h`。protoc 插件
`pb_code_generator` 只是一个翻译单元，头文件并入了 .cc。

模板仍然在导入方实例化：`RpcHandler<>`、`Server::Builder::RegisterHandler<>()`
在 handler 的实现单元里实例化，那里的全局模块片段有完整的 `Message` 和
`DescriptorPool`，前置声明和真实声明按约定 2 的顺序合并。

生成的 `config.pb.h` 不能出现在任何接口单元的全局模块片段：tbox 里 `ServerContext`
只要内联地调用包含它的旧 `config_manager.h`，写 BMI 就 C1001。所以
`bazel_template.util.config_manager` 的接口只前置声明 `BaseConfig`，
`bazel_template.server.context` 把用到配置的方法放进 context.cc。

接口 purview 里不能留旧头文件的前置声明（tbox 里的 `class AuthenticationManager;`）：
它会声明一个**附着在本模块**的新类，实现单元里对它的引用就解析到这个新类，链接时
报带 `[模块名]` 的符号未定义。前置声明只放全局模块片段，或者直接 import 真正的模块。

Bazel 目前在 MSVC 上不会因为接口单元变了就重编同一模块的实现单元（依赖扫描没有
记录 `module X;` 隐含的自导入），改了 .cppm 之后实现单元可能拿着旧 BMI 的 .obj 去
链接。遇到莫名其妙的链接错误先 `touch` 实现单元或 `bazel clean`。

Windows 的 `windows.h` 把 `SendMessage` 定义成宏：实现单元的全局模块片段包含了
它，就要在 `module X;` 前 `#undef SendMessage`，否则成员名和接口对不上
（`websocket_client.cc` 就是这样）。

MSVC 还有一个怪癖：BMI 里 grpc 的 `static` 内联函数（`atm_windows.h` 的
`gpr_atm_no_barrier_fetch_add`）在导入方只剩声明，报 C2129 “declared but not
defined”。凡是导入了可达 grpc 类型的模块的翻译单元（接口单元、实现单元、普通
.cc 都一样），自己先 `#include "grpc++/grpc++.h"`。同理，导入方需要 grpc 的
protobuf 序列化特征时要自己包含 `grpc++/impl/codegen/proto_utils.h`（或生成的
`*.grpc.pb.h`），模块不会替它带进来。

## lint 门禁

clang-tidy 没有 BMI（VS 只带 clang-tidy.exe，MSVC 的 BMI 是 .ifc）。
`tools/lint/cpp_modules.bzl` 包装 rules_lint 的 aspect：每个模块接口生成一个
stub 头文件；每个用到模块的被检查目标生成一棵“树”，里面是它的源文件、接口和
闭包内所有仓库头文件的改写副本（`import` 改成 include stub），clang-tidy 编译
副本时把树放在 quote include 搜索路径最前面（写在 `--config` 文件第一项，
`--extra-arg-before` 会排在工具链自己的 `-iquote` 之后）。
`clang_tidy_driver.py` 把报告里的路径映射回原文件。aspect 会顺着 deps 传播到
外部仓库，但对外部目标不生成任何动作，否则会有输出冲突。

门禁用的 clang-tidy 来自 VS 2026 的 LLVM 22（`bazel/llvm_tools.bzl`，VS 2026 的 STL
要求 clang ≥ 22）。升级到 22 带来的变化：`--config` 参数文件里 `-iquote`/`-I`/
`-isystem` 必须写成连体形式（`-iquote<dir>`），分行写会把下一行当成输入文件；新检查
`cppcoreguidelines-pro-bounds-avoid-unchecked-container-access` 和
`portability-template-virtual-member-function` 在 `.clang-tidy` 里关掉，理由写在那里；
其余新发现（`#if defined(X)` → `#ifdef X`、`std::lock_guard` → `std::scoped_lock`、
多余的 `typename`、noexcept 函数里可能抛出的调用）已按建议修掉。
