load("@bazel_skylib//lib:selects.bzl", "selects")
load("@bazel_template//bazel:common.bzl", "GLOBAL_COPTS", "GLOBAL_LINKOPTS", "GLOBAL_LOCAL_DEFINES", "template_rule")

package(default_visibility = ["//visibility:public"])

COPTS = GLOBAL_COPTS + select({
    "@platforms//os:windows": [
        "/Ox",
        "/Iexternal/libdwarf/src/lib/libdwarf",
        "/Iexternal/folly",
        "/I$(GENDIR)/external/folly",
        "/Iexternal/zstd/lib",
        "/Iexternal/double-conversion",
        "/Iexternal/lz4/lib",
        "/Iexternal/bzip2",
        "/Iexternal/com_github_google_snappy",
        "/Iexternal/libsodium/src/libsodium/include",
        "/Iexternal/libsodium/src/libsodium/include/sodium",
        "/I$(GENDIR)/external/libsodium/src/libsodium/include",
    ],
    "//conditions:default": [
        "-std=c++17",
        "-Iexternal/libdwarf/src/lib/libdwarf",
        "-isystem external/libiberty/include",
        "-isystem external/folly",
        "-isystem $(GENDIR)/external/folly",
        "-isystem external/zstd/lib",
        "-isystem external/double-conversion",
        "-isystem external/lz4/lib",
        "-isystem external/bzip2",
        "-isystem external/com_github_google_snappy",
        "-isystem external/libsodium/src/libsodium/include",
        "-Iexternal/libsodium/src/libsodium/include/sodium",
        "-isystem $(GENDIR)/external/libsodium/src/libsodium/include",
        "-Wall",
        "-O3",
        "-Wno-deprecated",
        "-Wno-deprecated-declarations",
        "-Wno-sign-compare",
        "-Wno-unused",
        "-Wunused-label",
        "-Wunused-result",
        "-Wuninitialized",
        "-Wshadow-compatible-local",
        "-Wno-noexcept-type",
        "-finput-charset=UTF-8",
        "-fsigned-char",
        "-faligned-new",
    ],
}) + select({
    "@platforms//os:linux": [
        "-I$(GENDIR)/external/libunwind/include",
        "-Iexternal/libunwind/src",
        "-Iexternal/libunwind/include",
        "-I$(GENDIR)/external/libunwind/include/tdep",
        "-Iexternal/libunwind/include/tdep",
        "-Iexternal/libunwind/src/mi",
        "-fopenmp",
        "-fcoroutines",
    ],
    "@platforms//os:osx": [],
    "@platforms//os:windows": [],
    "//conditions:default": [],
})

LOCAL_DEFINES = GLOBAL_LOCAL_DEFINES + [
    "HAVE_CONFIG_H",
] + select({
    "@platforms//os:windows": [
        "_CRT_NONSTDC_NO_WARNINGS",
        "_CRT_SECURE_NO_WARNINGS",
        "_SCL_SECURE_NO_WARNINGS",
        "_ENABLE_EXTENDED_ALIGNED_STORAGE",
        "WIN32_LEAN_AND_MEAN",
    ],
    "//conditions:default": [
        "_GNU_SOURCE",
        "_REENTRANT",
        "_LARGEFILE64_SOURCE",
    ],
})

cc_library(
    name = "MathOperation_AVX2",
    srcs = [
        "folly/crypto/detail/MathOperation_AVX2.cpp",
    ],
    copts = COPTS + select({
        "@platforms//cpu:x86_64": ["-mavx"],
        "@platforms//cpu:aarch64": [],
    }),
    local_defines = LOCAL_DEFINES,
    deps = [":common"],
)

cc_library(
    name = "MathOperation",
    srcs = [
        "folly/crypto/detail/MathOperation_SSE2.cpp",
        "folly/crypto/detail/MathOperation_Simple.cpp",
    ],
    copts = COPTS + select({
        "@platforms//cpu:x86_64": ["-mno-avx"],
        "@platforms//cpu:aarch64": [],
    }),
    local_defines = LOCAL_DEFINES,
    deps = [":common"],
)

