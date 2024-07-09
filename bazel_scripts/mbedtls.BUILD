package(default_visibility = ["//visibility:public"])

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
    copts = [
        "-Wall",
        "-Wextra",
        "-Wwrite-strings",
        "-Wformat=2",
        "-Wno-format-nonliteral",
        "-Wvla",
        "-Wlogical-op",
        "-Wshadow",
        "-Wformat-signedness",
        "-Wformat-overflow=2",
        "-Wformat-truncation",
        "-Werror",
        "-Wmissing-declarations",
        "-Wmissing-prototypes",
    ],
    includes = [
        "include",
        "library",
    ],
    visibility = ["//visibility:public"],
    deps = [":mbedtls_config"],
)

cc_library(
    name = "mbedtls_default_config",
    hdrs = ["include/mbedtls/mbedtls_config.h"],
    includes = [
        "include",
    ],
)

label_flag(
    name = "mbedtls_config",
    build_setting_default = ":mbedtls_default_config",
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "linktest",
    linkopts = select({
        "@platforms//os:macos": ["-all_load"],
        "//conditions:default": [],
    }),
    linkshared = True,
    visibility = ["//visibility:private"],
    deps = ["mbedtls"],
)
