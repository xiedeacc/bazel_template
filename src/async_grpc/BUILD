load("@com_google_protobuf//bazel:cc_proto_library.bzl", "cc_proto_library")
# Copyright 2018 The Cartographer Authors
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

load("@rules_cc//cc:defs.bzl", "cc_library")
load("@rules_proto//proto:defs.bzl", "proto_library")
load("//bazel:build.bzl", "cc_test")

# Top-level proto and C++ targets for Cartographer's gRPC server.

load("@bazel_template//bazel:common.bzl", "GLOBAL_COPTS", "GLOBAL_LOCAL_DEFINES")

licenses(["notice"])  # Apache 2.0

package(
    default_visibility = ["//visibility:public"],
    features = [
        "cpp_modules",
        "layering_check",
    ],
)

COPTS = GLOBAL_COPTS + select({
    "@platforms//os:windows": [],
    "//conditions:default": ["-Wno-sign-compare"],
}) + select({
    "@platforms//os:linux": [],
    "@platforms//os:osx": [],
    "@platforms//os:windows": [],
    "//conditions:default": [],
})

LOCAL_DEFINES = GLOBAL_LOCAL_DEFINES

proto_library(
    name = "protos",
    srcs = glob(
        ["**/*.proto"],
        exclude = ["test_service.proto"],
        allow_empty = True,
    ),
    deps = ["@com_google_protobuf//:empty_proto"],
)

# Service definition used only by server_test.cc. Kept out of ':protos' so it
# does not end up linked into the library.
proto_library(
    name = "test_service_proto",
    testonly = True,
    srcs = ["test_service.proto"],
)

cc_proto_library(
    name = "cc_test_service_proto",
    testonly = True,
    deps = [":test_service_proto"],
)

cc_proto_library(
    name = "cc_protos",
    deps = [":protos"],
)

# TODO(rodrigoq): This special rule name is required by cc_grpc_library. This
# makes :protos look like it was created by
#   cc_grpc_library(proto_only=True, ...)
proto_library(
    name = "_cc_protos_only",
    deps = [
        ":protos",
        "@com_google_protobuf//:empty_proto",
    ],
)

# bazel_template.async_grpc: the server side and the completion queue based async
# client. The headers left here are the ones both the module and the client
# module (client.cppm) need in their global module fragments: the macro-based
# mutex, the traits and the retry helpers.
cc_library(
    name = "async_grpc",
    srcs = [
        "common/time.cc",
        "completion_queue_pool.cc",
        "completion_queue_thread.cc",
        "event_queue_thread.cc",
        "opencensus_span.cc",
        "retry.cc",
        "rpc.cc",
        "server.cc",
        "service.cc",
    ],
    hdrs = [
        "common/blocking_queue.h",
        "common/mutex.h",
        "common/time.h",
        "retry.h",
        "rpc_service_method_traits.h",
        "type_traits.h",
    ],
    copts = COPTS,
    features = ["-layering_check"],
    includes = ["."],
    local_defines = LOCAL_DEFINES,
    module_interfaces = ["async_grpc.cppm"],
    deps = [
        ":cc_protos",
        "//src/common:logging",
        "//src/util:util_fwd",
        "@com_github_grpc_grpc//:grpc++",
        "@com_github_grpc_grpc//:grpc++_codegen_proto",
        "@com_google_protobuf//:protobuf",
    ],
)

# bazel_template.async_grpc.client: the blocking client wrappers, consumed by the
# client binary's translation units. Interface only; retry.h and the traits
# headers stay headers because the server side, which cannot import, uses
# them too.
cc_library(
    name = "client",
    copts = COPTS,
    local_defines = LOCAL_DEFINES,
    module_interfaces = ["client.cppm"],
    deps = [
        ":async_grpc",
        "//src/common:logging",
        "@com_github_grpc_grpc//:grpc++",
        "@com_github_grpc_grpc//:grpc++_codegen_proto",
        "@com_google_protobuf//:protobuf",
    ],
)

cc_test(
    name = "server_test",
    timeout = "moderate",
    srcs = ["server_test.cc"],
    copts = COPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":async_grpc",
        ":cc_test_service_proto",
        ":client",
        "@com_github_grpc_grpc//:grpc++",
        "@com_github_grpc_grpc//:grpc++_codegen_proto",
    ],
)

#cc_library(
#name = "async_grpc_tracing",
#srcs = glob(
#[
#"**/*.cc",
#],
#exclude = [
#"**/*_test.cc",
#],
#),
#hdrs = glob([
#"**/*.h",
#]),
#copts = ["-Wno-sign-compare"],
#includes = ["."],
#defines = ["BUILD_TRACING=1"],
#deps = [
#":cc_protos",
#"@com_github_grpc_grpc//:grpc++",
#"@com_google_glog//:glog",
#"@com_github_census_instrumentation_opencensus_cpp//opencensus/exporters/trace/stackdriver:stackdriver_exporter",
#"@com_github_census_instrumentation_opencensus_cpp//opencensus/trace",
#],
#)

#[cc_test(
#name = src.replace("/", "_").replace(".cc", ""),
#size = "small",
#srcs = [src],
#deps = [
#":async_grpc",
#"@com_google_googletest//:gtest_main",
#],
#) for src in glob(
#["**/*_test.cc"],
#)]
