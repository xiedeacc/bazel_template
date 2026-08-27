load("@rules_cc//cc:defs.bzl", "cc_library")
load("@bazel_template//bazel:common.bzl", "GLOBAL_COPTS", "GLOBAL_DEFINES", "GLOBAL_LINKOPTS", "GLOBAL_LOCAL_DEFINES")

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

DEFINES = GLOBAL_DEFINES

LINKOPTS = GLOBAL_LINKOPTS + select({
    "@platforms//os:windows": [],
    "//conditions:default": [],
})

cc_library(
    name = "xxhash",
    srcs = ["xxhash.c"],
    hdrs = ["xxhash.h"],
    copts = COPTS,
    defines = DEFINES,
    includes = ["."],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
)
