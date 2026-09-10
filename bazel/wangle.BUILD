load("@rules_cc//cc:defs.bzl", "cc_library")
load("@bazel_template//bazel:common.bzl", "GLOBAL_COPTS", "GLOBAL_DEFINES", "GLOBAL_LINKOPTS", "GLOBAL_LOCAL_DEFINES", "template_rule")

package(default_visibility = ["//visibility:public"])

COPTS = GLOBAL_COPTS + select({
    "@platforms//os:windows": [
        "/Iexternal/wangle",
        "/Iexternal/libsodium/src/libsodium/include",
        "/Iexternal/libsodium/src/libsodium/include/sodium",
        "/I$(GENDIR)/external/libsodium/src/libsodium/include",
        "/Iexternal/double-conversion",
        "/I$(GENDIR)/external/folly",
        "/Iexternal/folly",
        "/I$(GENDIR)/external/fizz",
        "/Iexternal/fizz",
    ],
    "//conditions:default": [
        "-isystem external/wangle",
        "-isystem external/libsodium/src/libsodium/include",
        "-Iexternal/libsodium/src/libsodium/include/sodium",
        "-isystem $(GENDIR)/external/libsodium/src/libsodium/include",
        "-isystem external/double-conversion",
        "-isystem $(GENDIR)/external/folly",
        "-isystem external/folly",
        "-isystem $(GENDIR)/external/fizz",
        "-isystem external/fizz",
    ],
}) + select({
    "@platforms//os:linux": [],
    "@platforms//os:osx": [],
    "@platforms//os:windows": [],
    "//conditions:default": [],
})

LOCAL_DEFINES = GLOBAL_LOCAL_DEFINES + select({
    "@platforms//os:windows": [],
    "//conditions:default": [],
})

DEFINES = GLOBAL_DEFINES

LINKOPTS = GLOBAL_LINKOPTS + select({
    "@platforms//os:windows": [],
    "@platforms//os:linux": [],
    "@platforms//os:osx": [],
    "//conditions:default": [],
})

template_rule(
    name = "wangle_config_h",
    src = "wangle/wangle-config.h.in",
    out = "wangle/wangle-config.h",
    substitutions = {
        "@WANGLE_LOGGING_BACKEND@": "XLOG",
    },
)

cc_library(
    name = "wangle",
    srcs = glob(
        [
            "wangle/**/*.cpp",
        ],
        exclude = [
            "wangle/example/**",
            "wangle/**/test/**",
        ],
    ),
    hdrs = glob(
        [
            "wangle/**/*.h",
        ],
        exclude = [
            "wangle/example/**",
            "wangle/**/test/**",
        ],
    ) + [":wangle_config_h"],
    copts = COPTS,
    defines = DEFINES,
    includes = ["."],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        "@fizz",
        "@folly",
    ],
)
