load("@bazel_template//bazel:common.bzl", "GLOBAL_COPTS", "GLOBAL_LINKOPTS", "GLOBAL_LOCAL_DEFINES", "version_info")
load("//bazel:build.bzl", "cc_test")
load("//bazel:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

cpplint()

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

cc_library(
    name = "server_context",
    hdrs = ["server_context.h"],
    local_defines = LOCAL_DEFINES,
    deps = [
        ":version_info",
        "//src/util:config_manager",
        "@fmt",
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
    deps = [":version_info"],
)

cc_library(
    name = "grpc_server_impl",
    hdrs = ["grpc_server_impl.h"],
    copts = COPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        "//src/async_grpc",
        "//src/proto:cc_grpc_service",
        "//src/server:server_context",
        "//src/server/grpc_handler",
        "//src/util:config_manager",
    ],
)

cc_library(
    name = "http_server_impl",
    hdrs = ["http_server_impl.h"],
    copts = COPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        "//src/server:server_context",
        "//src/util:config_manager",
        "@proxygen",
    ],
)

cc_binary(
    name = "server",
    srcs = ["server.cc"],
    copts = COPTS,
    linkopts = LINKOPTS + select({
        "@bazel_template//bazel:cross_compiling_for_linux_aarch64_musl": [],
        "//conditions:default": [],
    }),
    local_defines = LOCAL_DEFINES,
    deps = [
        ":server_context",
        "//src/proto:cc_grpc_service",
        "//src/server/http_handler",
        "@folly",
        "@libsodium//:sodium",
        "@openssl//:crypto",
        ":http_server_impl",
        #"@jemalloc",
        "@boost//:url",
        "@openssl//:ssl",
        "@xz//:lzma",
    ],
)
