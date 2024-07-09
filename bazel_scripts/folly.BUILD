package(default_visibility = ["//visibility:public"])

config_setting(
    name = "linux_x86_64",
    constraint_values = [
        "@platforms//os:linux",
        "@platforms//cpu:x86_64",
    ],
)

config_setting(
    name = "linux_aarch64",
    constraint_values = [
        "@platforms//os:linux",
        "@platforms//cpu:aarch64",
    ],
)

genrule(
    name = "folly-config_h",
    outs = ["folly/folly-config.h"],
    cmd = "\n".join([
        "cat <<'EOF' >$@",
        "/*",
        " * Copyright (c) Meta Platforms, Inc. and affiliates.",
        " *",
        " * Licensed under the Apache License, Version 2.0 (the \"License\");",
        " * you may not use this file except in compliance with the License.",
        " * You may obtain a copy of the License at",
        " *",
        " *     http://www.apache.org/licenses/LICENSE-2.0",
        " *",
        " * Unless required by applicable law or agreed to in writing, software",
        " * distributed under the License is distributed on an \"AS IS\" BASIS,",
        " * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.",
        " * See the License for the specific language governing permissions and",
        " * limitations under the License.",
        " */",
        "",
        "#pragma once",
        "",
        "#ifdef __APPLE__",
        "#include <TargetConditionals.h> // @manual",
        "#endif",
        "",
        "#if !defined(FOLLY_MOBILE)",
        "#if defined(__ANDROID__) || \\",
        "    (defined(__APPLE__) &&  \\",
        "     (TARGET_IPHONE_SIMULATOR || TARGET_OS_SIMULATOR || TARGET_OS_IPHONE))",
        "#define FOLLY_MOBILE 1",
        "#else",
        "#define FOLLY_MOBILE 0",
        "#endif",
        "#endif // FOLLY_MOBILE",
        "",
        "#define FOLLY_HAVE_PTHREAD 1",
        "#define FOLLY_HAVE_PTHREAD_ATFORK 1",
        "",
        "#define FOLLY_HAVE_LIBGFLAGS 1",
        "/* #undef FOLLY_UNUSUAL_GFLAGS_NAMESPACE */",
        "#define FOLLY_GFLAGS_NAMESPACE gflags",
        "",
        "#define FOLLY_HAVE_LIBGLOG 1",
        "",
        "#define FOLLY_USE_JEMALLOC 1",
        "",
        "#if __has_include(<features.h>)",
        "#include <features.h>",
        "#endif",
        "",
        "#define FOLLY_HAVE_ACCEPT4 1",
        "#define FOLLY_HAVE_GETRANDOM 1",
        "#define FOLLY_HAVE_PREADV 1",
        "#define FOLLY_HAVE_PWRITEV 1",
        "#define FOLLY_HAVE_CLOCK_GETTIME 1",
        "#define FOLLY_HAVE_PIPE2 1",
        "",
        "#define FOLLY_HAVE_IFUNC 1",
        "#define FOLLY_HAVE_UNALIGNED_ACCESS 1",
        "#define FOLLY_HAVE_VLA 1",
        "#define FOLLY_HAVE_WEAK_SYMBOLS 1",
        "#define FOLLY_HAVE_LINUX_VDSO 1",
        "#define FOLLY_HAVE_MALLOC_USABLE_SIZE 1",
        "/* #undef FOLLY_HAVE_INT128_T */",
        "#define FOLLY_HAVE_WCHAR_SUPPORT 1",
        "#define FOLLY_HAVE_EXTRANDOM_SFMT19937 1",
        "#define HAVE_VSNPRINTF_ERRORS 1",
        "",
        "#define FOLLY_HAVE_LIBUNWIND 1",
        "#define FOLLY_HAVE_DWARF 1",
        "#define FOLLY_HAVE_ELF 1",
        "#define FOLLY_HAVE_SWAPCONTEXT 1",
        "#define FOLLY_HAVE_BACKTRACE 1",
        "#define FOLLY_USE_SYMBOLIZER 1",
        "#define FOLLY_DEMANGLE_MAX_SYMBOL_SIZE 1024",
        "",
        "#define FOLLY_HAVE_SHADOW_LOCAL_WARNINGS 1",
        "",
        "#define FOLLY_HAVE_LIBLZ4 1",
        "#define FOLLY_HAVE_LIBLZMA 1",
        "#define FOLLY_HAVE_LIBSNAPPY 1",
        "#define FOLLY_HAVE_LIBZ 1",
        "#define FOLLY_HAVE_LIBZSTD 1",
        "#define FOLLY_HAVE_LIBBZ2 1",
        "",
        "#define FOLLY_LIBRARY_SANITIZE_ADDRESS 0",
        "",
        "/* #undef FOLLY_SUPPORT_SHARED_LIBRARY */",
        "",
        "#define FOLLY_HAVE_LIBRT 0",
        "EOF",
    ]),
)

