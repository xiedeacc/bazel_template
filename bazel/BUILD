load("@bazel_skylib//lib:selects.bzl", "selects")

package(default_visibility = ["//visibility:public"])

filegroup(
    name = "exported_testdata",
    srcs = glob(["**"]),
)

exports_files(glob(["**"]))

py_binary(
    name = "gen_local_config_git",
    srcs = ["gen_local_config_git.py"],
    python_version = "PY3",
    srcs_version = "PY3",
    tags = ["no-remote-exec"],
)
