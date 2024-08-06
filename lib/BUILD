load("//bazel:build.bzl", "cc_test")
load("//bazel:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

exports_files(glob(["**"]))

cc_library(
    name = "tcmalloc_lib",
    srcs = ["libtcmalloc.so"],
    hdrs = glob(["tcmalloc/**/*.h"]),
    includes = ["lib"],
    deps = [
        "@com_google_absl//absl/functional:function_ref",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/time",
        "@com_google_absl//absl/types:span",
    ],
)
