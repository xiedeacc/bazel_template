load("@bazel_template//bazel:common.bzl", "GLOBAL_COPTS")

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
    includes = [
        "include",
        "library",
    ],
    deps = [":mbedtls_config"],
)

cc_library(
    name = "mbedtls_default_config",
    hdrs = ["include/mbedtls/mbedtls_config.h"],
    includes = ["include"],
)

label_flag(
    name = "mbedtls_config",
    build_setting_default = ":mbedtls_default_config",
)

cc_binary(
    name = "linktest",
    linkopts = select({
        "@platforms//os:macos": ["-all_load"],
        "//conditions:default": [],
    }),
    linkshared = True,
    deps = [":mbedtls"],
)
