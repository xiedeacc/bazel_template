load("@bazel_skylib//lib:selects.bzl", "selects")
load("@folly//bazel_scripts:common.bzl", "extract_symbols", "template_rule")

package(default_visibility = ["//visibility:public"])

alias(
    name = "bz2lib",
    actual = ":libbzip2",
    visibility = ["//visibility:public"],
)

alias(
    name = "libbzip2",
    actual = selects.with_or({
        ("@platforms//os:osx", "@platforms//os:ios", "@platforms//os:watchos", "@platforms//os:tvos"): ":bz2lib_system",
        "//conditions:default": ":bz2lib_source",
    }),
    visibility = ["//visibility:public"],
)

COPTS = [
    "-O3",
    "-g",
    "-fPIC",
    #"-Wall",
    #"-Wextra",
    #"-Wmissing-prototypes",
    #"-Wstrict-prototypes",
    #"-Wmissing-declarations",
    #"-Wpointer-arith",
    #"-Wdeclaration-after-statement",
    #"-Wformat-security",
    #"-Wwrite-strings",
    #"-Wshadow",
    #"-Winline",
    #"-Wnested-externs",
    #"-Wfloat-equal",
    #"-Wundef",
    #"-Wendif-labels",
    #"-Wempty-body",
    #"-Wcast-align",
    #"-Wclobbered",
    #"-Wvla",
    #"-Wpragmas",
    #"-Wunreachable-code",
    #"-Waddress",
    #"-Wattributes",
    #"-Wdiv-by-zero",
    #"-Wconversion",
    #"-Wformat-nonliteral",
    #"-Wmissing-field-initializers",
    #"-Wmissing-noreturn",
    #"-Wsign-conversion",
    #"-Wunused-macros",
    #"-Wunused-parameter",
    #"-Wredundant-decls",
    #"-Wno-format-nonliteral",
]

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
    hdrs = [
        "bzlib.h",
    ],
    copts = COPTS,
    local_defines = [
        "bz2_EXPORTS",
        "BZ_DEBUG=0",
        "BZ_UNIX=1",
    ],
)

cc_binary(
    name = "bzip2",
    srcs = [
        "bzip2.c",
    ],
    copts = COPTS,
    local_defines = [
        "BZ_UNIX=1",
    ],
    deps = [
        "libbzip2",
    ],
)

cc_binary(
    name = "bzip2recover",
    srcs = [
        "bzip2recover.c",
    ],
    copts = COPTS,
    local_defines = [
        "BZ_UNIX=1",
    ],
    deps = [
        "libbzip2",
    ],
)

cc_library(
    name = "bz2lib_system",
    linkopts = ["-lbz2"],
)
