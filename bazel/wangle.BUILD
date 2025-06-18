load("@bazel_template//bazel:common.bzl", "GLOBAL_COPTS", "GLOBAL_LOCAL_DEFINES")

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
    ),
    copts = COPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        "@fizz",
        "@folly",
    ],
)
