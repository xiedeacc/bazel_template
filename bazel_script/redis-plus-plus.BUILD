load("@rules_foreign_cc//foreign_cc:configure.bzl", "configure_make")
load("@rules_foreign_cc//foreign_cc:defs.bzl", "make")
load("@bazel_skylib//lib:selects.bzl", "selects")

cc_library(
    name = "redis-plus-plus",
    srcs = [
    ] + glob(
        ["src/**/*.cpp"],
        exclude = [],
    ),
    hdrs = [
    ] + glob(
        [
            "src/**/*.h",
            "src/**/*.hpp",
        ],
        exclude = [
            "src/sw/redis++/cxx11/**/*.h",
            "src/sw/redis++/future/boost/**/*.h",
            "src/sw/redis++/no_tls/**/*.h",
        ],
    ),
    copts = [
        "-std=c++17",
    ],
    includes = [
        "src",
        "src/sw/redis++/cxx17",
        "src/sw/redis++/future/std",
        "src/sw/redis++/tls",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "@com_github_libevent_libevent//:libevent",
        "@hiredis",
        "@libuv",
        "@openssl//:ssl",
    ],
)
