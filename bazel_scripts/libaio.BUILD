package(default_visibility = ["//visibility:public"])

config_setting(
    name = "linux_x86_64",
    constraint_values = [
        "@platforms//os:linux",
        "@platforms//cpu:x86_64",
    ],
)

config_setting(
    name = "linux_aarch64",
    constraint_values = [
        "@platforms//os:linux",
        "@platforms//cpu:aarch64",
    ],
)

platform(
    name = "linux_aarch64_platform",
    constraint_values = [
        "@platforms//os:linux",
        "@platforms//cpu:aarch64",
    ],
)

filegroup(
    name = "libaio.map",
    srcs = ["src/libaio.map"],
)

cc_library(
    name = "aio",
    srcs = glob([
        "src/*.c",
    ]),
    hdrs = [
        "src/aio_ring.h",
        "src/libaio.h",
        "src/syscall.h",
        "src/syscall-generic.h",
        "src/vsys_def.h",
    ] + select({
        ":linux_x86_64": [
            "src/syscall-x86_64.h",
        ],
        ":linux_aarch64": [
            "src/syscall-arm.h",
        ],
    }),
    additional_linker_inputs = [
        ":libaio.map",
    ],
    copts = [
        "-g",
        "-O3",
        "-fomit-frame-pointer",
    ],
    includes = ["src"],
    linkopts = [
        "-Wl,--version-script=$(location :libaio.map)",
        "-Wl,-soname=libaio.so.1",
    ],
    visibility = ["//visibility:public"],
)
