package(default_visibility = ["//visibility:public"])

cc_library(
    name = "mbedtls",
    srcs = glob(
        [
            "library/*.c",
            "library/*.h",
        ],
        exclude = [
        ],
    ),
    hdrs = glob(
        [
            "include/**/*.h",
        ],
        exclude = [
        ],
    ),
    copts = [
        "-isystem .",
        "-Wall",
        "-Wextra",
        "-Wwrite-strings",
        "-Wformat=2",
        "-Wno-format-nonliteral",
        "-Wvla",
        "-Wlogical-op",
        "-Wshadow",
        "-Wformat-signedness",
        "-Wformat-overflow=2",
        "-Wformat-truncation",
        "-Werror",
        "-Wmissing-declarations",
        "-Wmissing-prototypes",
        "-std=c99",
    ],
    includes = [
        "include",
        "library",
    ],
    linkopts = [
        "-pthread",
    ],
    visibility = ["//visibility:public"],
    deps = [
    ],
)
