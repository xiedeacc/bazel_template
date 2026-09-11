load("@rules_cc//cc:defs.bzl", "cc_library")
load("@bazel_template//bazel:common.bzl", "GLOBAL_COPTS", "GLOBAL_LINKOPTS", "GLOBAL_LOCAL_DEFINES")
load("//bazel:build.bzl", "cc_test")
load("//bazel:pb_code_gen_plugin.bzl", "cc_proto_plugin")

package(
    default_visibility = ["//visibility:public"],
    features = [
        "cpp_modules",
        "layering_check",
    ],
)

COPTS = GLOBAL_COPTS

LOCAL_DEFINES = GLOBAL_LOCAL_DEFINES

LINKOPTS = GLOBAL_LINKOPTS

# A header library on purpose: its API is macros. See logging.h.
cc_library(
    name = "logging",
    srcs = ["logging.cc"],
    hdrs = ["logging.h"],
    copts = COPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        "@com_github_glog_glog//:glog",
        "@spdlog",
    ],
)

# Registers WSAStartup in a C initializer so proxygen's namespace-scope
# socket addresses can resolve before main() on Windows. See winsock_init.cc.
cc_library(
    name = "winsock_init",
    srcs = ["winsock_init.cc"],
    copts = COPTS,
    local_defines = LOCAL_DEFINES,
    alwayslink = True,
)

# bazel_template.common.defs
cc_library(
    name = "defs",
    copts = COPTS,
    local_defines = LOCAL_DEFINES,
    module_interfaces = ["defs.cppm"],
)

# A template with only standard headers; it stays a header so that it can sit
# in any module's global module fragment.
cc_library(
    name = "blocking_queue",
    hdrs = ["blocking_queue.h"],
    copts = COPTS,
    local_defines = LOCAL_DEFINES,
)

cc_test(
    name = "blocking_queue_test",
    srcs = ["blocking_queue_test.cc"],
    copts = COPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":blocking_queue",
        "@com_google_googletest//:gtest",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "socket_compat",
    hdrs = ["socket_compat.h"],
)
