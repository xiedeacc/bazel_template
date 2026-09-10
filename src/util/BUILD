load("@rules_cc//cc:defs.bzl", "cc_library")
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

# Forward declarations of the third-party types module interfaces may name
# but must not define in their global module fragment (see util_fwd.h).
cc_library(
    name = "util_fwd",
    hdrs = ["util_fwd.h"],
)

# bazel_template.util; util.cc is an implementation unit.
cc_library(
    name = "util",
    srcs = ["util.cc"],
    copts = COPTS,
    features = ["-layering_check"],
    local_defines = LOCAL_DEFINES,
    module_interfaces = ["util.cppm"],
    deps = [
        ":util_fwd",
        "@com_google_protobuf//:protobuf",
        "@com_google_protobuf//src/google/protobuf/json",
        "@openssl",
    ],
)

# bazel_template.util.config_manager
cc_library(
    name = "config_manager",
    srcs = ["config_manager.cc"],
    copts = COPTS,
    features = ["-layering_check"],
    local_defines = LOCAL_DEFINES,
    module_interfaces = ["config_manager.cppm"],
    deps = [
        ":util",
        "//src/proto:cc_config",
        "//src/proto:cc_config_fwd",
        "@com_github_glog_glog//:glog",
    ],
)

cc_test(
    name = "util_test",
    timeout = "short",
    srcs = ["util_test.cc"],
    copts = COPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":util",
        "@com_github_glog_glog//:glog",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "config_manager_test",
    srcs = ["config_manager_test.cc"],
    copts = COPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":config_manager",
        "@com_github_glog_glog//:glog",
        "@com_google_googletest//:gtest_main",
    ],
)
