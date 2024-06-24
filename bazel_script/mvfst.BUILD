package(default_visibility = ["//visibility:public"])

cc_library(
    name = "mvfst",
    srcs = glob(
        [
            "quic/**/*.cpp",
        ],
        exclude = [
            "quic/samples/**/*.cpp",
            "quic/test/*.cpp",
            "quic/**/test/*.cpp",
            "quic/docs/**/*.cpp",
            "quic/tools/tperf/**/*.cpp",
        ],
    ),
    hdrs = [
    ] + glob(
        [
            "quic/**/*.h",
        ],
        exclude = [
            "quic/samples/**/*.h",
            "quic/test/*.h",
            "quic/**/test/*.h",
            "quic/docs/**/*.h",
            "quic/tools/tperf/**/*.h",
        ],
    ),
    copts = [
        "-I.",
        "-Iexternal/folly",
        "-isystem external/fizz",
        "-std=c++17",
    ],
    includes = [
        "external/double-conversion",
        "external/folly",
        "external/zlib",
    ],
    linkopts = [
    ],
    linkstatic = True,
    visibility = ["//visibility:public"],
    deps = [
        "@fizz",
        "@folly",
    ],
    alwayslink = True,
)
