package(default_visibility = ["//visibility:public"])

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
    copts = [
        "-g",
        "-O3",
        "-fomit-frame-pointer",
        "-fPIC",
    ],
    includes = ["src"],
    linkstatic = True,
    alwayslink = True,
)
