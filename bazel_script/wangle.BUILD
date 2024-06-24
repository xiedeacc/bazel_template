package(default_visibility = ["//visibility:public"])

cc_library(
    name = "wangle",
    srcs = glob(
        [
            "wangle/**/*.cpp",
        ],
        exclude = [
            "wangle/example/rpc/*.cpp",
            "wangle/test/*.cpp",
            "wangle/**/test/*.cpp",
        ],
    ),
    hdrs = glob(
        [
            "wangle/**/*.h",
        ],
        exclude = [
            "wangle/example/rpc/*.h",
            "wangle/test/*.h",
            "wangle/**/test/*.h",
        ],
    ),
    copts = [
        "-std=c++17",
        "-I.",
    ],
    includes = [
        "external/double-conversion",
        "external/fizz",
        "external/folly",
        "external/zlib",
    ],
    linkopts = [
        "-pthread",
        "-ldl",
        "-lstdc++fs",
    ],
    linkstatic = True,
    visibility = ["//visibility:public"],
    deps = [
        "@fizz",
        "@folly",
    ],
    alwayslink = True,
)
