load("@bazel_skylib//lib:selects.bzl", "selects")

package(default_visibility = ["//visibility:public"])

alias(
    name = "zlib",
    actual = selects.with_or({
        ("@platforms//os:android", "@platforms//os:osx", "@platforms//os:ios", "@platforms//os:watchos", "@platforms//os:tvos"): ":zlib_system",
        "//conditions:default": ":z",
    }),
    visibility = ["//visibility:public"],
)

cc_library(
    name = "z",
    srcs = [
        "adler32.c",
        "compress.c",
        "crc32.c",
        "crc32.h",
        "deflate.c",
        "deflate.h",
        "gzclose.c",
        "gzguts.h",
        "gzlib.c",
        "gzread.c",
        "gzwrite.c",
        "infback.c",
        "inffast.c",
        "inffast.h",
        "inffixed.h",
        "inflate.c",
        "inflate.h",
        "inftrees.c",
        "inftrees.h",
        "trees.c",
        "trees.h",
        "uncompr.c",
        "zutil.c",
        "zutil.h",
    ],
    hdrs = [
        "zconf.h",
        "zlib.h",
    ],
    copts = select({
        "@platforms//os:windows": [],
        "//conditions:default": [
            "-O3",
            "-g",
        ],
    }),
    includes = ["."],
    local_defines = select({
        "@platforms//os:windows": [],
        "//conditions:default": [
            "_LARGEFILE64_SOURCE=1",
            "HAVE_HIDDEN",
        ],
    }),
)

# For OSs that bundle libz
cc_library(
    name = "zlib_system",
    linkopts = ["-lz"],
)
