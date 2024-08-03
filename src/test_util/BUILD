load("//bazel:build.bzl", "cc_test")
load("//bazel:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "test_main",
    srcs = ["test_main.cc"],
    copts = [
        "-isystem external/double-conversion",
        "-isystem external/folly",
        "-isystem $(GENDIR)/external/folly",
    ],
    linkopts = ["-lunwind"],
    deps = [
        "@com_github_gflags_gflags//:gflags",
        "@com_github_glog_glog//:glog",
        "@com_google_googletest//:gtest",
        "@folly",
    ],
)

cpplint()
