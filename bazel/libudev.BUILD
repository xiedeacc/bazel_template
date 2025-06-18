package(default_visibility = ["//visibility:public"])

# TODO (xiedeacc) complete it
cc_library(
    name = "udev",
    srcs = glob(
        ["src/libudev/*.c"],
        exclude = ["src/libudev/test-udev-device-thread.c"],
    ) + [
        "src/basic/alloc-util.c",
    ],
    hdrs = glob(["src/libudev/*.h"]) + [
        "src/basic/alloc-util.h",
        "src/basic/constants.h",
        "src/basic/log.h",
        "src/basic/macro.h",
        "src/fundamental/macro-fundamental.h",
        "src/shared/device-nodes.h",
        "src/systemd/_sd-common.h",
        "src/systemd/sd-hwdb.h",
    ],
    copts = [
        "-g",
        "-O3",
        "-Iexternal/libudev/src/fundamental",
        "-Iexternal/libudev/src/systemd",
        "-Iexternal/libudev/src/shared",
        "-Iexternal/libudev/src/basic",
        "-Iexternal/libudev/src/libudev",
    ],
    includes = ["src"],
)
