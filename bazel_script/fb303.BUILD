package(default_visibility = ["//visibility:public"])

cc_library(
    name = "fb303",
    srcs = glob(
        [
            "fb303/*.cpp",
            "fb303/detail/*.cpp",
            "fb303/thrift/gen-cpp2/*.cpp",
        ],
        exclude = [
            "fb303/test/**",
        ],
    ),
    hdrs = glob(
        [
            "fb303/*.h",
            "fb303/detail/*.h",
            "fb303/thrift/gen-cpp2/*.h",
            "fb303/thrift/gen-cpp2/*.tcc",
        ],
        exclude = [
        ],
    ),
    copts = [
        "-isystem .",
        "-std=c++17",
    ],
    includes = [
        "external/double-conversion",
        "external/fbthrift",
        "external/fizz",
        "external/folly",
        "external/mvfst",
        "external/wangle",
    ],
    linkopts = [
        "-pthread",
        "-ldl",
        "-lstdc++fs",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "@c-ares",
        "@com_github_facebook_zstd//:libzstd",
        "@com_github_fmtlib_fmt//:fmt",
        "@com_github_gflags_gflags//:gflags",
        "@com_github_glog_glog//:glog",
        "@com_github_google_snappy//:snappy",
        "@com_github_libevent_libevent//:libevent",
        "@double-conversion//:double-conversion",
        "@fbthrift//:compilerlib",
        "@fizz",
        "@folly",
        "@jemalloc",
        "@libunwind//:unwind",
        "@lzma",
        "@mvfst",
        "@openssl//:ssl",
        "@org_bzip_bzip2//:libbzip2",
        "@wangle",
        "@zlib",
    ],
)
