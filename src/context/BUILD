load("//bazel:build.bzl", "cc_test")
load("//bazel:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "server_run_info",
    hdrs = ["server_run_info.h"],
    deps = [
    ],
)

cc_library(
    name = "grpc_context",
    srcs = [
        "grpc_context.cc",
    ],
    hdrs = [
        "grpc_context.h",
    ],
    deps = [
        "//src/proto:cc_service",
        "//src/util",
        "//src/util:config_manager",
        "@com_google_absl//absl/synchronization",
    ],
)

cpplint()
