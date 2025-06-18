load("@bazel_template//bazel:common.bzl", "GLOBAL_COPTS", "GLOBAL_LINKOPTS", "GLOBAL_LOCAL_DEFINES", "version_info")
load("//bazel:build.bzl", "cc_test")
load("//bazel:cpplint.bzl", "cpplint")
load("//bazel:swig_java.bzl", "java_wrap_cc")

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

filegroup(
    name = "config_manager_swig",
    srcs = [
        "config_manager.i",
    ],
)

cc_library(
    name = "config_manager",
    srcs = ["config_manager.cc"],
    hdrs = ["config_manager.h"],
    copts = COPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        "//src/util:config_manager",
    ],
    alwayslink = True,
)

java_wrap_cc(
    name = "config_manager_swig_java",
    src = "config_manager.i",
    copts = COPTS,
    package = "com.xiedeacc.example",
    swig_includes = [
        ":config_manager_swig",
    ],
    swig_opt = "-DSWIGWORDSIZE64",
    tags = ["ignore"],
    use_directors = False,
    deps = [
        ":config_manager",
        "//src/util:config_manager",
    ],
)

cc_test(
    name = "config_manager_test",
    srcs = ["config_manager_test.cc"],
    copts = COPTS,
    data = ["//conf:client_config.json"],
    local_defines = LOCAL_DEFINES,
    tags = ["ignore"],
    deps = [
        ":config_manager",
    ],
)

#cpplint()
