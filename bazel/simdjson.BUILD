load("@rules_cc//cc:defs.bzl", "cc_library")
load("@bazel_template//bazel:common.bzl", "GLOBAL_COPTS", "GLOBAL_DEFINES", "GLOBAL_LINKOPTS", "GLOBAL_LOCAL_DEFINES")

package(default_visibility = ["//visibility:public"])

COPTS = GLOBAL_COPTS + select({
    "@platforms//os:linux": [],
    "@platforms//os:osx": [],
    "@platforms//os:windows": [],
    "//conditions:default": [],
})

LOCAL_DEFINES = GLOBAL_LOCAL_DEFINES + select({
    "@platforms//os:linux": [],
    "@platforms//os:osx": [],
    "@platforms//os:windows": [],
    "//conditions:default": [],
})

LINKOPTS = GLOBAL_LINKOPTS + select({
    "@platforms//os:linux": [],
    "@platforms//os:osx": [],
    "@platforms//os:windows": [],
    "//conditions:default": [],
})

DEFINES = GLOBAL_DEFINES

cc_library(
    name = "simdjson",
    srcs = ["singleheader/simdjson.cpp"],
    hdrs = ["singleheader/simdjson.h"],
    copts = COPTS + [
        "-O3",
        "-DNDEBUG",
        "-DSIMDJSON_AVX512_ALLOWED=0",
    ],
    defines = DEFINES,
    includes = ["singleheader"],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
)
