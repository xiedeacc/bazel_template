load("@rules_cc//cc:defs.bzl", "cc_library")
load("//bazel:build.bzl", "cc_test")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "test_main",
    srcs = ["test_main.cc"],
    copts = [
        "-isystem external/double-conversion",
        "-isystem external/folly",
        "-isystem $(GENDIR)/external/folly",
    ],
    deps = [
        "@gflags",
        "//src/common:logging",
        "@com_google_googletest//:gtest",
        "@folly",
    ],
)
