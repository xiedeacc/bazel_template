load("//bazel:build.bzl", "cc_test")
load("//bazel:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "tag_base",
    hdrs = [
        "tag_base.h",
    ],
    deps = [
        "@boost//:algorithm",
        "@boost//:thread",
        "@com_github_grpc_grpc//:grpc++",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/time",
    ],
)

cc_library(
    name = "client_grpc_reader_writer",
    hdrs = [
        "client_grpc_reader.h",
        "client_grpc_writer.h",
    ],
    deps = [
        ":tag_base",
    ],
)

cc_library(
    name = "server_grpc_reader_writer",
    hdrs = [
        "server_grpc_reader.h",
        "server_grpc_writer.h",
    ],
    deps = [
        ":tag_base",
    ],
)

cc_library(
    name = "grpc_client_impl",
    hdrs = [
        "grpc_client_impl.h",
    ],
    deps = [
        ":tag_base",
    ],
)

cc_library(
    name = "server_impl",
    hdrs = [
        "server_impl.h",
    ],
    defines = [
        "BOOST_THREAD_PROVIDES_FUTURE_CONTINUATION=1",
        "BOOST_THREAD_PROVIDES_EXECUTORS",
        "BOOST_BIND_GLOBAL_PLACEHOLDERS",
        "BOOST_THREAD_USES_MOVE",
    ],
    deps = [
        ":tag_base",
    ],
)

cc_library(
    name = "client_grpc_tag",
    hdrs = [
        "client_grpc_tag.h",
    ],
    deps = [
        ":client_grpc_reader_writer",
        ":grpc_client_impl",
        "//src/util",
    ],
)

cc_library(
    name = "server_grpc_tag",
    hdrs = [
        "server_grpc_tag.h",
    ],
    deps = [
        ":server_impl",
        ":server_grpc_reader_writer",
        "//src/util",
    ],
)

cpplint()
