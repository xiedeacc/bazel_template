load("@bazel_skylib//lib:selects.bzl", "selects")
load("@bazel_template//bazel:common.bzl", "GLOBAL_COPTS", "GLOBAL_LINKOPTS", "GLOBAL_LOCAL_DEFINES", "template_rule")

package(default_visibility = ["//visibility:public"])

COPTS = GLOBAL_COPTS + select({
    "@platforms//os:windows": ["/Ox"],
    "//conditions:default": ["-O3"],
}) + select({
    "@platforms//os:linux": [],
    "@platforms//os:osx": [],
    "@platforms//os:windows": [],
    "//conditions:default": [],
})

LOCAL_DEFINES = GLOBAL_LOCAL_DEFINES + select({
    "@platforms//os:windows": [
        #"bz2_EXPORTS",
        "BZ_DEBUG=0",
        "BZ_LCCWIN32=1",
    ],
    "//conditions:default": [
        #"bz2_EXPORTS",
        "BZ_DEBUG=0",
        "BZ_UNIX=1",
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

alias(
    name = "bz2lib",
    actual = ":libbzip2",
)

alias(
    name = "libbzip2",
    actual = selects.with_or({
        #("@platforms//os:osx", "@platforms//os:ios", "@platforms//os:watchos", "@platforms//os:tvos"): ":bz2lib_system",
        ("@platforms//os:osx", "@platforms//os:ios", "@platforms//os:watchos", "@platforms//os:tvos"): ":bz2lib_source",
        "//conditions:default": ":bz2lib_source",
    }),
)

template_rule(
    name = "bz_version_h",
    src = "bz_version.h.in",
    out = "bz_version.h",
    substitutions = {
        "@BZ_VERSION@": "1.1.0",
    },
)

cc_library(
    name = "bz2lib_source",
    srcs = [
        "blocksort.c",
        "bz_version.h",
        "bzlib.c",
        "bzlib_private.h",
        "compress.c",
        "crctable.c",
        "decompress.c",
        "huffman.c",
        "randtable.c",
    ],
    hdrs = ["bzlib.h"],
    copts = COPTS,
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
)

cc_binary(
    name = "bzip2",
    srcs = ["bzip2.c"],
    copts = COPTS,
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [":libbzip2"],
)

cc_binary(
    name = "bzip2recover",
    srcs = ["bzip2recover.c"],
    copts = COPTS,
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [":libbzip2"],
)

cc_library(
    name = "bz2lib_system",
    linkopts = ["-lbz2"],
)
