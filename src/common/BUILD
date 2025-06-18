load("@bazel_template//bazel:common.bzl", "GLOBAL_COPTS", "GLOBAL_LINKOPTS", "GLOBAL_LOCAL_DEFINES")
load("//bazel:build.bzl", "cc_test")
load("//bazel:cpplint.bzl", "cpplint")
load("//bazel:pb_code_gen_plugin.bzl", "cc_proto_plugin")

package(default_visibility = ["//visibility:public"])

cpplint()

COPTS = GLOBAL_COPTS

LOCAL_DEFINES = GLOBAL_LOCAL_DEFINES

LINKOPTS = GLOBAL_LINKOPTS

cc_library(
    name = "defs",
    hdrs = ["defs.h"],
    deps = [
        ":blocking_queue",
        "//src/proto:cc_error",
        "//src/proto:cc_service",
    ],
)

cc_library(
    name = "blocking_queue",
    hdrs = ["blocking_queue.h"],
    copts = COPTS,
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [],
)

cc_test(
    name = "blocking_queue_test",
    srcs = ["blocking_queue_test.cc"],
    copts = COPTS,
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":blocking_queue",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_proto_plugin(
    name = "error_code",
    src = "//src/proto:error.proto",
    outs = ["error.h"],
    data = ["//src/common/template:error.h"],
    plugin = "//src/common/proto_plugin:pb_code_generator",
    proto_deps = [
        "//src/proto:error.proto",
    ],
    proto_paths = [
        ".",
    ],
    protoc = "@com_google_protobuf//:protoc",
    template_dir = "src/common/template",
)
