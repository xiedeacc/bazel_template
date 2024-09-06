load("//bazel:build.bzl", "cc_test")
load("//bazel:common.bzl", "version_info")
load("//bazel:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "http_handler",
    srcs = [
        "http_handler.cc",
    ],
    hdrs = [
        "http_handler.h",
    ],
    copts = [
        "-isystem external/double-conversion",
        "-isystem external/folly",
        "-isystem external/proxygen",
        "-isystem $(GENDIR)/external/proxygen",
        "-isystem external/wangle",
        "-isystem $(GENDIR)/external/folly",
    ],
    deps = [
        "//src/util",
        "//src/util:config_manager",
        "@proxygen",
    ],
)

cpplint()
