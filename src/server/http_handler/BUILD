load("@bazel_template//bazel:common.bzl", "GLOBAL_COPTS")
load("//bazel:build.bzl", "cc_test")
load("//bazel:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

COPTS = GLOBAL_COPTS + select({
    "@bazel_template//bazel:not_cross_compiling_on_windows": [
        "/Iexternal/double-conversion",
        "/Iexternal/folly",
        "/I$(GENDIR)/external/folly",
        "/Iexternal/proxygen",
        "/I$(GENDIR)/external/proxygen",
        "/Iexternal/wangle",
    ],
    "//conditions:default": [
        "-isystem external/double-conversion",
        "-isystem external/folly",
        "-isystem $(GENDIR)/external/folly",
        "-isystem external/proxygen",
        "-isystem $(GENDIR)/external/proxygen",
        "-isystem external/wangle",
    ],
}) + select({
    "@platforms//os:linux": [],
    "@platforms//os:osx": [],
    "@platforms//os:windows": [],
    "//conditions:default": [],
})

cc_library(
    name = "http_handler",
    srcs = [
        "http_handler.cc",
    ],
    hdrs = [
        "http_handler.h",
    ],
    copts = COPTS,
    deps = [
        "//src/util",
        "//src/util:config_manager",
        "@proxygen",
    ],
)

cpplint()
