# features
* include most popular C/C++ libraries, eg. grpc, protobuf, boost, abseil, folly, proxygen, mvfst, jemalloc, zstd, curl, openssl ...
* include grpc server and client example
* include http server and client example
* include protobuf plugin example
* include swig for Java and Python example
* support multiple os, eg: linux, macosx, windows
* support multiple cpu, eg: x86_64, aarch64
* all source code compiled successful by clang, gcc, msvc-cl
* include compile_commands.json generator
* include cpu and memory profiling
* include memory leak, corruption detection
* automatic code style detect

# future todo:
1. compiler use from @bazel_tools//tools/cpp:compiler
2. use aspect find the most possible relative search path
3. transitive usage
4. renovate.json
5. integrate clang-tidy, clang-check, clang static analyzer https://github.com/erenon/bazel_clang_tidy
6. integrate IWYU https://github.com/storypku/bazel_iwyu

# usage

## generate another repo
```
./generate.sh repo_name
```

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

## generate compile_commands.json
```
clear && bazel run @hedron_compile_commands//:refresh_all
```

## unit test and code style detect
```
bazel test //... --config=unit_test_tcmalloc #单测和内存泄露检查
bazel test //... --config=unit_test_jemalloc #单测和内存泄露检查
bazel test //... --config=cpplint            #只跑cpplint检查
```
## unit test coverage
```
bazel coverage //... --test_tag_filters=-cpplint
genhtml bazel-out/k8-fastbuild/testlogs/src/common/host_spec_test/coverage.dat -o /zfs/www/coverage
genhtml --ignore-errors source bazel-out/k8-fastbuild/testlogs/src/common/host_spec_test/coverage.dat -o /zfs/www/test_coverage
https://coverage.xiamu.com
```

## mem leak detect
```
bazel test //... --config=unit_test_jemalloc #单测和内存泄露检查
https://github.com/jemalloc/jemalloc/wiki/Use-Case%3A-Leak-Checking
```

## mem corruption detect
```
bazel test --config=sanitize //...
```

## cpu performance profiling
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

## other bazel command
```
bazel query --notool_deps --noimplicit_deps "deps(//src/server:grpc_server)" --output graph
bazel query 'attr(visibility, "//visibility:public", //:*)'
bazel query "rdeps(//..., //src/util:util)"
bazel query "rdeps(//..., @com_google_protobuf//:protobuf)"
bazel query 'deps(//src/server:grpc_server)' --output graph > graph.in
bazel query --noimplicit_deps 'deps(//:main)' --output graph > simplified_graph.in
dot -Tpng < graph.in > graph.png
```

## strace
```
cat /proc/self/stack
cat /proc/21880/stack
strace -Ff -tt -p 56509 2>&1 | tee strace.log
pstack 56509
```
