TODO
1. toolchains_llvm not works well for coss compiling
2. header and library std and system search not perfect in //toolchain:clang_toolchain_for_linux_aarch64
3. let toolchains_openwrt support gcc and clang and openwrt
4. swig
5. grpc
6. curl缺少quic + ngquic
7. glog external处理
8. include what you want
9. revocate
10. include检查
11. fizz aegis, liboqs



```
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


```
