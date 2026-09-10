load("@bazel_skylib//lib:selects.bzl", "selects")
load("@rules_cc//cc:defs.bzl", "cc_library")
load("@bazel_template//bazel:common.bzl", "GLOBAL_COPTS", "GLOBAL_DEFINES", "GLOBAL_LINKOPTS", "GLOBAL_LOCAL_DEFINES")

COPTS = GLOBAL_COPTS + select({
    "@platforms//os:windows": [],
    "//conditions:default": [],
}) + select({
    "@platforms//os:linux": [],
    "@platforms//os:osx": [],
    "@platforms//os:windows": [],
    "//conditions:default": [],
})

DEFINES = GLOBAL_DEFINES

LOCAL_DEFINES = GLOBAL_LOCAL_DEFINES + select({
    "@platforms//os:windows": [],
    "//conditions:default": [],
}) + select({
    "@platforms//os:linux": [],
    "@platforms//os:osx": [],
    "@platforms//os:windows": [],
    "//conditions:default": [],
})

LINKOPTS = GLOBAL_LINKOPTS + select({
    "@platforms//os:windows": [],
    "//conditions:default": [],
}) + select({
    "@platforms//os:linux": [],
    "@platforms//os:osx": [],
    "@platforms//os:windows": [],
    "//conditions:default": [],
})

package(default_visibility = ["//visibility:public"])

selects.config_setting_group(
    name = "linux_aarch64_glibc",
    match_all = [
        "@platforms//os:linux",
        "@platforms//cpu:aarch64",
        "@bazel_template//bazel:libc_glibc",
    ],
)

cc_library(
    name = "mimalloc",
    srcs = [
        "src/static.c",
    ],
    hdrs = glob([
        "include/**/*.h",
        "src/*.h",
    ]),
    copts = COPTS + select({
        ":linux_aarch64_glibc": [],
        "//conditions:default": ["-DMI_MALLOC_OVERRIDE=1"],
    }) + ["-fPIC"],
    defines = DEFINES,
    includes = [
        "include",
        "src",
    ],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    textual_hdrs = glob(
        [
            "src/**/*.c",
        ],
        exclude = [
            "src/static.c",
        ],
    ),
)