cc_library(
    name = "checksum",
    srcs = [
        "folly/external/fast-crc32/sse_crc32c_v8s3x3.cpp",
        "folly/hash/detail/ChecksumDetail.cpp",
        "folly/hash/detail/Crc32CombineDetail.cpp",
        "folly/hash/detail/Crc32cDetail.cpp",
    ],
    copts = COPTS + select({
        "@platforms//cpu:x86_64": ["-mpclmul"],
        "@platforms//cpu:aarch64": [],
    }),
    local_defines = LOCAL_DEFINES,
    deps = [":common"],
)

cc_library(
    name = "checksum-avx512",
    srcs = [
        "folly/external/fast-crc32/avx512_crc32c_v8s3x4.cpp",
    ],
    copts = COPTS + select({
        "@platforms//cpu:x86_64": ["-mpclmul"],
        "@platforms//cpu:aarch64": [],
    }),
    local_defines = LOCAL_DEFINES,
    deps = [":common"],
)

cc_library(
    name = "base64",
    srcs = [
        "folly/detail/base64_detail/Base64_SSE4_2.cpp",
    ],
    copts = COPTS + select({
        "@platforms//cpu:x86_64": ["-msse4.2"],
        "@platforms//cpu:aarch64": [],
    }),
    local_defines = LOCAL_DEFINES,
    deps = [":common"],
)

cc_library(
    name = "assemble",
    srcs = selects.with_or({
        ("@bazel_template//bazel:linux_x86_64", "@bazel_template//bazel:osx_x86_64"): [
            "folly/memcpy.S",
            #"folly/memset.S",
        ],
        "@bazel_template//bazel:linux_aarch64": [
            "folly/external/aor/memcpy-advsimd.S",
            "folly/external/aor/memcpy-armv8.S",
            "folly/external/aor/memcpy_sve.S",
            "folly/external/aor/memset-advsimd.S",
        ],
        "//conditions:default": [],
    }),
    copts = COPTS + ["-x assembler-with-cpp"],
    local_defines = LOCAL_DEFINES + select({
        "@platforms//cpu:x86_64": [],
        "@platforms//cpu:aarch64": [
            "FOLLY_MEMCPY_IS_MEMCPY",
            "FOLLY_MEMSET_IS_MEMSET",
        ],
    }),
    deps = [":common"],
)

