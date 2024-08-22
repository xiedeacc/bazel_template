# todo
6. toolchains_openwrt need windows

# future todo:
3. import virtual include like boost
4. use baze aspect find the most possible relative search path
5. 集成clang-tidy, clang-check, clang static analyzer
6. module map, -compiler_param_file, -layering_check
7. aspect用法
8. transitive用法
9. renovate.json

# features
* include most popular C/C++ libraries, eg. grpc protobuf boost abseil folly proxygen mvfst jemalloc tcmalloc xz zstd curl c-ares ...
* include grpc server and client example
* include http server and client example
* include protobuf plugin example
* include swig for JAVA and Python example
* support multiple platforms, eg: linux-x86_64 linux_aarch64 openwrt_aarch64 and macosx, windows ing
* all source code compiled successful by clang and gcc both
* include compile_commands.json generator
* include cpu and memory profiling
* include memory leak, corruption detection
* automatic code google style detect

# usage

## build
```
clear && bazel test --config=cpplint //...
clear && bazel test --config=unit_test //...
clear && bazel test --config=sanitize //...
clear && bazel test --config=gcc --config=unit_test_tcmalloc //...
clear && bazel test --config=gcc --config=unit_test_jemalloc //...
clear && bazel test --config=gcc --config=sanitize //...
clear && bazel test --config=clang --config=unit_test_tcmalloc //...
clear && bazel test --config=clang --config=unit_test_jemalloc //...
clear && bazel test --config=clang --config=sanitize //...

clear && bazel build --config=gcc --config=tcmalloc //...
clear && bazel build --config=gcc --config=jemalloc //...
clear && bazel build --config=gcc_macosx_x86_64 //...
clear && bazel build --config=openwrt_aarch64 //...

clear && bazel build --config=clang --config=tcmalloc //...
clear && bazel build --config=clang --config=jemalloc //...
clear && bazel build --config=clang_aarch64 //...
clear && bazel build --config=clang_macosx_x86_64 //...

clear && bazel coverage //...

```

## 生成compile_commands.json
```
clear && bazel run @hedron_compile_commands//:refresh_all
```

## 单测与代码风格
```
bazel test //... --config=unit_test_tcmalloc #单测和内存泄露检查
bazel test //... --config=unit_test_jemalloc #单测和内存泄露检查
bazel test //... --config=cpplint            #只跑cpplint检查
```

## 内存leak检测
```
bazel test //... --config=unit_test_jemalloc #单测和内存泄露检查
https://github.com/jemalloc/jemalloc/wiki/Use-Case%3A-Leak-Checking
```

## 内存corruption
```
bazel test --config=sanitize //...
```

