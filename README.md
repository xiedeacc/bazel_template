# features

# todo
1. fix bazel test --config=sanitize //... core
0. fix //src/swig:config_manager_test core
6. toolchains_openwrt need download from remote, support gcc, clang, openwrt, windows, 多版本,全部注册

# future todo:
1. import virtual include like boost
2. use baze aspect find the most possible relative search path
3. 集成clang-tidy, clang-check, clang static analyzer
4. module map, -compiler_param_file, -layering_check
5. aspect用法
6. transitive用法
7. renovate.json
8. fizz lack of aegis, liboqs

# usage

## 生成compile_commands.json
```
clear && bazel run @hedron_compile_commands//:refresh_all
```

## 单测与代码风格
```
bazel test //... --test_tag_filters=cpplint    #只跑cpplint检查
bazel test //... --test_tag_filters=-cpplint   #不跑cpplint检查
bazel test //... --config=unit_test            #根据.bazelrc配置文件，跑单测和内存泄露检查，不跑cpplint检查
bazel test //... --config=cpplint              #只跑cpplint检查
```

## 内存leak检测
```
bazel test --config=unit_test //... #检测到内存泄露单测将失败，并查询详细日志即可
```

## 内存corruption
```
bazel test --config=sanitize //...
```

## cpu性能和内存分析
```
go install github.com/google/pprof@latest
bazel test --test_env="CPUPROFILE=prof.out" --test_env=HEAPCHECK=normal //src/common:host_spec_test #prof.out在bazel构建根目录下
CPUPROFILE=prof.out bazel-bin/src/common/host_spec_test
HEAPPROFILE=heap.out bazel-bin/src/common/host_spec_test

pprof --web bazel-bin/src/common/host_spec_test prof.out
pprof --text ./bazel-bin/src/common/host_spec_test prof.out
pprof --pdf ./bazel-bin/src/common/host_spec_test prof.out > profile.pdf
pprof ./bazel-bin/src/common/host_spec_test heap.prof --inuse_objects --lines --heapcheck --edgefraction=1e-10 --nodefraction=1e-10 --gv
pprof ./bazel-bin/src/common/host_spec_test heap.prof --inuse_objects --lines --heapcheck --edgefraction=1e-10 --nodefraction=1e-10 --pdf > profile.pdf

perf record -F 99 -g bazel-bin/src/demo 10000
perf script | /root/src/software/FlameGraph/stackcollapse-perf.pl | /root/src/software/FlameGraph/flamegraph.pl > flamegraph.svg

https://gperftools.github.io/gperftools/heapprofile.html
https://gperftools.github.io/gperftools/cpuprofile.html
https://gperftools.github.io/gperftools/heap_checker.html
https://gperftools.github.io/gperftools/tcmalloc.html
```

## 覆盖率分析
```
bazel coverage //... --test_tag_filters=-cpplint
genhtml bazel-out/k8-fastbuild/testlogs/src/common/host_spec_test/coverage.dat -o /zfs/www/coverage
genhtml --ignore-errors source bazel-out/k8-fastbuild/testlogs/src/common/host_spec_test/coverage.dat -o /zfs/www/test_coverage
https://coverage.xiamu.com
```

## 依赖图等查询
```
bazel query --notool_deps --noimplicit_deps "deps(//src/server:grpc_server)" --output graph
bazel query 'attr(visibility, "//visibility:public", //:*)'
bazel query "deps(//src/context:all)"
bazel query 'deps(//src/server:grpc_server)' --output graph > graph.in
bazel query --noimplicit_deps 'deps(//:main)' --output graph > simplified_graph.in
dot -Tpng < graph.in > graph.png
```

## 系统调用分析
```
cat /proc/self/stack
cat /proc/21880/stack
strace -Ff -tt -p 56509 2>&1 | tee strace.log
pstack 56509
```

## 指定toolchain
```
bazel build \
  --platforms=@toolchains_llvm//platforms:linux-x86_64 \
  --extra_execution_platforms=@toolchains_llvm//platforms:linux-x86_64 \
  --extra_toolchains=@llvm_toolchain_linux_exec//:cc-toolchain-x86_64-linux \
  //...
```

# 一些常见和编译相关命令

