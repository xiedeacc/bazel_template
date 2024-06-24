package(default_visibility = ["//visibility:public"])

cc_library(
    name = "numa",
    srcs = glob(
        [
            "*.c",
        ],
        exclude = [
            "numademo.c",
        ],
    ),
    hdrs = glob([
        "*.h",
    ]),
    copts = [
        "-g",
        "-O3",
        "-Wall",
    ],
    includes = [
    ],
    linkopts = [
        "-latomic",
    ],
    linkstatic = True,
    local_defines = [
        "HAVE_CONFIG_H",
    ],
    visibility = ["//visibility:public"],
    deps = [
    ],
    alwayslink = True,
)
