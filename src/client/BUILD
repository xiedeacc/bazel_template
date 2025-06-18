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
    name = "websocket_client",
    hdrs = ["websocket_client.h"],
    copts = COPTS,
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        "//src/client/handler:handlers",
        "//src/common:blocking_queue",
        "//src/proto:cc_enums",
        "@boost//:asio",
        "@boost//:beast",
        "@boost//:json",
        "@folly",
        "@simdjson",
    ],
)

cc_test(
    name = "websocket_client_test",
    srcs = ["websocket_client_test.cc"],
    copts = COPTS,
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":websocket_client",
        "//src/proto:cc_service",
        "//src/util",
        "@boost//:asio",
        "@boost//:beast",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_binary(
    name = "client",
    srcs = ["client.cc"],
    copts = COPTS,
    linkopts = LINKOPTS + select({
        "@bazel_template//bazel:cross_compiling_for_linux_aarch64_musl": [],
        "//conditions:default": [],
    }),
    local_defines = LOCAL_DEFINES,
    deps = [
        "//src/proto:cc_grpc_service",
        "//src/util:config_manager",
        "@folly",
        "@libsodium//:sodium",
        "@openssl//:crypto",
        ":websocket_client",
        #"@jemalloc",
        "@boost//:url",
        "@openssl//:ssl",
        "@xz//:lzma",
    ],
)
