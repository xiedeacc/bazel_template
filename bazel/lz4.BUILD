load("@bazel_template//bazel:common.bzl", "GLOBAL_COPTS", "GLOBAL_LINKOPTS", "GLOBAL_LOCAL_DEFINES")

package(default_visibility = ["//visibility:public"])

COPTS = GLOBAL_COPTS + select({
    "@platforms//os:windows": [
        "/Ox",
        "/Iexternal/lz4/lib",
        "/Iexternal/lz4/programs",
    ],
    "//conditions:default": [
        "-O3",
        "-Iexternal/lz4/lib",
        "-Iexternal/lz4/programs",
    ],
}) + select({
    "@platforms//os:linux": [],
    "@platforms//os:osx": [],
    "@platforms//os:windows": [],
    "//conditions:default": [],
})

LOCAL_DEFINES = GLOBAL_LOCAL_DEFINES + [
    "XXH_NAMESPACE=LZ4_",
    #"lz4_shared_EXPORTS",
] + select({
    "@platforms//os:windows": [
        "ENABLE_LZ4C_LEGACY_OPTIONS",
        "LZ4IO_MULTITHREAD=0",
    ],
    "//conditions:default": [
        "ENABLE_LZ4C_LEGACY_OPTIONS",
    ],
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

cc_library(
    name = "lz4",
    srcs = [
        "lib/lz4.c",
        "lib/lz4file.c",
        "lib/lz4frame.c",
        "lib/lz4hc.c",
        "lib/xxhash.c",
    ],
    hdrs = [
        "lib/lz4.h",
        "lib/lz4file.h",
        "lib/lz4frame.h",
        "lib/lz4frame_static.h",
        "lib/lz4hc.h",
        "lib/xxhash.c",
        "lib/xxhash.h",
    ],
    copts = COPTS,
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    textual_hdrs = [
        "lib/xxhash.c",
        "lib/lz4.c",
    ],
)

cc_library(
    name = "lz4_util",
    srcs = [
        "programs/bench.c",
        "programs/lorem.c",
        "programs/lz4io.c",
        "programs/threadpool.c",
        "programs/timefn.c",
        "programs/util.c",
    ],
    hdrs = [
        "programs/bench.h",
        "programs/lorem.h",
        "programs/lz4conf.h",
        "programs/lz4io.h",
        "programs/platform.h",
        "programs/threadpool.h",
        "programs/timefn.h",
        "programs/util.h",
    ],
    copts = COPTS,
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [":lz4"],
)

cc_binary(
    name = "lz4cli",
    srcs = ["programs/lz4cli.c"],
    copts = COPTS,
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [":lz4_util"],
)