## cpu性能和内存分析
```
bazel test --test_env="CPUPROFILE=prof.out" //src/common:host_spec_test #prof.out在bazel构建根目录下
CPUPROFILE=prof.out bazel-bin/src/common/host_spec_test

pprof --web bazel-bin/src/common/host_spec_test prof.out
pprof --text ./bazel-bin/src/common/host_spec_test prof.out
pprof --pdf ./bazel-bin/src/common/host_spec_test prof.out > profile.pdf

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
bazel query "rdeps(//..., //src/util:util)"
bazel query "rdeps(//..., @com_google_protobuf//:protobuf)"
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

用LD_PRELOAD不生效时，可以用下面命令check下soname信息是否写，没有写入,可以使用
cc_binary(
    name = "libtcmalloc.so",
    linkopts = ["-Wl,-soname,libtcmalloc.so"],
    linkshared = 1,
    deps = [":tcmalloc"],
)

readelf -Wd libtcmalloc.so | grep SONAME 

https://wizardforcel.gitbooks.io/100-gcc-tips/content/index.html

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
crosstool-NG
ct-ng list-samples

binutils
mkdir build
../configure --prefix=/usr/local/binutils-linux-x86_64
../configure --target=x86_64-apple-darwin \
  --prefix=/usr/local/binutils-darwin \
  --with-sysroot=/root/src/software/osxcross/target/SDK/MacOSX14.2.sdk

cctools
./configure --prefix=/root/src/software/osxcross/target \
  --target=x86_64-apple-darwin23.3 \
  --with-libtapi=/root/src/software/osxcross/target \
  --with-libxar=/root/src/software/osxcross/target \
  --enable-static=yes --disable-shared

osxcross
LDFLAGS=-L/usr/local/llvm/18/lib/x86_64-unknown-linux-gnu LIBS=-lunwind ./configure \
  --prefix=/root/src/software/osxcross/target \
  --target=x86_64-apple-darwin23.3 \
  --with-libtapi=/root/src/software/osxcross/target \
  --with-libxar=/root/src/software/osxcross/target

gcc
export LD=/usr/local/llvm/18/bin/ld.lld
export AR=/root/src/software/osxcross/target/bin/x86_64-apple-darwin23.3-ar
export AS=/root/src/software/osxcross/target/bin/x86_64-apple-darwin23.3-as
../configure \
  --prefix=/root/src/software/gcc14.1.0-darwin23.3-x86_64_toolchain \
  --target=x86_64-apple-darwin23.3 \
  --with-sysroot=/root/src/software/osxcross/target/SDK/MacOSX14.2.sdk \
  --enable-languages=c,c++,objc,obj-c++ \
  --enable-lto \
  --enable-checking=release \
  --disable-multilib

LLVM_DIR=/usr/local/llvm/18 \
CC=${LLVM_DIR}/bin/clang \
CPPFLAGS="-I${LLVM_DIR}/include -I${LLVM_DIR}/include/x86_64-unknown-linux-gnu/c++/v1 -I${LLVM_DIR}/include/c++/v1 -I${LLVM_DIR}/lib/clang/18/include -I/usr/include" \
CFLAGS="-v -fPIC -I${LLVM_DIR}/include -I${LLVM_DIR}/include/x86_64-unknown-linux-gnu/c++/v1 -I${LLVM_DIR}/include/c++/v1 -I${LLVM_DIR}/lib/clang/18/include -I/usr/include" \
LDFLAGS="-L${LLVM_DIR}/lib -L${LLVM_DIR}/lib/x86_64-unknown-linux-gnu -L${LLVM_DIR}/lib/clang/18/lib/x86_64-unknown-linux-gnu -L/usr/lib" \
./configure

CC='/root/src/software/gcc14.2.0-windows-x86_64_toolchain/bin/x86_64-w64-mingw32-gcc' \
CXX='/root/src/software/gcc14.2.0-windows-x86_64_toolchain/bin/x86_64-w64-mingw32-g++' \
CFLAGS='--sysroot=/root/src/software/gcc14.2.0-windows-x86_64_toolchain -I/root/src/software/gcc14.2.0-windows-x86_64_toolchain/lib/gcc/x86_64-w64-mingw32/14.2.0/include -I/root/src/software/gcc14.2.0-windows-x86_64_toolchain/lib/gcc/x86_64-w64-mingw32/14.2.0/include-fixed -I/root/src/software/gcc14.2.0-windows-x86_64_toolchain/x86_64-w64-mingw32/sysroot/usr/x86_64-w64-mingw32/include' \
CXXFLAGS='--sysroot=/root/src/software/gcc14.2.0-windows-x86_64_toolchain -I/root/src/software/gcc14.2.0-windows-x86_64_toolchain/x86_64-w64-mingw32/include/c++/14.2.0/x86_64-w64-mingw32 -I/root/src/software/gcc14.2.0-windows-x86_64_toolchain/x86_64-w64-mingw32/include/c++/14.2.0 -I/root/src/software/gcc14.2.0-windows-x86_64_toolchain/x86_64-w64-mingw32/include/c++/14.2.0/backward -I/root/src/software/gcc14.2.0-windows-x86_64_toolchain/lib/gcc/x86_64-w64-mingw32/14.2.0/include -I/root/src/software/gcc14.2.0-windows-x86_64_toolchain/lib/gcc/x86_64-w64-mingw32/14.2.0/include-fixed -I/root/src/software/gcc14.2.0-windows-x86_64_toolchain/x86_64-w64-mingw32/sysroot/usr/x86_64-w64-mingw32/include' \
LDFLAGS='-B/root/src/software/gcc14.2.0-windows-x86_64_toolchain/bin -B/root/src/software/gcc14.2.0-windows-x86_64_toolchain/x86_64-w64-mingw32/sysroot/usr/x86_64-w64-mingw32/lib/ -L/root/src/software/gcc14.2.0-windows-x86_64_toolchain/lib -L/root/src/software/gcc14.2.0-windows-x86_64_toolchain/lib/gcc/x86_64-w64-mingw32/14.2.0 -L/root/src/software/gcc14.2.0-windows-x86_64_toolchain/x86_64-w64-mingw32/lib -L/root/src/software/gcc14.2.0-windows-x86_64_toolchain/sysroot/usr/x86_64-w64-mingw32/lib ' \
./configure --host=x86_64-w64-mingw32 --disable-shared

CC='/root/src/software/gcc14.2.0-windows-x86_64_toolchain/bin/x86_64-w64-mingw32-gcc' \
CXX='/root/src/software/gcc14.2.0-windows-x86_64_toolchain/bin/x86_64-w64-mingw32-g++' \
CFLAGS='--sysroot=/root/src/software/gcc14.2.0-windows-x86_64_toolchain -I/root/src/software/gcc14.2.0-windows-x86_64_toolchain/lib/gcc/x86_64-w64-mingw32/14.2.0/include -I/root/src/software/gcc14.2.0-windows-x86_64_toolchain/lib/gcc/x86_64-w64-mingw32/14.2.0/include-fixed -I/root/src/software/gcc14.2.0-windows-x86_64_toolchain/x86_64-w64-mingw32/sysroot/usr/x86_64-w64-mingw32/include' \
CXXFLAGS='--sysroot=/root/src/software/gcc14.2.0-windows-x86_64_toolchain -I/root/src/software/gcc14.2.0-windows-x86_64_toolchain/x86_64-w64-mingw32/include/c++/14.2.0/x86_64-w64-mingw32 -I/root/src/software/gcc14.2.0-windows-x86_64_toolchain/x86_64-w64-mingw32/include/c++/14.2.0 -I/root/src/software/gcc14.2.0-windows-x86_64_toolchain/x86_64-w64-mingw32/include/c++/14.2.0/backward -I/root/src/software/gcc14.2.0-windows-x86_64_toolchain/lib/gcc/x86_64-w64-mingw32/14.2.0/include -I/root/src/software/gcc14.2.0-windows-x86_64_toolchain/lib/gcc/x86_64-w64-mingw32/14.2.0/include-fixed -I/root/src/software/gcc14.2.0-windows-x86_64_toolchain/x86_64-w64-mingw32/sysroot/usr/x86_64-w64-mingw32/include' \
LDFLAGS='-B/root/src/software/gcc14.2.0-windows-x86_64_toolchain/bin -B/root/src/software/gcc14.2.0-windows-x86_64_toolchain/x86_64-w64-mingw32/sysroot/usr/x86_64-w64-mingw32/lib/ -L/root/src/software/gcc14.2.0-windows-x86_64_toolchain/lib -L/root/src/software/gcc14.2.0-windows-x86_64_toolchain/lib/gcc/x86_64-w64-mingw32/14.2.0 -L/root/src/software/gcc14.2.0-windows-x86_64_toolchain/x86_64-w64-mingw32/lib -L/root/src/software/gcc14.2.0-windows-x86_64_toolchain/sysroot/usr/x86_64-w64-mingw32/lib ' \
cmake ..

ls | xargs /root/.cache/bazel/_bazel_root/d7f4cad81e2b036f4c3382027c9d9c7a/external/cc_toolchain_repo_x86_64_windows_generic_mingw-w64_gcc/bin/x86_64-w64-mingw32-nm -A | grep fseek
find . -name "*.o" -exec nm -A {} + | grep "symbol_name"


export PATH=/root/src/software/gcc14.2.0-windows-x86_64_toolchain/bin:$PATH
./Configure mingw64 --cross-compile-prefix=x86_64-w64-mingw32- no-shared no-dso
make install DESTDIR=/root/src/software/gcc14.2.0-windows-x86_64_toolchain/x86_64-w64-mingw32/sysroot/usrmake install DESTDIR=/root/src/software/gcc14.2.0-windows-x86_64_toolchain/x86_64-w64-mingw32/sysroot/usr

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

## binutils
```
https://gcc.gnu.org/pub/gcc/infrastructure/
https://gcc.gnu.org/pub/gcc/infrastructure/mpfr-4.1.0.tar.bz2
https://gcc.gnu.org/pub/gcc/infrastructure/gmp-6.2.1.tar.bz2
https://gcc.gnu.org/pub/gcc/infrastructure/isl-0.24.tar.bz2
https://gcc.gnu.org/pub/gcc/infrastructure/mpc-1.2.1.tar.gz
https://ftp.gnu.org/gnu/binutils/binutils-2.43.tar.gz
cd bintuils
mkdir build
cd build
../configure

