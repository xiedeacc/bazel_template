load("@bazel_template//bazel:common.bzl", "GLOBAL_COPTS", "GLOBAL_LOCAL_DEFINES")

package(default_visibility = ["//visibility:public"])

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
    copts = GLOBAL_COPTS + select({
        "@bazel_template//bazel:not_cross_compiling_on_windows": [
            "/Ox",
            "/Iexternal/lz4/lib",
        ],
        "//conditions:default": [
            "-O3",
            "-Iexternal/lz4/lib",
        ],
    }),
    linkopts = [],
    local_defines = [
        "NDEBUG",
        "XXH_NAMESPACE=LZ4_",
        "lz4_shared_EXPORTS",
    ],
    textual_hdrs = [
        "lib/xxhash.c",
        "lib/lz4.c",
    ],
    visibility = ["//visibility:public"],
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
    copts = GLOBAL_COPTS + select({
        "@bazel_template//bazel:not_cross_compiling_on_windows": [
            "/Ox",
            "/Iexternal/lz4/lib",
            "/Iexternal/lz4/programs",
        ],
        "//conditions:default": [
            "-O3",
            "-Iexternal/lz4/lib",
            "-Iexternal/lz4/programs",
        ],
    }),
    local_defines = GLOBAL_LOCAL_DEFINES + [
        "XXH_NAMESPACE=LZ4_",
        "ENABLE_LZ4C_LEGACY_OPTIONS",
        "LZ4IO_MULTITHREAD=0",
    ],
    deps = [
        ":lz4",
    ],
)

cc_binary(
    name = "lz4cli",
    srcs = [
        "programs/lz4cli.c",
    ],
    copts = GLOBAL_COPTS + select({
        "@bazel_template//bazel:not_cross_compiling_on_windows": [
            "/Ox",
            "/Iexternal/lz4/lib",
            "/Iexternal/lz4/programs",
        ],
        "//conditions:default": [
            "-O3",
            "-Iexternal/lz4/lib",
            "-Iexternal/lz4/programs",
        ],
    }),
    local_defines = [
        "XXH_NAMESPACE=LZ4_",
        "ENABLE_LZ4C_LEGACY_OPTIONS",
    ],
    deps = [
        ":lz4_util",
    ],
)
