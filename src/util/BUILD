load("//bazel:build.bzl", "cc_test")
load("//bazel:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "util",
    srcs = ["util.cc"],
    hdrs = ["util.h"],
    deps = [
        ":net",
        "@boost//:algorithm",
        "@com_github_glog_glog//:glog",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/synchronization",
        "@com_google_absl//absl/time",
        "@com_google_protobuf//:protobuf",
        "@smhasher",
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

cpplint()
