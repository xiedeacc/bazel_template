load("//bazel:build.bzl", "cc_test")
load("//bazel:common.bzl", "version_info")
load("//bazel:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "grpc_server",
    srcs = ["grpc_server.cc"],
    deps = [
        ":grpc_server_impl",
        #"@jemalloc",
        ":version_info",
    ],
)

cc_library(
    name = "grpc_server_impl",
    srcs = ["grpc_server_impl.cc"],
    hdrs = ["grpc_server_impl.h"],
    deps = [
        ":grpc_server_impl_base",
        "//src/common:blocking_list",
        "//src/common:garbage_collector",
        "//src/context:grpc_context",
        "//src/context:server_run_info",
        "//src/grpc_server_impl/call_tags",
        "//src/util",
        "//src/util:config_manager",
    ],
)

cc_library(
    name = "grpc_server_impl_base",
    hdrs = [
        "grpc_server_impl_base.h",
    ],
    deps = [
        "//src/common/grpc_framework:grpc_server_impl",
        "@boost//:algorithm",
        "@com_github_gflags_gflags//:gflags",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/time",
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
