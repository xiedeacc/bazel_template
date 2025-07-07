load("@bazel_template//bazel:common.bzl", "GLOBAL_COPTS", "GLOBAL_DEFINES", "GLOBAL_LINKOPTS", "GLOBAL_LOCAL_DEFINES")

package(default_visibility = ["//visibility:public"])

COPTS = GLOBAL_COPTS + select({
    "@platforms//os:windows": [
        "/std:c++11",
    ],
    "//conditions:default": [
        "-std=c++11",
    ],
}) + select({
    "@platforms//os:linux": [
        "-fPIC",
    ],
    "@platforms//os:osx": [
        "-fPIC",
    ],
    "@platforms//os:windows": [],
    "//conditions:default": [],
})

LOCAL_DEFINES = GLOBAL_LOCAL_DEFINES + [
    "TIXML2_EXPORT",
] + select({
    "@platforms//os:windows": [
        "_CRT_SECURE_NO_WARNINGS",
    ],
    "//conditions:default": [],
})

LINKOPTS = GLOBAL_LINKOPTS + select({
    "@platforms//os:windows": [],
    "@platforms//os:linux": [],
    "@platforms//os:osx": [],
    "//conditions:default": [],
})

cc_library(
    name = "tinyxml2",
    srcs = glob([
        "tinyxml2.cpp",
    ]),
    hdrs = glob([
        "tinyxml2.h",
    ]),
    copts = COPTS,
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
)

# Convenience alias
alias(
    name = "tinyxml2_lib",
    actual = ":tinyxml2",
    visibility = ["//visibility:public"],
) 