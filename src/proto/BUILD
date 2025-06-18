load("@bazel_template//bazel:common.bzl", "GLOBAL_COPTS", "GLOBAL_LINKOPTS", "GLOBAL_LOCAL_DEFINES")
load("@com_github_grpc_grpc//bazel:cc_grpc_library.bzl", "cc_grpc_library")
load("@rules_proto//proto:defs.bzl", "proto_library")

package(default_visibility = ["//visibility:public"])

cc_grpc_library(
    name = "cc_grpc_service",
    srcs = [
        ":service_proto",
    ],
    grpc_only = True,
    deps = [
        ":cc_enums",
        ":cc_error",
        ":cc_service",
    ],
)

cc_proto_library(
    name = "cc_service",
    deps = [
        ":service_proto",
    ],
)

cc_proto_library(
    name = "cc_error",
    deps = [
        ":error_proto",
    ],
)

cc_proto_library(
    name = "cc_config",
    deps = [
        ":config_proto",
    ],
)

cc_proto_library(
    name = "cc_enums",
    deps = [
        ":enums_proto",
    ],
)

proto_library(
    name = "service_proto",
    srcs = ["service.proto"],
    deps = [
        ":enums_proto",
        ":error_proto",
    ],
)

proto_library(
    name = "config_proto",
    srcs = ["config.proto"],
    deps = [],
)

proto_library(
    name = "error_proto",
    srcs = ["error.proto"],
    deps = [],
)

proto_library(
    name = "enums_proto",
    srcs = ["enums.proto"],
)

filegroup(
    name = "proto_files",
    srcs = glob(["*.proto"]),
)
