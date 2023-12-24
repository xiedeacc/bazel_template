load("//tools:cpplint.bzl", "cpplint")
load("//tools:build.bzl", "cc_test")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "singleton",
    hdrs = [
        "barrier.h",
        "singleton.h",
    ],
    deps = [
        "@com_google_absl//absl/synchronization",
    ],
)

cc_test(
    name = "singleton_test",
    timeout = "short",
    srcs = [
        "singleton_test.cc",
    ],
    deps = [
        ":singleton",
    ],
)

cc_library(
    name = "net",
    srcs = [
        "host_spec.cc",
        "ip_address.cc",
    ],
    hdrs = [
        "host_spec.h",
        "ip_address.h",
    ],
    deps = [
        "@com_google_absl//absl/strings",
    ],
)

cc_test(
    name = "ip_address_test",
    timeout = "short",
    srcs = [
        "ip_address_test.cc",
    ],
    deps = [
        ":net",
    ],
)

cc_test(
    name = "host_spec_test",
    timeout = "short",
    srcs = [
        "host_spec_test.cc",
    ],
    deps = [
        ":net",
    ],
)

cc_library(
    name = "util",
    srcs = ["util.cc"],
    hdrs = [
        "util.h",
    ],
    deps = [
        ":net",
        "@boost//:algorithm",
        "@boost//:beast",
        "@boost//:filesystem",
        "@boost//:iostreams",
        "@brpc",
        "@com_github_fmtlib_fmt//:fmt",
        "@com_github_glog_glog//:glog",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/time",
        "@com_google_protobuf//:protobuf",
        "@openssl",
    ],
)

cc_test(
    name = "util_test",
    srcs = ["util_test.cc"],
    data = [
        "//test_data/config_test:base_config.json",
    ],
    deps = [
        ":util",
        "@com_github_glog_glog//:glog",
    ],
)

cc_library(
    name = "config_util",
    srcs = ["config_util.cc"],
    hdrs = [
        "config_util.h",
    ],
    deps = [
        ":singleton",
        ":util",
        "//src/config",
        "//src/config:config_wrapper",
        "@nacos_sdk",
    ],
)

cc_test(
    name = "config_util_test",
    timeout = "short",
    srcs = ["config_util_test.cc"],
    data = [
        "//test_data/config_test:base_config.json",
    ],
    deps = [
        ":config_util",
    ],
)

cc_library(
    name = "time_metric",
    srcs = [
        "time_metric.cc",
    ],
    hdrs = [
        "time_metric.h",
    ],
    deps = [
        "//src/util",
        "@com_github_glog_glog//:glog",
    ],
)

cpplint()
