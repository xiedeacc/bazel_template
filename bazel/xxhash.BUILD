load("@bazel_template//bazel:common.bzl", "GLOBAL_COPTS", "GLOBAL_LOCAL_DEFINES")

package(default_visibility = ["//visibility:public"])

COPTS = GLOBAL_COPTS + select({
    "@platforms//os:windows": [],
    "//conditions:default": [],
}) + select({
    "@platforms//os:linux": [],
    "@platforms//os:osx": [],
    "@platforms//os:windows": [],
    "//conditions:default": [],
})

LOCAL_DEFINES = GLOBAL_LOCAL_DEFINES + select({
    "@platforms//os:windows": [],
    "//conditions:default": [],
})

cc_library(
    name = "xxhash",
    srcs = ["xxhash.c"],
    hdrs = ["xxhash.h"],
    copts = COPTS,
    local_defines = LOCAL_DEFINES,
)
