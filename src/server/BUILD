load("//bazel:build.bzl", "cc_test")
load("//bazel:common.bzl", "version_info")
load("//bazel:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "main",
    srcs = ["main.cc"],
    copts = [
        "-std=c++17",
    ],
    deps = [
        "@boost//:algorithm",
        "@boost//:beast",
        "@boost//:filesystem",
        "@boost//:iostreams",
        "@brotli",
        "@c-ares",
        "@com_github_gflags_gflags//:gflags",
        "@com_github_glog_glog//:glog",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/synchronization",
        "@com_google_absl//absl/time",
        "@com_google_googletest//:gtest",
        "@com_google_protobuf//:protobuf",
        "@curl",
        #"@jemalloc",
        "@libsodium//:sodium",
        "@openssl",
        "@proxygen",
    ],
)

version_info(
    name = "version_info_h",
    out = "version_info.h",
    exec_tool = "//bazel:gen_local_config_git",
)

cc_library(
    name = "version_info",
    hdrs = [":version_info_h"],
    visibility = ["//visibility:public"],
)

cc_test(
    name = "version_info_test",
    srcs = ["version_info_test.cc"],
    deps = [
        ":version_info",
        "@com_github_glog_glog//:glog",
        "@com_google_googletest//:gtest",
    ],
)

cpplint()
