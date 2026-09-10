load("@rules_cc//cc:defs.bzl", "cc_library", "cc_binary")
load("@bazel_template//bazel:common.bzl", "GLOBAL_COPTS", "GLOBAL_LINKOPTS", "GLOBAL_LOCAL_DEFINES", "version_info")
load("//bazel:build.bzl", "cc_test")

package(
    default_visibility = ["//visibility:public"],
    features = [
        "cpp_modules",
        "layering_check",
    ],
)

COPTS = GLOBAL_COPTS + select({
    "@platforms//os:windows": [
        "/Iexternal/double-conversion",
        "/Iexternal/folly",
        "/Iexternal/double-conversion",
        "/Iexternal/proxygen",
        "/I$(GENDIR)/external/proxygen",
        "/I$(GENDIR)/external/fizz",
        "/Iexternal/libsodium/src/libsodium/include",
        "/Iexternal/fizz",
        "/Iexternal/wangle",
        "/I$(GENDIR)/external/folly",
    ],
    "//conditions:default": [
        "-isystem external/double-conversion",
        "-isystem external/folly",
        "-isystem external/double-conversion",
        "-isystem external/proxygen",
        "-isystem $(GENDIR)/external/proxygen",
        "-isystem $(GENDIR)/external/fizz",
        "-isystem external/libsodium/src/libsodium/include",
        "-isystem external/fizz",
        "-isystem external/wangle",
        "-isystem $(GENDIR)/external/folly",
    ],
}) + select({
    "@platforms//os:linux": [],
    "@platforms//os:osx": [],
    "@platforms//os:windows": [],
    "//conditions:default": [],
})

LOCAL_DEFINES = GLOBAL_LOCAL_DEFINES + select({
    "@platforms//os:linux": [],
    "@platforms//os:osx": [],
    "@platforms//os:windows": [],
    "//conditions:default": [],
})

LINKOPTS = GLOBAL_LINKOPTS + select({
    "@platforms//os:windows": [],
    "//conditions:default": [],
}) + select({
    "@platforms//os:linux": [],
    "@platforms//os:osx": [],
    "@platforms//os:windows": [],
    "//conditions:default": [],
})

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
    deps = [":version_info"],
)

# bazel_template.server.context
cc_library(
    name = "context",
    srcs = ["context.cc"],
    copts = COPTS,
    features = ["-layering_check"],
    local_defines = LOCAL_DEFINES,
    module_interfaces = ["context.cppm"],
    deps = [
        ":version_info",
        "//src/async_grpc",
        "//src/util:config_manager",
        "@com_github_glog_glog//:glog",
    ],
)

# bazel_template.server.grpc_server; grpc_server.cc registers the handlers.
cc_library(
    name = "grpc_server",
    srcs = ["grpc_server.cc"],
    copts = COPTS,
    features = ["-layering_check"],
    local_defines = LOCAL_DEFINES,
    module_interfaces = ["grpc_server.cppm"],
    deps = [
        ":context",
        "//src/async_grpc",
        "//src/server/grpc_handler",
        "//src/util:config_manager",
        "@com_github_grpc_grpc//:grpc++",
    ],
)

# bazel_template.server.http_server; http_server.cc holds the proxygen code.
cc_library(
    name = "http_server",
    srcs = ["http_server.cc"],
    copts = COPTS,
    features = ["-layering_check"],
    local_defines = LOCAL_DEFINES,
    module_interfaces = ["http_server.cppm"],
    deps = [
        ":context",
        "//src/server/http_handler",
        "//src/util:config_manager",
        "@folly",
        "@proxygen",
    ],
)

cc_binary(
    name = "server",
    srcs = ["server.cc"],
    copts = COPTS,
    linkopts = LINKOPTS,
    linkstatic = True,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":context",
        ":grpc_server",
        ":http_server",
        "//src/proto:cc_grpc_service",
        "//src/util:config_manager",
        "@boost//:url",
        "@com_github_glog_glog//:glog",
        "@com_github_grpc_grpc//:grpc++",
        "@folly",
        "@gflags",
        "@libsodium//:sodium",
        "@openssl//:crypto",
        "@openssl//:ssl",
        "@xz//:lzma",
    ],
)
