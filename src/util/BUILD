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
    name = "config_manager",
    srcs = ["config_manager.cc"],
    hdrs = ["config_manager.h"],
    copts = COPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":util",
        "//src/proto:cc_config",
        "@folly",
    ],
)

cc_library(
    name = "util",
    srcs = ["util.cc"],
    hdrs = ["util.h"],
    deps = [
        "@boost//:algorithm",
        "@boost//:beast",
        "@boost//:iostreams",
        "@com_github_glog_glog//:glog",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/synchronization",
        "@com_google_absl//absl/time",
        "@com_google_protobuf//:protobuf",
        "@fmt",
        "@folly",
        "@openssl",
        "@smhasher",
    ],
)

cc_test(
    name = "util_test",
    timeout = "short",
    srcs = ["util_test.cc"],
    deps = [
        ":util",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "config_manager_test",
    srcs = [
        "config_manager_test.cc",
    ],
    copts = COPTS,
    data = ["//conf:server_config.json"],
    local_defines = LOCAL_DEFINES,
    deps = [":config_manager"],
)
