load("//bazel:build.bzl", "cc_test")
load("//bazel:cpplint.bzl", "cpplint")
load("//bazel:swig_java.bzl", "java_wrap_cc")

package(default_visibility = ["//visibility:public"])

exports_files(["CPPLINT.cfg"])

config_setting(
    name = "on_linux",
    constraint_values = ["@platforms//os:linux"],
)

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
    copts = [
        "-isystem external/double-conversion",
        "-isystem external/folly",
        "-isystem $(GENDIR)/external/folly",
    ],
    deps = [
        "//src/util:config_manager",
    ],
    alwayslink = True,
)

#java_wrap_cc(
#name = "config_manager_swig_java",
#src = "config_manager.i",
#copts = [
#"-isystem external/double-conversion",
#"-isystem external/folly",
#"-isystem $(GENDIR)/external/folly",
#],
#package = "com.xiedeacc.example",
#swig_includes = [
#":config_manager_swig",
#],
#swig_opt = "-DSWIGWORDSIZE64",
#use_directors = False,
#deps = [
#":config_manager",
#"//src/util:config_manager",
#],
#)

cc_test(
    name = "config_manager_test",
    srcs = ["config_manager_test.cc"],
    copts = [
        "-isystem external/double-conversion",
        "-isystem external/folly",
        "-isystem $(GENDIR)/external/folly",
    ],
    data = ["//conf:base_config.json"],
    deps = [
        ":config_manager",
    ],
)

cpplint()
