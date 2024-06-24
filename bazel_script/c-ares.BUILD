package(default_visibility = ["//visibility:public"])

cc_library(
    name = "c-ares",
    srcs = glob(
        [
            "src/lib/*.c",
            "src/lib/*.h",
        ],
        exclude = [
            "src/lib/.deps/*",
        ],
    ),
    hdrs = glob(["include/*.h"]),
    copts = [
        "-g",
        "-O3",
    ],
    includes = [
        "include",
    ],
    linkopts = [
    ],
    local_defines = [
        "HAVE_CONFIG_H=1",
        "_GNU_SOURCE",
        "_POSIX_C_SOURCE=199309L",
        "_XOPEN_SOURCE=600",
    ],
    visibility = ["//visibility:public"],
    deps = [
    ],
)

cc_library(
    name = "ares_getopt",
    srcs = ["src/tools/ares_getopt.c"],
    hdrs = ["src/tools/ares_getopt.h"],
    copts = [
        "-g",
        "-O3",
    ],
    includes = [
    ],
    linkopts = [
    ],
    local_defines = [
        "HAVE_CONFIG_H=1",
        "_GNU_SOURCE",
        "_POSIX_C_SOURCE=199309L",
        "_XOPEN_SOURCE=600",
    ],
    visibility = ["//visibility:public"],
    deps = [
    ],
)

cc_binary(
    name = "adig",
    srcs = ["src/tools/adig.c"],
    copts = [
        "-g",
        "-O3",
    ],
    includes = [
        "src/lib",
    ],
    linkopts = [
    ],
    local_defines = [
        "HAVE_CONFIG_H=1",
        "_GNU_SOURCE",
        "_POSIX_C_SOURCE=199309L",
        "_XOPEN_SOURCE=600",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":ares_getopt",
        ":c-ares",
    ],
)

cc_binary(
    name = "ahost",
    srcs = ["src/tools/ahost.c"],
    copts = [
        "-g",
        "-O3",
    ],
    includes = [
        "src/lib",
    ],
    linkopts = [
    ],
    local_defines = [
        "HAVE_CONFIG_H=1",
        "_GNU_SOURCE",
        "_POSIX_C_SOURCE=199309L",
        "_XOPEN_SOURCE=600",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":ares_getopt",
        ":c-ares",
    ],
)