```

## protobuf
```
clear && bazel build //src/google/protobuf:protobuf
cp bazel-bin/protoc /usr/local/lib
cp bazel-bin/src/google/protobuf/libprotobuf.a /usr/local/lib
cp bazel-bin/src/google/protobuf/libprotobuf.so /usr/local/lib
cp -r src/google /usr/local/include

```

## absl
```
cmake BUILD_TESTING=OFF -DABSL_PROPAGATE_CXX_STD=ON -DBUILD_SHARED_LIBS=ON ..
cmake BUILD_TESTING=OFF -DABSL_PROPAGATE_CXX_STD=ON ..

```

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

## gcc
```
export PATH=/root/src/software/osxcross/target/bin:$PATH
../configure \
  --prefix=/root/src/software/gcc14.1.0-darwin23.3-x86_64_toolchain \
  --target=x86_64-apple-darwin23.3 \
  --with-sysroot=/root/src/software/osxcross/target/SDK/MacOSX14.2.sdk \
  --enable-languages=c,c++,objc,obj-c++ \
  --enable-lto \
  --enable-checking=release \
  --disable-multilib \
  --with-ld=/root/src/software/osxcross/target/bin/x86_64-apple-darwin23.3-ld \
  --with-ar=/root/src/software/osxcross/target/bin/x86_64-apple-darwin23.3-ar \
  --with-as=/root/src/software/osxcross/target/bin/x86_64-apple-darwin23.3-as
```
## llvm/clang
```
cmake -G "Unix Makefiles" ../llvm \
    -DCMAKE_INSTALL_PREFIX=/usr/local/llvm/18 \
    -DCMAKE_BUILD_TYPE=Release \
    -DLLVM_ENABLE_PROJECTS="bolt;clang;clang-tools-extra;compiler-rt;cross-project-tests;libc;libclc;lld;lldb;mlir;polly;pstl;openmp" \
    -DLLVM_ENABLE_RUNTIMES="libc;libunwind;libcxxabi;libcxx" \
    -DCLANG_DEFAULT_CXX_STDLIB=libc++ \
    -DCLANG_DEFAULT_RTLIB=compiler-rt \
    -DCLANG_DEFAULT_LINKER=lld \
    -DMLIR_INCLUDE_INTEGRATION_TESTS=OFF \
    -DLLVM_INCLUDE_TESTS=OFF \
    -DLLVM_BUILD_TESTS=OFF \
    -DLLDB_INCLUDE_TESTS=OFF \
    -DCLANG_INCLUDE_TESTS=OFF

```