```
/usr/local/llvm-18/bin/clang++ -E -x c++ - -v < /dev/null
echo "" | gcc -v -E -

-v: verbose mode, including the compiler's version, the directories it searches for header files, and the commands it executes.
-H: makes the preprocessor print the name of each header file used
-E: tells GCC to stop after the preprocessing stage, the output is the preprocessed source code, which includes the expanded #include directives, macros, and other preprocessing directives.
-S: stops the compilation process after the assembly stage, producing an assembly language file
-P: inhibits the generation of #line directives in the preprocessed output
-C: instructs the preprocessor to retain comments in the output.
-dD: tells the preprocessor to output macro definitions in addition to the preprocessed output
-dM: tells the preprocessor to output only the macro definitions, without processing the rest of the input file.

gcc -E -P source_file.c -o preprocessed_output.c
gcc -M source_file.c -o dependencies.d
gcc -E -dD source_file.c -o macros_output.c

cd /root/src/library/fbthrift && /root/src/library/fbthrift/bazel-bin/thrift1 --gen mstch_cpp2:no_metadata,include_prefix=thrift/conformance/if \
-o thrift/conformance/if \
-I /root/src/library/fbthrift thrift/conformance/if/serialization.thrift

CC=/root/src/software/openwrt/toolchain/bin/aarch64-openwrt-linux-musl-gcc-12.3.0 \
CXX=/root/src/software/openwrt/toolchain/bin/aarch64-openwrt-linux-musl-g++.bin \
CCAS=/root/src/software/openwrt/toolchain/bin/aarch64-openwrt-linux-musl-as.bin \
CXXFLAGS="--sysroot=/root/src/software/openwrt/toolchain -isystem /root/src/software/openwrt/toolchain/aarch64-openwrt-linux-musl/include/c++/12.3.0" \
CFLAGS="--sysroot=/root/src/software/openwrt/toolchain -isystem /root/src/software/openwrt/toolchain/include" \
LDFLAGS="--sysroot=/root/src/software/openwrt/toolchain -L/root/src/software/openwrt/toolchain/lib" ./configure --host=aarch64-unknown-linux-gnu

cmake \
  -DCMAKE_INCLUDE_PATH=/usr/local/llvm/18/include/aarch64-unknown-linux-gnu/c++/v1:/usr/local/llvm/18/include/c++/v1 \
  -DCMAKE_LIBRARY_PATH=/usr/local/llvm18/lib ...

CMAKE_PREFIX_PATH=/usr/local:/usr/local/llvm/18 LD_LIBRARY_PATH=/usr/local/lib PATH=/usr/local/bin PKG_CONFIG_PATH=/usr/local/lib/pkgconfig \
  cmake --build build --target install --config RelWithDebInfo -j 16
```

# 一些常见和交叉编译相关命令

```
CC=aarch64-linux-gnu-gcc ./configure --enable-shared=no --host=aarch64-unknown-linux-gnu

/usr/local/llvm/18/bin/clang++ -v --target=aarch64-unknown-linux-gnu \
  --sysroot=/root/src/software/clang_sysroot \
  -I/root/src/software/clang_sysroot/include/aarch64-unknown-linux-gnu/c++/v1 \
  -I/root/src/software/clang_sysroot/include/c++/v1 \
  -I/root/src/software/clang_sysroot/lib/clang/18/include \
  -I/root/src/software/clang_sysroot/usr/sysinclude \
  -B/usr/local/llvm/18/bin \
  -L/root/src/software/clang_sysroot/lib \
  -L/root/src/software/clang_sysroot/lib/aarch64-unknown-linux-gnu \
  -L/root/src/software/clang_sysroot/lib/clang/18/lib/aarch64-unknown-linux-gnu \
  -L/root/src/software/clang_sysroot/usr/lib \
  -static -stdlib=libc++ -lc++ -lc++abi -lunwind -fuse-ld=lld --rtlib=compiler-rt \
  main.cc

LD_LIBRARY_PATH=/root/src/software/clang_sysroot/lib:\
/root/src/software/clang_sysroot/usr/lib:\
/root/src/software/clang_sysroot/usr/lib64:\
/root/src/software/clang_sysroot/lib/clang/18/lib/aarch64-unknown-linux-gnu:\
/root/src/software/clang_sysroot/lib/aarch64-unknown-linux-gnu qemu-aarch64 bazel-bin/src/main

```

# 一些常见软件编译参数

## openssl
指定rpath, 解决和系统自带openssl共存问题
```
openssl
LDFLAGS="-Wl,-rpath,/usr/local/lib64" ./Configure enable-brotli enable-egd enable-tfo enable-thread-pool enable-default-thread-pool enable-zlib enable-zstd
./Configure enable-brotli enable-egd enable-tfo enable-thread-pool enable-default-thread-pool enable-zlib enable-zstd --libdir=lib
```

## curl
```
autoreconf -fi
./configure --enable-versioned-symbols --with-openssl=/usr/local

cmake -DCMAKE_VERBOSE_MAKEFILE=TRUE -DCMAKE_BUILD_TYPE=RelWithDebInfo -DBUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_RPATH=/usr/local/lib64:/usr/local/lib ..
# libcurl.so.4: no version information available
```

## llvm/clang
```
cmake -G "Unix Makefiles" ../llvm \
    -DCMAKE_INSTALL_PREFIX=/usr/local/llvm/18 \
    -DCMAKE_BUILD_TYPE=Release \
    -DLLVM_ENABLE_PROJECTS="bolt;clang;clang-tools-extra;libclc;lld;lldb;mlir;polly;openmp" \
    -DLLVM_ENABLE_RUNTIMES="libc;libunwind;libcxxabi;pstl;libcxx;compiler-rt" \
    -DCLANG_DEFAULT_CXX_STDLIB=libc++ \
    -DCLANG_DEFAULT_RTLIB=compiler-rt \
    -DCLANG_DEFAULT_LINKER=lld \
    -DMLIR_INCLUDE_INTEGRATION_TESTS=OFF \
    -DLLVM_INCLUDE_TESTS=OFF \
    -DLLVM_BUILD_TESTS=OFF \
    -DLLDB_INCLUDE_TESTS=OFF \
    -DCLANG_INCLUDE_TESTS=OFF
```
