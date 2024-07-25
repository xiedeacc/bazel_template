load("//bazel:build.bzl", "cc_test")
load("//bazel:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "util",
    hdrs = ["util.h"],
    deps = ["@boost//:algorithm"],
)

cpplint()
