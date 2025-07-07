load("@bazel_skylib//lib:selects.bzl", "selects")
load("@bazel_skylib//rules:write_file.bzl", "write_file")
load("@bazel_template//bazel:common.bzl", "GLOBAL_COPTS", "GLOBAL_DEFINES", "GLOBAL_LINKOPTS", "GLOBAL_LOCAL_DEFINES", "template_rule")

package(default_visibility = ["//visibility:public"])

COPTS = GLOBAL_COPTS + select({
    "@platforms//os:windows": [
        "/std:c11",
    ],
    "//conditions:default": [
        "-std=c11",
    ],
}) + select({
    "@platforms//os:linux": [
        "-fPIC",
    ],
    "@platforms//os:osx": [
        "-fPIC",
    ],
    "@platforms//os:windows": [],
    "//conditions:default": [],
})

LOCAL_DEFINES = GLOBAL_LOCAL_DEFINES + [
    "CBOR_API_VISIBILITY",
    "CBOR_PARSER_MAX_RECURSIONS=1024",
] + select({
    "@platforms//os:windows": [
        "_CRT_SECURE_NO_WARNINGS",
    ],
    "//conditions:default": [],
})

LINKOPTS = GLOBAL_LINKOPTS + select({
    "@platforms//os:windows": [],
    "@platforms//os:linux": [],
    "@platforms//os:osx": [],
    "//conditions:default": [],
})

write_file(
    name = "configuration_h",
    out = "cbor/configuration.h",
    content = [
        "#ifndef LIBCBOR_CONFIGURATION_H",
        "#define LIBCBOR_CONFIGURATION_H",
        "#define CBOR_MAJOR_VERSION 0",
        "#define CBOR_MINOR_VERSION 12",
        "#define CBOR_PATCH_VERSION 0",
        "#define CBOR_BUFFER_GROWTH 2",
        "#define CBOR_MAX_STACK_SIZE 2048",
        "#define CBOR_PRETTY_PRINTER 1",
        "#define CBOR_RESTRICT_SPECIFIER restrict",
        "#define CBOR_INLINE_SPECIFIER",
        "#endif //LIBCBOR_CONFIGURATION_H",
    ],
)

write_file(
    name = "cbor_export_h",
    out = "cbor/cbor_export.h",
    content = [
        "#ifndef CBOR_EXPORT_H",
        "#define CBOR_EXPORT_H",
        "",
        "#ifdef CBOR_STATIC_DEFINE",
        "#  define CBOR_EXPORT",
        "#  define CBOR_NO_EXPORT",
        "#else",
        "#  ifndef CBOR_EXPORT",
        "#    ifdef cbor_EXPORTS",
        "        /* We are building this library */",
        "#      define CBOR_EXPORT",
        "#    else",
        "        /* We are using this library */",
        "#      define CBOR_EXPORT",
        "#    endif",
        "#  endif",
        "",
        "#  ifndef CBOR_NO_EXPORT",
        "#    define CBOR_NO_EXPORT",
        "#  endif",
        "#endif",
        "",
        "#ifndef CBOR_DEPRECATED",
        "#  define CBOR_DEPRECATED __attribute__ ((__deprecated__))",
        "#endif",
        "",
        "#ifndef CBOR_DEPRECATED_EXPORT",
        "#  define CBOR_DEPRECATED_EXPORT CBOR_EXPORT CBOR_DEPRECATED",
        "#endif",
        "",
        "#ifndef CBOR_DEPRECATED_NO_EXPORT",
        "#  define CBOR_DEPRECATED_NO_EXPORT CBOR_NO_EXPORT CBOR_DEPRECATED",
        "#endif",
        "",
        "#if 0 /* DEFINE_NO_DEPRECATED */",
        "#  ifndef CBOR_NO_DEPRECATED",
        "#    define CBOR_NO_DEPRECATED",
        "#  endif",
        "#endif",
        "",
        "#endif /* CBOR_EXPORT_H */",
    ],
)

cc_library(
    name = "libcbor",
    srcs = glob(
        [
            "src/**/*.c",
        ],
        exclude = [
            "src/*_test.c",
            "src/*_tests.c",
        ],
    ),
    hdrs = glob([
        "src/**/*.h",
    ]) + [
        ":cbor_export_h",
        ":configuration_h",
    ],
    copts = COPTS + ["-Iexternal/libcbor/src"],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
)

# Convenience alias
alias(
    name = "libcbor_lib",
    actual = ":libcbor",
    visibility = ["//visibility:public"],
)
