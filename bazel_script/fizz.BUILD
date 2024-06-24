package(default_visibility = ["//visibility:public"])

cc_library(
    name = "fizz",
    srcs = glob(
        [
            "fizz/**/*.cpp",
            "fizz/**/*.c",
        ],
        exclude = [
            "fizz/build_",
            "fizz/cmake",
            "fizz/test",
            "fizz/third-party/libsodium-aegis/**",
            "fizz/**/test/**/*.cpp",
            "fizz/extensions/javacrypto/**/*.cpp",
        ],
    ),
    hdrs = glob(
        [
            "fizz/**/*.h",
        ],
        exclude = [
        ],
    ),
    copts = [
        "-I.",
        "-std=c++17",
    ],
    includes = [
        "$(BINDIR)/external/liboqs/src",
        "external/double-conversion",
        "external/folly",
        "external/zlib",
    ],
    linkopts = [
        "-pthread",
        "-ldl",
        "-lstdc++fs",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "@boost//:algorithm",
        "@boost//:context",
        "@boost//:crc",
        "@boost//:filesystem",
        "@boost//:multi_index",
        "@boost//:preprocessor",
        "@boost//:program_options",
        "@brotli",
        "@com_github_facebook_zstd//:libzstd",
        "@com_github_fmtlib_fmt//:fmt",
        "@com_github_gflags_gflags//:gflags",
        "@com_github_glog_glog//:glog",
        "@com_github_google_snappy//:snappy",
        "@com_github_libevent_libevent//:libevent",
        "@double-conversion//:double-conversion",
        "@folly",
        "@jemalloc",
        "@libdwarf//:dwarf",
        "@liboqs//:oqs",
        "@libsodium",
        "@libunwind//:unwind",
        "@lzma",
        "@openssl//:ssl",
        "@org_bzip_bzip2//:libbzip2",
        "@zlib",
    ],
)
