load("//bazel:build.bzl", "cc_test")
load("//bazel:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "conf_gen",
    srcs = ["conf_gen.cc"],
    deps = ["//src/util"],
)

cc_binary(
    name = "conf_diff",
    srcs = ["conf_diff.cc"],
    deps = ["//src/util"],
)

cc_binary(
    name = "conf_replace",
    srcs = ["conf_replace.cc"],
    deps = ["//src/util"],
)

cc_binary(
    name = "gen_build",
    srcs = ["gen_build.cc"],
    deps = ["//src/util"],
)

cpplint()
