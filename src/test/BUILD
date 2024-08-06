load("//bazel:build.bzl", "cc_test")
load("//bazel:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

cpplint()

cc_binary(
    name = "test",
    srcs = ["test.cc"],
    copts = [
        "-isystem external/double-conversion",
        "-isystem external/folly",
        "-isystem $(GENDIR)/external/folly",
    ],
    data = ["//conf:base_config.json"],
    deps = [
        "//src/util:config_manager",
        "@com_github_gflags_gflags//:gflags",
        "@com_github_glog_glog//:glog",
        "@folly",
    ],
)

cc_library(
    name = "test_main",
    srcs = ["test_main.cc"],
    copts = [
        "-isystem external/double-conversion",
        "-isystem external/folly",
        "-isystem $(GENDIR)/external/folly",
    ],
    deps = [
        "@com_github_gflags_gflags//:gflags",
        "@com_github_glog_glog//:glog",
        "@com_google_googletest//:gtest",
        "@folly",
        #"@tcmalloc//tcmalloc",
    ],
)
