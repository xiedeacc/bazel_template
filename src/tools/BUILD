load("@rules_cc//cc:defs.bzl", "cc_binary")
load("//bazel:build.bzl", "cc_test")

package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "gen_large_json_file",
    srcs = ["gen_large_json_file.cc"],
    deps = ["//src/util"],
)
