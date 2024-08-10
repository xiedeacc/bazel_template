load("@bazel_skylib//lib:selects.bzl", "selects")
load("@bazel_template//bazel:common.bzl", "template_rule")

package(default_visibility = ["//visibility:public"])

COPTS = [
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
    "-isystem $(GENDIR)/external/libunwind/include",
    "-Iexternal/libunwind/src",
    "-Iexternal/libunwind/include",
    "-I$(GENDIR)/external/libunwind/include/tdep",
    "-Iexternal/libunwind/include/tdep",
    "-Iexternal/libunwind/src/mi",
    "-Wall",
    "-Wno-deprecated",
    "-Wno-deprecated-declarations",
    "-Wno-sign-compare",
    "-Wno-unused",
    "-Wunused-label",
    "-Wunused-result",
    "-Wuninitialized",
    "-Wshadow-compatible-local",
    "-Wno-noexcept-type",
    "-std=c++17",
    "-fcoroutines",
    "-finput-charset=UTF-8",
    "-fsigned-char",
    "-faligned-new",
    "-fopenmp",
]

LOCAL_DEFINES = [
    "GFLAGS_IS_A_DLL=0",
    "HAVE_CONFIG_H",
    "_GNU_SOURCE",
    "_REENTRANT",
    "_LARGEFILE64_SOURCE",
    "BOOST_ATOMIC_DYN_LINK",
    "BOOST_ATOMIC_NO_LIB",
    "BOOST_CONTEXT_DYN_LINK",
    "BOOST_CONTEXT_NO_LIB",
    "BOOST_FILESYSTEM_DYN_LINK",
    "BOOST_FILESYSTEM_NO_LIB",
    "BOOST_PROGRAM_OPTIONS_DYN_LINK",
    "BOOST_PROGRAM_OPTIONS_NO_LIB",
    "BOOST_REGEX_DYN_LINK",
    "BOOST_REGEX_NO_LIB",
    "BOOST_SYSTEM_DYN_LINK",
    "BOOST_SYSTEM_NO_LIB",
    "BOOST_THREAD_DYN_LINK",
    "BOOST_THREAD_NO_LIB",
] + select({
    #"@bazel_template//bazel:jemalloc": ["USE_JEMALLOC"],
    "@bazel_template//bazel:jemalloc": [],
    "@bazel_template//bazel:tcmalloc": [],
    "//conditions:default": [],
})

cc_library(
    name = "MathOperation_AVX2",
    srcs = [
        "folly/crypto/detail/MathOperation_AVX2.cpp",
    ],
    copts = COPTS + select({
        "@bazel_template//bazel:linux_x86_64": ["-mavx"],
        "@bazel_template//bazel:linux_aarch64": [],
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
        "@bazel_template//bazel:linux_x86_64": ["-mno-avx"],
        "@bazel_template//bazel:linux_aarch64": [],
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
        "@bazel_template//bazel:linux_x86_64": ["-mpclmul"],
        "@bazel_template//bazel:linux_aarch64": [],
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
        "@bazel_template//bazel:linux_x86_64": ["-mpclmul"],
        "@bazel_template//bazel:linux_aarch64": [],
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
        "@bazel_template//bazel:linux_x86_64": ["-msse4.2"],
        "@bazel_template//bazel:linux_aarch64": [],
    }),
    local_defines = LOCAL_DEFINES,
    deps = [":common"],
)

cc_library(
    name = "assemble",
    srcs = select({
        "@bazel_template//bazel:linux_x86_64": [
            "folly/memcpy.S",
            #"folly/memset.S",
        ],
        "@bazel_template//bazel:linux_aarch64": [
            "folly/external/aor/memcpy-advsimd.S",
            "folly/external/aor/memcpy-armv8.S",
            "folly/external/aor/memcpy_sve.S",
            "folly/external/aor/memset-advsimd.S",
        ],
    }),
    copts = COPTS + ["-x assembler-with-cpp"],
    local_defines = LOCAL_DEFINES + select({
        "@bazel_template//bazel:linux_x86_64": [
        ],
        "@bazel_template//bazel:linux_aarch64": [
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
            "folly/experimental/symbolizer/tool/Addr2Line.cpp",
            "folly/io/tool/HugePageUtil.cpp",
            "folly/json/tool/JSONSchemaTester.cpp",
            "folly/tool/BenchmarkCompare.cpp",
            "folly/build/**",
            "folly/docs/**",
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
            "folly/python/**/*.cpp",
            "folly/**/example/**",
            "folly/**/test/**/*.cpp",
            "folly/**/*Test.cpp",
        ],
    ),
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
        "@bazel_template//bazel:linux_x86_64": [
        ],
        "@bazel_template//bazel:linux_aarch64": [
            "folly/external/aor/asmdefs.h",
        ],
    }),
    copts = COPTS,
    linkstatic = 0,
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
        "@boost//:utility",
        "@com_github_gflags_gflags//:gflags",
        "@com_github_glog_glog//:glog",
        "@com_github_google_snappy//:snappy",
        "@com_google_googletest//:gtest",
        "@double-conversion//:double-conversion",
        "@fmt",
        "@libaio//:aio",
        "@libdwarf//:dwarf",
        "@libevent//:event",
        "@libevent//:event_openssl",
        "@libevent//:event_pthreads",
        "@libiberty//:iberty",
        "@libsodium//:sodium",
        "@libunwind//:unwind",
        "@liburing//:liburing-ffi",
        "@openssl//:ssl",
        "@zstd",
    ] + select({
        "@bazel_template//bazel:jemalloc": ["@jemalloc"],
        #"@bazel_template//bazel:tcmalloc": ["@tcmalloc//tcmalloc"],
        "@bazel_template//bazel:tcmalloc": ["@bazel_template//lib:tcmalloc_lib"],
        "//conditions:default": [],
    }),
    alwayslink = True,
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
        "#define FOLLY_USE_JEMALLOC 1",
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
        "#define FOLLY_HAVE_EXTRANDOM_SFMT19937 1",
        "#define HAVE_VSNPRINTF_ERRORS 1",
        "#define FOLLY_HAVE_LIBUNWIND 1",
        "#define FOLLY_HAVE_DWARF 1",
        "#define FOLLY_HAVE_ELF 1",
        "#ifdef __GLIBC__",
        "#define FOLLY_HAVE_SWAPCONTEXT 1",
        "#define FOLLY_HAVE_BACKTRACE 1",
        "#endif",
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
    substitutions = selects.with_or({
        "@bazel_template//bazel:gcc": {
        },
        "@bazel_template//bazel:clang": {
            "#define FOLLY_HAVE_EXTRANDOM_SFMT19937 1": "#define FOLLY_HAVE_EXTRANDOM_SFMT19937 0",
        },
    }) | select({
        "@bazel_template//bazel:jemalloc": {"#define FOLLY_USE_JEMALLOC 1": "#define FOLLY_USE_JEMALLOC 1"},
        "@bazel_template//bazel:tcmalloc": {"#define FOLLY_USE_JEMALLOC 1": ""},
        "//conditions:default": {"#define FOLLY_USE_JEMALLOC 1": ""},
    }) | select({
        "@bazel_template//bazel:linux_x86_64": {},
        "@bazel_template//bazel:linux_aarch64": {},
    }),
)
