load("//tools:cpplint.bzl", "cpplint")
load("//tools:build.bzl", "cc_test")
load("//bazel:swig_java.bzl", "java_wrap_cc")

package(default_visibility = ["//visibility:public"])

config_setting(
    name = "on_linux",
    constraint_values = ["@platforms//os:linux"],
)

filegroup(
    name = "handler_swig",
    srcs = [
        "handler.i",
    ],
)

cc_library(
    name = "handler",
    srcs = ["handler.cc"],
    hdrs = ["handler.h"],
    deps = [
        "//src/server:service_impl",
    ],
    alwayslink = True,
)

cc_test(
    name = "handler_test",
    timeout = "short",
    srcs = ["handler_test.cc"],
    data = [],
    deps = [
        ":handler",
    ],
)

java_wrap_cc(
    name = "handler_swig_java",
    src = "handler.i",
    package = "bazel_template.swig",
    swig_includes = [
        ":handler_swig",
    ],
    swig_opt = "-DSWIGWORDSIZE64",
    use_directors = False,
    deps = [
        ":handler",
        "//src/util:config_util",
    ],
)

cpplint()
