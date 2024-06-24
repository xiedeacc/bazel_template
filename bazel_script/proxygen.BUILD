package(default_visibility = ["//visibility:public"])

cc_library(
    name = "lib",
    srcs = glob(
        [
            "proxygen/external/**/*.cpp",
            "proxygen/lib/**/*.cpp",
        ],
        exclude = [
            "proxygen/lib/dns/**",
            "proxygen/**/test/**",
            "proxygen/test/**",
            "proxygen/lib/**/samples/**/*.cpp",
            "proxygen/_build",
        ],
    ),
    hdrs = glob(
        [
            "proxygen/external/**/*.h",
            "proxygen/lib/**/*.h",
        ],
        exclude = [
            "proxygen/lib/**/samples/**/*.h",
        ],
    ),
    copts = [
        "-isystem .",
        "-std=c++17",
    ],
    includes = [
        "external/double-conversion",
        "external/fb303",
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
    linkstatic = True,
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
        "@fb303",
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
    alwayslink = True,
)
