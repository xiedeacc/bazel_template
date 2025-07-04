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

# Top-level proto and C++ targets for Cartographer's gRPC server.

load("@bazel_template//bazel:common.bzl", "GLOBAL_COPTS", "GLOBAL_LOCAL_DEFINES")

licenses(["notice"])  # Apache 2.0

package(default_visibility = ["//visibility:public"])

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
    srcs = glob(["**/*.proto"]),
    deps = ["@com_google_protobuf//:empty_proto"],
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

cc_library(
    name = "async_grpc",
    srcs = glob(
        [
            "**/*.cc",
        ],
        exclude = [
            "**/*_test.cc",
        ],
    ),
    hdrs = glob(
        [
            "**/*.h",
        ],
        exclude = ["common/make_unique.h"],
    ),
    copts = COPTS,
    includes = ["."],
    local_defines = LOCAL_DEFINES,
    deps = [
        ":cc_protos",
        "@com_github_glog_glog//:glog",
        "@com_github_grpc_grpc//:grpc++",
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
