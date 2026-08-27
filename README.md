# bazel_template

A C++20 project template built with **Bazel 9 + Bzlmod**. It wires up a large set
of third-party C/C++ libraries and ships working gRPC, HTTP, WebSocket, protobuf
plugin, and SWIG examples, so a new service can start from a tree that already
builds and tests on Linux, macOS, and Windows.

## Features

* Most popular C/C++ libraries pre-integrated: grpc, protobuf, boost, abseil,
  folly, proxygen, wangle, fizz, mvfst, fbthrift, openssl, curl, zstd, lz4,
  libevent, libuv, sqlite, spdlog, simdjson, and more
* gRPC server and client example
* HTTP server and WebSocket example
* protobuf code-generator plugin example
* SWIG bindings for Java and Python
* Multiple platforms: linux, macos, windows
* Multiple CPUs: x86_64, aarch64
* Cross compilation against glibc and musl toolchains
* Builds with gcc, clang, and msvc-cl
* `compile_commands.json` generation
* Sanitizers (asan/msan/tsan/ubsan) and coverage
* mimalloc linked into tests by default

## Requirements

* **Bazel 9.2.0** — pinned in `.bazelversion`
* A C++20 compiler: MSVC 2022 (Windows), GCC 16 (Linux, see `.bazelrc`), or Clang
* Python 3 (used by codegen helpers)
* JDK for the SWIG/Java target (Bazel provisions `remotejdk_21`)

External dependencies are managed entirely by `MODULE.bazel` (Bzlmod). The
`WORKSPACE` file is an empty marker kept only for tools that still look for one.

## Quick start

```bash
bazel build //src/...
bazel test --config=unit_test //src/...
```

Run the server and client from the repository root — both read their config
from a path relative to the working directory:

```bash
bazel run //src/server:server     # reads ./conf/server_config.json
bazel run //src/client:client     # reads ./conf/client_config.json
```

`conf/server_config.json` defaults to gRPC on port 10001 and HTTP/WebSocket on
port 10003.

## Build configurations

Platform configs are selected automatically via
`--enable_platform_specific_config`, so `build:linux`, `build:macos`, and
`build:windows` apply on their own. The rest are opt-in:

| Config | Purpose |
| --- | --- |
| `--config=opt` / `--config=dbg` | Optimized / debug compilation mode |
| `--config=asan` | AddressSanitizer |
| `--config=msan` | MemorySanitizer |
| `--config=tsan` / `--config=thready_tsan` | ThreadSanitizer |
| `--config=ubsan` | UndefinedBehaviorSanitizer |
| `--config=clang-tidy` | Run clang-tidy as an aspect |
| `--config=unit_test` | Tests only, excludes cpplint targets |

### Cross compilation

| Config | Target |
| --- | --- |
| `--config=clang_gnu` / `--config=clang_musl` | x86_64 linux, glibc / musl |
| `--config=clang_x86_64_linux_gnu` | x86_64 linux, glibc |
| `--config=clang_aarch64_linux_gnu` / `--config=clang_aarch64_linux_musl` | aarch64 linux |
| `--config=gcc_aarch64_linux_gnu` / `--config=gcc_aarch64_linux_musl` | aarch64 linux |

These expect the toolchains from
[xiedeacc/cc_toolchains](https://github.com/xiedeacc/cc_toolchains) and a
sysroot laid out under `/root/src/software/toolchains`; adjust the
`--action_env` paths in `.bazelrc` to match your machine.

## Tooling

### compile_commands.json

```bash
PYTHONUTF8=1 bazel run //:refresh_compile_commands
```

`PYTHONUTF8=1` is needed on a non-UTF-8 Windows locale: the refresh script
decodes subprocess output with the system codepage and dies on MSVC output that
is not valid GBK. See *Known issues* — this does not run to completion on
Windows yet.

### clang-tidy

```bash
bazel build --config=clang-tidy //src/...
```

### Coverage

```bash
bazel coverage //src/...
genhtml --ignore-errors source bazel-out/_coverage/_coverage_report.dat -o coverage_html
```

### Sanitizers

```bash
bazel test --config=asan  //src/...
bazel test --config=tsan  //src/...
bazel test --config=ubsan //src/...
```

Sanitizers require gcc or clang; they are not wired up for MSVC.

### Useful queries

```bash
bazel query "rdeps(//src/..., //src/util:util)"
bazel query --notool_deps --noimplicit_deps "deps(//src/server:server)" --output graph > graph.in
dot -Tpng < graph.in > graph.png
```

## Generating a new repository from this template

```bash
./generate.sh repo_name
```

This rewrites the module name and the `@bazel_template//` labels, so remember
that `bazel/*.BUILD` files reference the module by name.

## Windows notes

Windows is a supported first-class target, but a few things are specific to it:

* **Short output paths.** `.bazelrc` sets `startup:windows
  --output_user_root=D:/bzl` to stay under the Win32 `MAX_PATH` limit. Change it
  if you do not have a `D:` drive.
* **gperf** is required by proxygen's header codegen. `bazel/proxygen.BUILD`
  points at `D:/software/gperf/bin/gperf.exe`. The path is absolute on purpose:
  the action runs under Git bash, which cannot split the semicolon-separated
  Windows `PATH` coming from `--action_env`, so a bare `gperf` is not found.
* **Winsock initialization.** `src/common/winsock_init.cc` registers
  `WSAStartup()` in the `.CRT$XIU` section so it runs before any C++ static
  initializer. proxygen constructs a namespace-scope `folly::SocketAddress`
  during static init, which fails with `WSANOTINITIALISED` and aborts the
  process before `main()` if Winsock is not ready. Any binary that links
  proxygen must depend on `//src/common:winsock_init`.
* **C/C++ standard.** `.bazelrc` disables the `default_cpp_std` and
  `default_c_std` toolchain features so `/std:c++20` and `/std:c17` are the only
  standard flags on the command line. (C has no C20; C17 is the current one.)

## Notes

* Third-party fetches go through `.bazel_downloader.cfg`
  (`common --downloader_config=.bazel_downloader.cfg`). Comment that line out if
  you do not want it.
* Maven artifacts resolve through the Aliyun mirror first, with
  `repo1.maven.org` as a fallback — see the `maven.install` block in
  `MODULE.bazel`.
* Cross compiling requires you to supply your own toolchains and sysroots.

## Known issues

* **cpplint discovers no targets.** `bazel test --config=cpplint //...` reports
  `Found 0 test targets`. `_extract_labels` in `bazel/cpplint.bzl` only accepts
  a tuple, but `native.existing_rules()` no longer returns `srcs`/`hdrs` as
  tuples, so the macro silently generates nothing. The lint gate is currently a
  no-op.
* **`refresh_compile_commands` does not finish on Windows.** The Bazel 9 load
  errors are patched (`bazel/hedron-compile-commands-py-binary.patch`), so the
  target builds and runs, but the refresh itself still fails: it needs
  `PYTHONUTF8=1` on a GBK locale, and then re-runs the build with its own flags,
  where boost's MASM assembly step fails to write its object file. Untested on
  Linux and macOS, which likely avoid both problems.

## Future work

1. Take the compiler from `@bazel_tools//tools/cpp:compiler`
2. Use an aspect to find the most likely relative search path
3. Transitive usage
4. `renovate.json`
5. Integrate IWYU — https://github.com/storypku/bazel_iwyu
