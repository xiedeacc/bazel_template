load("//bazel:build.bzl", "cc_test")
load("//bazel:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

cpplint()

cc_binary(
    name = "gen_large_json_file",
    srcs = ["gen_large_json_file.cc"],
    deps = ["//src/util"],
)
