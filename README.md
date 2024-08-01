TODO
0. GitManger need handle submodule problem and hiden ref problem
5. 代码风格检查，cpu性能分析，内存泄漏检查, gperftools + asnr, 单测覆盖度分析
6. toolchains_openwrt need download from remote, support gcc, clang, openwrt, windows, 多版本,全部注册
7. 文档
8. git sync use bare repo

future todo:
1. 学习boost的virtual include
1. use baze aspect find the most possible relative search path
2. curl缺少quic + ngquic
3. module map, -compiler_param_file, -layering_check
4. aspect用法
5. transitive用法
6. include what you want
7. renovate.json
8. fizz lack of aegis, liboqs

```
/usr/local/llvm-18/bin/clang++ -E -x c++ - -v < /dev/null

bazel build \
  --platforms=@toolchains_llvm//platforms:linux-x86_64 \
  --extra_execution_platforms=@toolchains_llvm//platforms:linux-x86_64 \
  --extra_toolchains=@llvm_toolchain_linux_exec//:cc-toolchain-x86_64-linux \
  //...

3.2 单测与代码格式
bazel test //... --test_tag_filters=cpplint    #只跑cpplint检查
bazel test //... --test_tag_filters=-cpplint   #不跑cpplint检查
bazel test //... --test_tag_filters=unit_test  #只跑单测
bazel test --config=unit_test //...  #根据.bazelrc配置文件，跑单测和内存泄露检查，不跑cpplint检查

3.3 覆盖率分析
bazel coverage //... --test_tag_filters=-cpplint
genhtml bazel-out/_coverage/_coverage_report.dat -o test_coverage
genhtml --ignore-errors source bazel-out/_coverage/_coverage_report.dat -o test_coverage

3.4 内存泄露分析
bazel test --config=unit_test //... #检测到内存泄露单测将失败，并查询详细日志即可
bazel test --test_env=HEAPCHECK=normal //...
bazel test --test_env=HEAPCHECK=normal --test_env=PPROF_PATH=/usr/local/bin/pprof //... #同时内存泄露检查和性能分析

3.5 cpu和内存性能分析
go install github.com/google/pprof@latest
bazel test --test_env="CPUPROFILE=prof.out" //src/common:barrier_test #需使用test --spawn_strategy=standalone

CPUPROFILE=prof.out bazel-bin/src/common/barrier_test

需要调用的是HeapProfilerStart和HeapProfilerStop
env HEAPPROFILE=heap.out bazel-bin/src/common/barrier_test

CPUPROFILE=prof.out LD_PRELOAD=/usr/local/lib/libtcmalloc_and_profiler.a bazel-bin/src/common/barrier_test
HEAPPROFILE=heap.out LD_PRELOAD=/usr/local/lib/libtcmalloc_and_profiler.a bazel-bin/src/common/barrier_test

google-pprof --web bazel-bin/src/common/barrier_test prof.out
pprof --text ./bazel-bin/src/common/barrier_test prof.out
pprof --pdf ./bazel-bin/src/common/barrier_test prof.out > profile.pdf
pprof ./bazel-bin/src/common/barrier_test "heap.prof" --inuse_objects --lines --heapcheck --edgefraction=1e-10 --nodefraction=1e-10 --gv
pprof ./bazel-bin/src/common/barrier_test "heap.prof" --inuse_objects --lines --heapcheck --edgefraction=1e-10 --nodefraction=1e-10 --pdf > profile.pdf

perf record -F 99 -g bazel-bin/src/demo 10000
perf script | /data/software/FlameGraph/stackcollapse-perf.pl | /data/software/FlameGraph/flamegraph.pl > flamegraph.svg

https://gperftools.github.io/gperftools/heapprofile.html
https://gperftools.github.io/gperftools/cpuprofile.html
https://gperftools.github.io/gperftools/heap_checker.html
https://gperftools.github.io/gperftools/tcmalloc.html

3.6 依赖图等查询
bazel query --notool_deps --noimplicit_deps "deps(//main:hello-world)" --output graph
bazel query 'attr(visibility, "//visibility:public", //:*)'
bazel query "deps(//src/context:process_context)"
bazel query 'deps(//:main)' --output graph > graph.in
bazel query --noimplicit_deps 'deps(//:main)' --output graph > simplified_graph.in
dot -Tpng < graph.in > graph.png

3.7 系统调用分析
cat /proc/self/stack
cat /proc/21880/stack
strace -Ff -tt -p 56509 2>&1 | tee strace.log
pstack 56509
```