cc_library(
    name = "common",
    srcs = glob(
        ["folly/**/*.cpp"],
        exclude = [
            "folly/external/aor/**",
            "folly/crypto/detail/MathOperation_AVX2.cpp",
            "folly/crypto/detail/MathOperation_SSE2.cpp",
            "folly/crypto/detail/MathOperation_Simple.cpp",
            "folly/detail/base64_detail/Base64_SSE4_2.cpp",
            "folly/external/fast-crc32/sse_crc32c_v8s3x3.cpp",
            "folly/external/fast-crc32/avx512_crc32c_v8s3x4.cpp",
            "folly/hash/detail/ChecksumDetail.cpp",
            "folly/hash/detail/Crc32CombineDetail.cpp",
            "folly/hash/detail/Crc32cDetail.cpp",
            "folly/io/tool/HugePageUtil.cpp",
            "folly/json/tool/JSONSchemaTester.cpp",
            "folly/tool/BenchmarkCompare.cpp",
            "folly/build/**",
            "folly/docs/**",
            "folly/python/**/*.cpp",
            "folly/**/example/**",
            "folly/**/test/**/*.cpp",
            "folly/**/*Test.cpp",
            "folly/Subprocess.cpp",
            "folly/debugging/symbolizer/tool/*.cpp",  #not support windows
            "folly/debugging/exception_tracer/*.cpp",
            "folly/executors/ManualExecutor.cpp",
            "folly/experimental/symbolizer/tool/*.cpp",  #not support windows
        ],
    ) + select({
        "@platforms//os:windows": [],
        "//conditions:default": [
            "folly/Subprocess.cpp",
            "folly/debugging/exception_tracer/ExceptionCounterLib.cpp",
            "folly/debugging/exception_tracer/ExceptionStackTraceLib.cpp",
            "folly/debugging/exception_tracer/ExceptionTracer.cpp",
            "folly/debugging/exception_tracer/ExceptionTracerLib.cpp",
            "folly/debugging/exception_tracer/SmartExceptionStackTraceHooks.cpp",
            "folly/debugging/exception_tracer/SmartExceptionTracer.cpp",
            "folly/debugging/exception_tracer/SmartExceptionTracerSingleton.cpp",
            "folly/debugging/exception_tracer/StackTrace.cpp",
            "folly/executors/ManualExecutor.cpp",
        ],
    }),
    hdrs = [
        "folly/io/async/test/ScopedBoundPort.h",
        "folly/io/async/test/SocketPair.h",
        "folly/io/async/test/TimeUtil.h",
        "folly/synchronization/test/Semaphore.h",
        "folly/test/DeterministicSchedule.h",
        ":folly-config_h",
    ] + glob(
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
        "@platforms//cpu:x86_64": [],
        "@platforms//cpu:aarch64": ["folly/external/aor/asmdefs.h"],
    }),
    copts = COPTS,
    defines = select({
        "@bazel_template//bazel:cross_compiling_for_linux_aarch64_musl": [
            "FOLLY_STATIC_LIBSTDCXX=1",
        ],
        "//conditions:default": [],
    }),
    linkopts = GLOBAL_LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        "@boost//:algorithm",
        "@boost//:bind",
        "@boost//:context",
        "@boost//:conversion",
        "@boost//:core",
        "@boost//:crc",
        "@boost//:filesystem",
        "@boost//:mpl",
        "@boost//:multi_index",
        "@boost//:preprocessor",
        "@boost//:program_options",
        "@boost//:thread",
        "@boost//:utility",
        "@com_github_gflags_gflags//:gflags",
        "@com_github_glog_glog//:glog",
        "@com_github_google_snappy//:snappy",
        "@com_google_googletest//:gtest",
        "@double-conversion//:double-conversion",
        "@fmt",
        "@libdwarf//:dwarf",
        "@libevent//:event",
        "@libevent//:event_openssl",
        "@libsodium//:sodium",
        "@openssl",
        "@zstd",
    ] + select({
        "@bazel_template//bazel:jemalloc": ["@jemalloc"],
        "@bazel_template//bazel:tcmalloc": ["@tcmalloc//tcmalloc"],
        "//conditions:default": [],
    }) + select({
        "@platforms//os:windows": [],
        "@platforms//os:osx": [
            "@libiberty//:iberty",
        ],
        "@platforms//os:linux": [
            "@libaio//:aio",
            "@libiberty//:iberty",
            "@libunwind//:unwind",
            "@liburing//:liburing-ffi",
        ],
        "//conditions:default": [],
    }),
)

cc_library(
    name = "folly",
    deps = [
        ":MathOperation",
        ":MathOperation_AVX2",
        ":assemble",
        ":base64",
        ":checksum",
        ":checksum-avx512",
        ":common",
    ],
)

