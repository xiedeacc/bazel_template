load("@bazel_skylib//lib:selects.bzl", "selects")
load("@bazel_template//bazel:common.bzl", "GLOBAL_COPTS", "GLOBAL_LINKOPTS", "GLOBAL_LOCAL_DEFINES")

package(default_visibility = ["//visibility:public"])

COPTS = GLOBAL_COPTS + select({
    "@platforms//os:windows": [
        "/Ox",
        "/Iexternal/zstd/lib",
        "/Iexternal/lz4/lib",
    ],
    "//conditions:default": [
        "-O3",
        "-Iexternal/zstd/lib",
        "-Iexternal/lz4/lib",
    ],
}) + select({
    "@platforms//os:linux": [],
    "@platforms//os:osx": [],
    "@platforms//os:windows": [],
    "//conditions:default": [],
})

LOCAL_DEFINES = GLOBAL_LOCAL_DEFINES + [
    "ZSTD_LEGACY_SUPPORT=4",
    "ZSTD_MULTITHREAD",
    "XXH_NAMESPACE=ZSTD_",
    "ZSTD_GZCOMPRESS",
    "ZSTD_GZDECOMPRESS",
    "ZSTD_LZ4COMPRESS",
    "ZSTD_LZ4DECOMPRES",
    "ZSTD_LZMACOMPRESS",
    "ZSTD_LZMADECOMPRES",
    "BACKTRACE_ENABLE=0",
    "DEBUGLEVEL=0",
    "ZSTD_MULTITHREAD",
] + select({
    "@platforms//os:windows": [],
    "//conditions:default": [
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
    name = "zstd",
    srcs = glob(
        [
            "lib/common/*.h",
            "lib/common/*.c",
            "lib/compress/*.h",
            "lib/compress/*.c",
            "lib/decompress/*.h",
            "lib/decompress/*.c",
            "lib/deprecated/*.h",
            "lib/deprecated/*.c",
            "lib/dictBuilder/*.h",
            "lib/dictBuilder/*.c",
            "lib/legacy/*.h",
            "lib/legacy/*.c",
        ],
        exclude = [
            "lib/legacy/zstd_v01.c",
            "lib/legacy/zstd_v02.c",
            "lib/legacy/zstd_v03.c",
        ],
    ) + selects.with_or({
        ("@bazel_template//bazel:linux_x86_64", "@bazel_template//bazel:osx_x86_64"): ["lib/decompress/huf_decompress_amd64.S"],
        "//conditions:default": [],
    }),
    hdrs = [
        "lib/zdict.h",
        "lib/zstd.h",
        "lib/zstd_errors.h",
    ],
    copts = COPTS,
    includes = ["lib"],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        "@lz4",
        "@xz//:lzma",
        "@zlib",
    ],
)

cc_library(
    name = "zlib_wrapper",
    srcs = glob([
        "zlibWrapper/*.c",
    ]),
    hdrs = glob([
        "zlibWrapper/*.h",
    ]),
    copts = COPTS,
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [":zstd"],
)

cc_library(
    name = "zstd_util",
    srcs = [
        "programs/benchfn.c",
        "programs/benchzstd.c",
        "programs/datagen.c",
        "programs/dibio.c",
        "programs/fileio.c",
        "programs/fileio_asyncio.c",
        "programs/lorem.c",
        "programs/timefn.c",
        "programs/util.c",
        "programs/zstdcli_trace.c",
    ],
    hdrs = [
        "programs/benchfn.h",
        "programs/benchzstd.h",
        "programs/datagen.h",
        "programs/dibio.h",
        "programs/fileio.h",
        "programs/fileio_asyncio.h",
        "programs/fileio_common.h",
        "programs/fileio_types.h",
        "programs/lorem.h",
        "programs/platform.h",
        "programs/timefn.h",
        "programs/util.h",
        "programs/zstdcli_trace.h",
    ],
    copts = COPTS,
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [":zstd"],
)

cc_binary(
    name = "zstdcli",
    srcs = ["programs/zstdcli.c"],
    copts = COPTS,
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [":zstd_util"],
)
