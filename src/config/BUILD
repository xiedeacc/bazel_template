load("//tools:cpplint.bzl", "cpplint")
load("//tools:build.bzl", "cc_test")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "abstract_config",
    srcs = [],
    hdrs = ["abstract_config.h"],
    deps = [
        "//src/proto:cc_config",
        "@com_github_glog_glog//:glog",
        "@com_google_googletest//:gtest",
        "@com_google_protobuf//:protobuf",
    ],
)

cc_library(
    name = "config",
    hdrs = [
        "base_config.h",
    ],
    deps = [
        ":abstract_config",
        "//src/util",
        "@com_google_absl//absl/synchronization",
    ],
)

cc_test(
    name = "config_test",
    srcs = ["config_test.cc"],
    data = [
        "//test_data/config_test:base_config.json",
    ],
    deps = [
        ":config",
        "//src/util:config_util",
    ],
)

cc_library(
    name = "config_wrapper",
    hdrs = [
        "config_wrapper.h",
    ],
    deps = [
    ],
)

cc_binary(
    name = "config_wrapper_benchmark",
    testonly = 1,
    srcs = [
        "config_wrapper_benchmark.cc",
    ],
    data = [
        "//test_data/config_test:base_config.json",
    ],
    tags = ["benchmark"],
    visibility = ["//visibility:private"],
    deps = [
        ":config",
        ":config_wrapper",
        "@com_google_benchmark//:benchmark_main",
    ],
)

cpplint()