genrule(
    name = "folly-config_h_in",
    outs = ["folly/folly-config.h.in"],
    cmd = "\n".join([
        "cat <<'EOF' >$@",
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
        "#define FOLLY_HAVE_LIBGFLAGS 1",
        "#define FOLLY_GFLAGS_NAMESPACE gflags",
        "#define FOLLY_HAVE_LIBGLOG 1",
        "#define FOLLY_USE_JEMALLOC 0",
        "#if __has_include(<features.h>)",
        "#include <features.h>",
        "#endif",
        "#define FOLLY_HAVE_ACCEPT4 1",
        "#define FOLLY_HAVE_GETRANDOM 1",
        "#define FOLLY_HAVE_PREADV 1",
        "#define FOLLY_HAVE_PWRITEV 1",
        "#define FOLLY_HAVE_CLOCK_GETTIME 1",
        "#define FOLLY_HAVE_PIPE2 1",
        "#define FOLLY_HAVE_IFUNC 1",
        "#define FOLLY_HAVE_UNALIGNED_ACCESS 1",
        "#define FOLLY_HAVE_VLA 1",
        "#define FOLLY_HAVE_WEAK_SYMBOLS 1",
        "#define FOLLY_HAVE_LINUX_VDSO 1",
        "#define FOLLY_HAVE_MALLOC_USABLE_SIZE 1",
        "/* #undef FOLLY_HAVE_INT128_T */",
        "#define FOLLY_HAVE_WCHAR_SUPPORT 1",
        "#if !defined(__clang__)",
        "#define FOLLY_HAVE_EXTRANDOM_SFMT19937 1",
        "#endif",
        "#define HAVE_VSNPRINTF_ERRORS 1",
        "#define FOLLY_HAVE_LIBUNWIND 1",
        "#define FOLLY_HAVE_DWARF 1",
        "#define FOLLY_HAVE_ELF 1",
        "#define FOLLY_HAVE_SWAPCONTEXT 1",
        "#define FOLLY_HAVE_BACKTRACE 1",
        "#define FOLLY_USE_SYMBOLIZER 1",
        "#define FOLLY_DEMANGLE_MAX_SYMBOL_SIZE 1024",
        "#define FOLLY_HAVE_SHADOW_LOCAL_WARNINGS 1",
        "#define FOLLY_HAVE_LIBLZ4 1",
        "#define FOLLY_HAVE_LIBLZMA 1",
        "#define FOLLY_HAVE_LIBSNAPPY 1",
        "#define FOLLY_HAVE_LIBZ 1",
        "#define FOLLY_HAVE_LIBZSTD 1",
        "#define FOLLY_HAVE_LIBBZ2 1",
        "#define FOLLY_LIBRARY_SANITIZE_ADDRESS 0",
        "/* #undef FOLLY_SUPPORT_SHARED_LIBRARY */",
        "#define FOLLY_HAVE_LIBRT 0",
        "#define FOLLY_ELF_NATIVE_CLASS 64",
        "EOF",
    ]),
)

