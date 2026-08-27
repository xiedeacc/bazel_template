load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")
load("@bazel_template//bazel:common.bzl", "GLOBAL_COPTS", "GLOBAL_DEFINES", "GLOBAL_LINKOPTS", "GLOBAL_LOCAL_DEFINES")

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

DEFINES = GLOBAL_DEFINES

LOCAL_DEFINES = GLOBAL_LOCAL_DEFINES

LINKOPTS = GLOBAL_LINKOPTS + select({
    "@platforms//os:windows": [],
    "@platforms//os:linux": [],
    "@platforms//os:osx": [],
    "//conditions:default": [],
})

cc_library(
    name = "mbedtls",
    srcs = glob(["library/*.c"]),
    hdrs = glob(
        [
            "include/**/*.h",
            "library/*.h",
        ],
        exclude = ["include/mbedtls/mbedtls_config.h"],
    ),
    copts = COPTS,
    defines = DEFINES,
    includes = [
        "include",
        "library",
    ],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [":mbedtls_config"],
)

cc_library(
    name = "mbedtls_default_config",
    hdrs = ["include/mbedtls/mbedtls_config.h"],
    copts = COPTS,
    defines = DEFINES,
    includes = ["include"],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
)

label_flag(
    name = "mbedtls_config",
    build_setting_default = ":mbedtls_default_config",
)

cc_binary(
    name = "linktest",
    copts = COPTS,
    defines = DEFINES,
    linkopts = LINKOPTS + select({
        "@platforms//os:macos": ["-all_load"],
        "//conditions:default": [],
    }),
    linkshared = True,
    local_defines = LOCAL_DEFINES,
    deps = [":mbedtls"],
)
