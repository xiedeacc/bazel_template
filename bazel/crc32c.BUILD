load("@bazel_skylib//lib:selects.bzl", "selects")
load("@rules_cc//cc:defs.bzl", "cc_library")
load("@bazel_template//bazel:common.bzl", "GLOBAL_COPTS", "GLOBAL_DEFINES", "GLOBAL_LINKOPTS", "GLOBAL_LOCAL_DEFINES", "template_rule")

package(default_visibility = ["//visibility:public"])

COPTS = GLOBAL_COPTS + select({
    "@platforms//os:windows": [
        "/std:c++20",
    ],
    "//conditions:default": [
        "-std=c++20",
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
    "CJSON_EXPORT_SYMBOLS",
    "CJSON_IMPORT_SYMBOLS",
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

DEFINES = GLOBAL_DEFINES

crc32c_arm64_srcs = [
    "src/crc32c_arm64.cc",
]

crc32c_sse42_srcs = [
    "src/crc32c_sse42.cc",
]

crc32c_hdrs = [
    "src/crc32c_arm64.h",
    "src/crc32c_arm64_check.h",
    "src/crc32c_internal.h",
    "src/crc32c_prefetch.h",
    "src/crc32c_read_le.h",
    "src/crc32c_round_up.h",
    "src/crc32c_sse42.h",
    "src/crc32c_sse42_check.h",
    "include/crc32c/crc32c.h",
]

crc32c_srcs = [
    "src/crc32c_portable.cc",
    "src/crc32c.cc",
]

# select() cannot be use recursively, need to introduce temporary conjunctions.
selects.config_setting_group(
    name = "linux_sse42",
    match_all = [
        "@platforms//os:linux",
        "@platforms//cpu:x86_64",
    ],
)

selects.config_setting_group(
    name = "macos_sse42",
    match_all = [
        "@platforms//os:macos",
        "@platforms//cpu:x86_64",
    ],
)

selects.config_setting_group(
    name = "windows_sse42",
    match_all = [
        "@platforms//os:windows",
        "@platforms//cpu:x86_64",
    ],
)

crc32c_copts = select({
    ":linux_sse42": ["-msse4.2"],
    ":macos_sse42": ["-msse4.2"],
    ":windows_sse42": ["/arch:AVX"],
    # No special flags are needed for ARM64+CRC32C.
    "@platforms//cpu:aarch64": [
        "-march=armv8-a+crc+crypto",
    ],
    "//conditions:default": [],
})

template_rule(
    name = "generate_config",
    src = "src/crc32c_config.h.in",
    out = "crc32c/crc32c_config.h",
    substitutions = {
        "#cmakedefine01": "#define",
        " BYTE_ORDER_BIG_ENDIAN": " BYTE_ORDER_BIG_ENDIAN 0",
        " HAVE_BUILTIN_PREFETCH": " HAVE_BUILTIN_PREFETCH 0",
        " HAVE_MM_PREFETCH": " HAVE_MM_PREFETCH 0",
        " HAVE_STRONG_GETAUXVAL": " HAVE_STRONG_GETAUXVAL 0",
        " HAVE_WEAK_GETAUXVAL": " HAVE_WEAK_GETAUXVAL 0",
        " CRC32C_TESTS_BUILT_WITH_GLOG": " CRC32C_TESTS_BUILT_WITH_GLOG 0",
    } | selects.with_or({
        #("@bazel_template//bazel:linux_x86_64", "@bazel_template//bazel:osx_x86_64"): {
        "@platforms//cpu:x86_64": {
            " HAVE_SSE42": " HAVE_SSE42 1",
            " HAVE_ARM64_CRC32C": " HAVE_ARM64_CRC32C 0",
            " HAVE_MM_PREFETCH": " HAVE_MM_PREFETCH 1",
        },
        "@platforms//cpu:aarch64": {
            " HAVE_SSE42": " HAVE_SSE42 0",
            " HAVE_ARM64_CRC32C": " HAVE_ARM64_CRC32C 1",
        },
        "//conditions:default": {},
    }) | selects.with_or({
        ("@bazel_template//bazel:clang", "@bazel_template//bazel:gcc"): {
            " HAVE_BUILTIN_PREFETCH": " HAVE_BUILTIN_PREFETCH 1",
        },
        "//conditions:default": {},
    }),
)

cc_library(
    name = "crc32c",
    srcs = crc32c_srcs + crc32c_sse42_srcs + crc32c_arm64_srcs,
    hdrs = crc32c_hdrs + ["crc32c/crc32c_config.h"],
    copts = COPTS + crc32c_copts,
    defines = DEFINES,
    includes = ["include"],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [],
)
