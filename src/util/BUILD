load("//bazel:build.bzl", "cc_test")
load("//bazel:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "config_manager",
    srcs = ["config_manager.cc"],
    hdrs = ["config_manager.h"],
    copts = [
        "-isystem external/double-conversion",
        "-isystem external/folly",
        "-isystem $(GENDIR)/external/folly",
    ],
    deps = [
        ":util",
        "//src/proto:cc_config",
        "@folly",
    ],
)

cc_library(
    name = "util",
    srcs = ["util.cc"],
    hdrs = ["util.h"],
    deps = [
        "//src/common:net",
        "@boost//:algorithm",
        "@boost//:beast",
        "@boost//:iostreams",
        "@com_github_glog_glog//:glog",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/synchronization",
        "@com_google_absl//absl/time",
        "@com_google_protobuf//:protobuf",
        "@fmt",
        "@openssl",
        "@smhasher",
    ],
)

cc_test(
    name = "util_test",
    timeout = "short",
    srcs = [
        "util_test.cc",
    ],
    deps = [
        ":util",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "config_manager_test",
    srcs = [
        "config_manager_test.cc",
    ],
    copts = [
        "-isystem external/double-conversion",
        "-isystem external/folly",
        "-isystem $(GENDIR)/external/folly",
    ],
    data = ["//conf:base_config.json"],
    deps = [
        ":config_manager",
    ],
)

cpplint()