```
openssl
LDFLAGS="-Wl,-rpath,/usr/local/lib64" ./Configure enable-brotli enable-egd enable-tfo enable-thread-pool enable-default-thread-pool enable-zlib enable-zstd
./Configure enable-brotli enable-egd enable-tfo enable-thread-pool enable-default-thread-pool enable-zlib enable-zstd --libdir=lib

curl
autoreconf -fi
./configure --enable-versioned-symbols --with-openssl=/usr/local

cmake -DCMAKE_VERBOSE_MAKEFILE=TRUE -DCMAKE_BUILD_TYPE=RelWithDebInfo -DBUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_RPATH=/usr/local/lib64:/usr/local/lib ..
# libcurl.so.4: no version information available

mstch_cpp2:no_metadata,include_prefix=thrift/lib/thrift
mstch_cpp2:templates,no_metadata,include_prefix=thrift/lib/thrift
mstch_cpp2:json,no_metadata,include_prefix=thrift/lib/thrift
mstch_cpp2:include_prefix=thrift/lib/thrift

cd /root/src/library/fbthrift && /root/src/library/fbthrift/bazel-bin/thrift1 --gen mstch_cpp2:no_metadata,include_prefix=thrift/conformance/if \
-o thrift/conformance/if \
-I /root/src/library/fbthrift thrift/conformance/if/serialization.thrift

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

TARGET=aarch64-linux-gnu cmake \
   -DCMAKE_C_FLAGS="--sysroot=/root/src/software/openwrt/toolchain/" \
   -DCMAKE_CXX_FLAGS="--sysroot=/root/src/software/openwrt/toolchain/" \
   -DCMAKE_C_COMPILER=/root/src/software/openwrt/toolchain/bin/aarch64-openwrt-linux-musl-gcc-12.3.0 \
   -DCMAKE_CXX_COMPILER=/root/src/software/openwrt/toolchain/bin/aarch64-openwrt-linux-musl-g++.bin  \
   -DCMAKE_LINKER=/root/src/software/openwrt/toolchain/bin/aarch64-openwrt-linux-musl-ld.bin \
   -DCMAKE_AR=/root/src/software/openwrt/toolchain/bin/aarch64-openwrt-linux-musl-gcc-ar \
   -DCMAKE_ASM_COMPILER=/root/src/software/openwrt/toolchain/bin/aarch64-openwrt-linux-musl-as.bin \
   -DCMAKE_RANLIB=/root/src/software/openwrt/toolchain/bin/aarch64-openwrt-linux-musl-gcc-ranlib \
   -DCMAKE_NM=/root/src/software/openwrt/toolchain/bin/aarch64-openwrt-linux-musl-gcc-nm \
   -DCMAKE_STRIP=/root/src/software/openwrt/toolchain/bin/aarch64-openwrt-linux-musl-strip \
   -DCMAKE_SYSROOT=/root/src/software/openwrt/toolchain \
   -DCMAKE_INCLUDE_PATH=":" \
   -DCMAKE_LIBRARY_PATH="/root/src/software/openwrt/toolchain/lib" \
   -DCMAKE_INSTALL_RPATH="/root/src/software/openwrt/toolchain/lib" \
   -DCMAKE_SHARED_LINKER_FLAGS="--sysroot=/root/src/software/openwrt/toolchain" \
   ..

CC=/root/src/software/openwrt/toolchain/bin/aarch64-openwrt-linux-musl-gcc-12.3.0 \
CXX=/root/src/software/openwrt/toolchain/bin/aarch64-openwrt-linux-musl-g++.bin \
CCAS=/root/src/software/openwrt/toolchain/bin/aarch64-openwrt-linux-musl-as.bin \
CXXFLAGS="--sysroot=/root/src/software/openwrt/toolchain -isystem /root/src/software/openwrt/toolchain/aarch64-openwrt-linux-musl/include/c++/12.3.0" \
CFLAGS="--sysroot=/root/src/software/openwrt/toolchain -isystem /root/src/software/openwrt/toolchain/include" \
LDFLAGS="--sysroot=/root/src/software/openwrt/toolchain -L/root/src/software/openwrt/toolchain/lib" ./configure --host=aarch64-unknown-linux-gnu


cmake \
  -DCMAKE_INCLUDE_PATH=/alt/include/path1:/alt/include/path2 \
  -DCMAKE_LIBRARY_PATH=/alt/lib/path1:/alt/lib/path2 ...
liuring
./configure \
 --cc=/root/src/software/openwrt/toolchain/bin/aarch64-openwrt-linux-musl-gcc-12.3.0 \
 --cxx=/root/src/software/openwrt/toolchain/bin/aarch64-openwrt-linux-musl-g++.bin  \
 --includedir=/root/src/software/openwrt/toolchain/include \
 --libdir=/root/src/software/openwrt/toolchain/lib \
 --libdevdir=/root/src/software/openwrt/toolchain/lib

mbedtls
cmake -DCMAKE_VERBOSE_MAKEFILE=TRUE -DENABLE_TESTING=OFF -DUSE_SHARED_MBEDTLS_LIBRARY=ON ..


libevent
cmake -DCMAKE_VERBOSE_MAKEFILE=TRUE -DEVENT__DISABLE_TESTS:BOOL=ON -DEVENT__ENABLE_VERBOSE_DEBUG:BOOL=OFF ..


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


/usr/local/llvm/18/bin/clang++ -v --target=aarch64-unknown-linux-gnu \
  --sysroot=/root/src/software/clang_sysroot \
  -I/root/src/software/clang_sysroot/include/aarch64-unknown-linux-gnu/c++/v1 \
  -I/root/src/software/clang_sysroot/include/c++/v1 \
  -I/root/src/software/clang_sysroot/lib/clang/18/include \
  -isystem /root/src/software/clang_sysroot/usr/sysinclude \
  -B/usr/local/llvm/18/bin \
  -L/root/src/software/clang_sysroot/lib \
  -L/root/src/software/clang_sysroot/lib/aarch64-unknown-linux-gnu \
  -L/root/src/software/clang_sysroot/lib/clang/18/lib/aarch64-unknown-linux-gnu \
  -L/root/src/software/clang_sysroot/usr/lib \
  -static -stdlib=libc++ -lc++ -lc++abi -lunwind -fuse-ld=lld --rtlib=compiler-rt \
  main.cc


/usr/local/llvm/18/bin/clang-18 -cc1 -triple aarch64-unknown-linux-gnu \
-emit-obj -mrelax-all -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names \
-main-file-name main.cc -static-define -mrelocation-model pic -pic-level 2 -pic-is-pie \
-mframe-pointer=non-leaf -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 \
-target-cpu generic -target-feature +v8a -target-feature +fp-armv8 -target-feature +neon -target-abi aapcs -debugger-tuning=gdb \
-fdebug-compilation-dir=/root/src/software -v -fcoverage-compilation-dir=/root/src/software \
-resource-dir /usr/local/llvm/18/lib/clang/18 \
-isysroot /root/src/software/clang_sysroot \
-internal-isystem /root/src/software/clang_sysroot/include/aarch64-unknown-linux-gnu/c++/v1 \
-internal-isystem /usr/local/llvm/18/bin/../include/c++/v1 \
-internal-isystem /usr/local/llvm/18/lib/clang/18/include \
-internal-externc-isystem /root/src/software/clang_sysroot/usr/include \
-internal-externc-isystem /root/src/software/clang_sysroot/include \
-fdeprecated-macro -ferror-limit 19 -fno-signed-char -fgnuc-version=4.2.1 \
-fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics \
-target-feature +outline-atomics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 \
-o /tmp/main-8512fd.o -x c++ main.cc


LD_LIBRARY_PATH=/root/src/software/openwrt/openwrt-toolchain-23.05.3-rockchip-armv8_gcc-12.3.0_musl.Linux-x86_64/toolchain-aarch64_generic_gcc-12.3.0_musl/lib qemu-aarch64 bazel-bin/src/main
LD_LIBRARY_PATH=/root/src/software/clang_sysroot/lib:/root/src/software/clang_sysroot/usr/lib:/root/src/software/clang_sysroot/usr/lib64:/root/src/software/clang_sysroot/lib/clang/18/lib/aarch64-unknown-linux-gnu:/root/src/software/clang_sysroot/lib/aarch64-unknown-linux-gnu qemu-aarch64 bazel-bin/src/main


CMAKE_PREFIX_PATH=\
      /tmp/fbcode_builder_getdeps-ZrootZsrcZlibraryZfollyZbuildZfbcode_builder-root/installed/boost-AjmARan9-QjeQELFQiaLfua8c_9tx8aLqAFh9QdqSd4:\
      /tmp/fbcode_builder_getdeps-ZrootZsrcZlibraryZfollyZbuildZfbcode_builder-root/installed/folly:\
 GETDEPS_BUILD_DIR=/tmp/fbcode_builder_getdeps-ZrootZsrcZlibraryZfollyZbuildZfbcode_builder-root/build \
 GETDEPS_CABAL_FLAGS='--extra-lib-dirs=/tmp/fbcode_builder_getdeps-ZrootZsrcZlibraryZfollyZbuildZfbcode_builder-root/installed/boost-AjmARan9-QjeQELFQiaLfua8c_9tx8aLqAFh9QdqSd4/lib --extra-include-dirs=/tmp/fbcode_builder_getdeps-ZrootZsrcZlibraryZfollyZbuildZfbcode_builder-root/installed/boost-AjmARan9-QjeQELFQiaLfua8c_9tx8aLqAFh9QdqSd4/include --extra-lib-dirs=/tmp/fbcode_builder_getdeps-ZrootZsrcZlibraryZfollyZbuildZfbcode_builder-root/installed/double-conversion-x8emC6iUhLOnT7N5UzjEF0T6GeLgN1y8NX2n84s-9po/lib --extra-include-dirs=/tmp/fbcode_builder_getdeps-ZrootZsrcZlibraryZfollyZbuildZfbcode_builder-root/installed/double-conversion-x8emC6iUhLOnT7N5UzjEF0T6GeLgN1y8NX2n84s-9po/include --extra-lib-dirs=/tmp/fbcode_builder_getdeps-ZrootZsrcZlibraryZfollyZbuildZfbcode_builder-root/installed/snappy-gxfOyHGf2FJ0ul7TAj3MVmChv9-6i3Mb1_l5gXysG90/lib --extra-include-dirs=/tmp/fbcode_builder_getdeps-ZrootZsrcZlibraryZfollyZbuildZfbcode_builder-root/installed/snappy-gxfOyHGf2FJ0ul7TAj3MVmChv9-6i3Mb1_l5gXysG90/include --extra-lib-dirs=/tmp/fbcode_builder_getdeps-ZrootZsrcZlibraryZfollyZbuildZfbcode_builder-root/installed/libiberty-BGHXCgZYbgyPPISiK8glfhC9K-oPT_dshcr_bWEn0Z4/lib --extra-include-dirs=/tmp/fbcode_builder_getdeps-ZrootZsrcZlibraryZfollyZbuildZfbcode_builder-root/installed/libiberty-BGHXCgZYbgyPPISiK8glfhC9K-oPT_dshcr_bWEn0Z4/include' \
 GETDEPS_INSTALL_DIR=/tmp/fbcode_builder_getdeps-ZrootZsrcZlibraryZfollyZbuildZfbcode_builder-root/installed \
 LD_LIBRARY_PATH=\
      /tmp/fbcode_builder_getdeps-ZrootZsrcZlibraryZfollyZbuildZfbcode_builder-root/installed/boost-AjmARan9-QjeQELFQiaLfua8c_9tx8aLqAFh9QdqSd4/lib:\
      /tmp/fbcode_builder_getdeps-ZrootZsrcZlibraryZfollyZbuildZfbcode_builder-root/installed/double-conversion-x8emC6iUhLOnT7N5UzjEF0T6GeLgN1y8NX2n84s-9po/lib:\
 PATH=\
      /tmp/fbcode_builder_getdeps-ZrootZsrcZlibraryZfollyZbuildZfbcode_builder-root/installed/ninja-a0uJUvTTC6-ZhmLhh59nTaABIYis4_IlMXls0u_7NTI/bin:\
 PKG_CONFIG_PATH=\
      /tmp/fbcode_builder_getdeps-ZrootZsrcZlibraryZfollyZbuildZfbcode_builder-root/installed/xz-95wfh-pRiHkAz3OGd-E0CZfed-V-cu1gY69h-MQKsD4/lib/pkgconfig:\
 SSL_CERT_DIR=/etc/ssl/certs \
 cd /tmp/fbcode_builder_getdeps-ZrootZsrcZlibraryZfollyZbuildZfbcode_builder-root/build/folly && \
 /tmp/fbcode_builder_getdeps-ZrootZsrcZlibraryZfollyZbuildZfbcode_builder-root/installed/cmake-p1rPk5ixSPag0XJ4Imj7OHLK5wq2mlYJJ0zNN_bttxo/bin/cmake \
      --build \
      /tmp/fbcode_builder_getdeps-ZrootZsrcZlibraryZfollyZbuildZfbcode_builder-root/build/folly \
      --target \
      install \
      --config \
      RelWithDebInfo \
      -j \
      30
```