cc_library(
    name = "folly",
    srcs = glob(
        [
            "folly/**/*.cpp",
        ],
        exclude = [
            "folly/experimental/symbolizer/tool/Addr2Line.cpp",
            "folly/io/tool/HugePageUtil.cpp",
            "folly/json/tool/JSONSchemaTester.cpp",
            "folly/tool/BenchmarkCompare.cpp",
            "folly/build/**",
            "folly/docs/**",
            "folly/external/aor/**",
            "folly/python/**/*.cpp",
            "folly/**/example/**",
            "folly/**/test/**/*.cpp",
        ],
    ) + select({
        ":linux_x86_64": [
            "folly/memset.S",
            "folly/memcpy.S",
        ],
        ":linux_aarch64": [
            "folly/external/aor/memcpy-advsimd.S",
            "folly/external/aor/memset-advsimd.S",
            "folly/external/aor/memcpy-armv8.S",
            "folly/external/aor/memcpy_sve.S",
        ],
    }),
    hdrs = ["folly-config_h"] + glob(
        [
            "folly/**/*.h",
        ],
        exclude = [
            "folly/external/aor/**",
            "folly/test/**/*.h",
            "folly/**/test/**/*.h",
            "folly/python/**/*.h",
        ],
    ) + select({
        ":linux_x86_64": [
        ],
        ":linux_aarch64": [
            "folly/external/aor/asmdefs.h",
        ],
    }),
    copts = [
        "-isystem external/folly",
        "-isystem $(BINDIR)/external/folly",
        "-isystem external/double-conversion",
        "-isystem external/libdwarf/src/lib/libdwarf",
        "-isystem external/lz4/lib",
        "-isystem external/com_github_google_snappy",
        "-isystem external/libsodium/src/libsodium/include",
        "-Iexternal/libsodium/src/libsodium/include/sodium",
        "-isystem $(BINDIR)/external/libsodium/src/libsodium/include",
        "-Wall",
        "-Wno-deprecated",
        "-Wno-deprecated-declarations",
        "-Wno-sign-compare",
        "-Wno-unused",
        "-Wunused-label",
        "-Wunused-result",
        "-Wuninitialized",
        "-faligned-new",
        "-Wshadow-compatible-local",
        "-Wno-noexcept-type",
        "-std=c++17",
        "-fPIC",
        "-finput-charset=UTF-8",
        "-fsigned-char",
        "-fopenmp",
        "-faligned-new",
        "-fcoroutines",
        "-msse4.2",
        "-mpclmul",
        "-mavx",
        "-mavx2",
    ],
    linkopts = [
        "-pthread",
        "-ldl",
        "-lstdc++",
    ],
    local_defines = [
        "_GNU_SOURCE",
        "_REENTRANT",
    ] + select({
        ":linux_x86_64": [
        ],
        ":linux_aarch64": [
            "-DFOLLY_MEMCPY_IS_MEMCPY",
            "-DFOLLY_MEMSET_IS_MEMSET",
        ],
    }),
    deps = [
        "@com_github_gflags_gflags//:gflags",
        "@com_github_glog_glog//:glog",
        "@com_github_google_snappy//:snappy",
        "@double-conversion//:double-conversion",
        "@fmt",
        "@jemalloc",
        "@libdwarf//:dwarf",
        "@libevent//:event",
        "@libevent//:event_openssl",
        "@libevent//:event_pthreads",
        "@libiberty//:iberty",
        "@libsodium//:sodium",
        "@libunwind//:unwind-all",
        "@openssl//:ssl",
        "@xz",
        "@zlib",
    ],
)