template_rule(
    name = "folly-config_h",
    src = ":folly-config_h_in",
    out = "folly/folly-config.h",
    substitutions = select({
        "@bazel_template//bazel:jemalloc": {"#define FOLLY_USE_JEMALLOC 0": "#define FOLLY_USE_JEMALLOC 1"},
        "@bazel_template//bazel:tcmalloc": {"#define FOLLY_USE_JEMALLOC 0": ""},
        "//conditions:default": {"#define FOLLY_USE_JEMALLOC 0": ""},
    }) | select({
        "@bazel_template//bazel:linux_aarch64": {
            "#define FOLLY_HAVE_SWAPCONTEXT 1": "",
            "#define FOLLY_HAVE_BACKTRACE 1": "",
        },
        "//conditions:default": {},
    }) | select({
        "@platforms//os:linux": {},
        "@platforms//os:osx": {
            "#define FOLLY_HAVE_ACCEPT4 1": "/* #undef FOLLY_HAVE_ACCEPT4 */",
            "#define FOLLY_HAVE_GETRANDOM 1": "#define FOLLY_HAVE_GETRANDOM 0",
            "#define FOLLY_HAVE_PIPE2 1": "/* #undef FOLLY_HAVE_PIPE2 */",
            "#define FOLLY_HAVE_IFUNC 1": "/* #undef FOLLY_HAVE_IFUNC */",
            "#define FOLLY_HAVE_WEAK_SYMBOLS 1": "#define FOLLY_HAVE_WEAK_SYMBOLS 0",
            "#define FOLLY_HAVE_LINUX_VDSO 1": "/* #undef FOLLY_HAVE_LINUX_VDSO */",
            "#define FOLLY_HAVE_MALLOC_USABLE_SIZE 1": "/* #undef FOLLY_HAVE_MALLOC_USABLE_SIZE */",
            "#define HAVE_VSNPRINTF_ERRORS 1": "/* #undef HAVE_VSNPRINTF_ERRORS */",
            "#define FOLLY_HAVE_LIBUNWIND 1": "/* #undef FOLLY_HAVE_LIBUNWIND */",
            "#define FOLLY_HAVE_ELF 1": "/* #undef FOLLY_HAVE_ELF */",
            "#define FOLLY_USE_SYMBOLIZER 1": "/* #undef FOLLY_USE_SYMBOLIZER */",
            "#define FOLLY_HAVE_SHADOW_LOCAL_WARNINGS 1": "/* #undef FOLLY_HAVE_SHADOW_LOCAL_WARNINGS */",
            "#define FOLLY_ELF_NATIVE_CLASS 64": "",
        },
        "@platforms//os:windows": {
            "#define FOLLY_HAVE_ACCEPT4 1": "/* #undef FOLLY_HAVE_ACCEPT4 */",
            "#define FOLLY_HAVE_GETRANDOM 1": "#define FOLLY_HAVE_GETRANDOM 0",
            "#define FOLLY_HAVE_PIPE2 1": "/* #undef FOLLY_HAVE_PIPE2 */",
            "#define FOLLY_HAVE_IFUNC 1": "/* #undef FOLLY_HAVE_IFUNC */",
            "#define FOLLY_HAVE_WEAK_SYMBOLS 1": "#define FOLLY_HAVE_WEAK_SYMBOLS 0",
            "#define FOLLY_HAVE_LINUX_VDSO 1": "/* #undef FOLLY_HAVE_LINUX_VDSO */",
            "#define FOLLY_HAVE_MALLOC_USABLE_SIZE 1": "/* #undef FOLLY_HAVE_MALLOC_USABLE_SIZE */",
            "#define HAVE_VSNPRINTF_ERRORS 1": "/* #undef HAVE_VSNPRINTF_ERRORS */",
            "#define FOLLY_HAVE_LIBUNWIND 1": "/* #undef FOLLY_HAVE_LIBUNWIND */",
            "#define FOLLY_HAVE_ELF 1": "/* #undef FOLLY_HAVE_ELF */",
            "#define FOLLY_USE_SYMBOLIZER 1": "/* #undef FOLLY_USE_SYMBOLIZER */",
            "#define FOLLY_HAVE_SHADOW_LOCAL_WARNINGS 1": "/* #undef FOLLY_HAVE_SHADOW_LOCAL_WARNINGS */",
            "#define FOLLY_ELF_NATIVE_CLASS 64": "",
            "#define FOLLY_HAVE_PREADV 1": "/* #undef FOLLY_HAVE_PREADV */",
            "#define FOLLY_HAVE_PWRITEV 1": "/* #undef FOLLY_HAVE_PWRITEV */",
            "#define FOLLY_HAVE_DWARF 1": "/* #undef FOLLY_HAVE_DWARF */",
            "#define FOLLY_HAVE_SWAPCONTEXT 1": "/* #undef FOLLY_HAVE_SWAPCONTEXT */",
            "#define FOLLY_HAVE_BACKTRACE 1": "/* #undef FOLLY_HAVE_BACKTRACE */",
            "#define FOLLY_HAVE_PTHREAD 1": "/* #undef FOLLY_HAVE_PTHREAD */",
            "#define FOLLY_HAVE_PTHREAD_ATFORK 1": "/* #undef FOLLY_HAVE_PTHREAD_ATFORK */",
            "#define FOLLY_HAVE_CLOCK_GETTIME 1": "/* #undef FOLLY_HAVE_CLOCK_GETTIME */",
            "#define FOLLY_HAVE_VLA 1": "/* #undef FOLLY_HAVE_VLA */",
            "#define FOLLY_HAVE_EXTRANDOM_SFMT19937 1": "/* #undef FOLLY_HAVE_EXTRANDOM_SFMT19937 */",
        },
    }) | select({
        "@bazel_template//bazel:cross_compiling_for_linux_aarch64_musl": {
        },
        "//conditions:default": {},
    }),
)
