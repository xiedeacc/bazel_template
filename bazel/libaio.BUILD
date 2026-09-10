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

filegroup(
    name = "libaio.map",
    srcs = ["src/libaio.map"],
)

cc_library(
    name = "aio",
    srcs = [
        "src/compat-0_1.c",
        "src/io_cancel.c",
        "src/io_destroy.c",
        "src/io_getevents.c",
        "src/io_pgetevents.c",
        "src/io_queue_init.c",
        "src/io_queue_release.c",
        "src/io_queue_run.c",
        "src/io_queue_wait.c",
        "src/io_setup.c",
        "src/io_submit.c",
        "src/raw_syscall.c",
    ],
    hdrs = [
        "src/aio_ring.h",
        "src/libaio.h",
        "src/syscall.h",
        "src/syscall-generic.h",
        "src/vsys_def.h",
    ] + select({
        "@bazel_template//bazel:linux_x86_64": [
            "src/syscall-x86_64.h",
        ],
        "@bazel_template//bazel:linux_aarch64": [
            "src/syscall-arm.h",
        ],
        "//conditions:default": [],
    }),
    copts = COPTS + [
        "-g",
        "-O3",
        "-fomit-frame-pointer",
    ],
    defines = DEFINES,
    includes = ["src"],
    linkopts = LINKOPTS,
    linkstatic = True,
    local_defines = LOCAL_DEFINES,
)
