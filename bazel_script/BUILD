load("@bazel_skylib//lib:selects.bzl", "selects")

package(default_visibility = ["//visibility:public"])

filegroup(
    name = "exported_testdata",
    srcs = glob(["**"]),
)

exports_files(glob(["**"]))

config_setting(
    name = "linux_aarch64",
    constraint_values = [
        "@bazel_tools//platforms:linux",
        "@bazel_tools//platforms:aarch64",
    ],
)

config_setting(
    name = "linux_x86_64",
    constraint_values = [
        "@bazel_tools//platforms:linux",
        "@bazel_tools//platforms:x86_64",
    ],
)

selects.config_setting_group(
    name = "brpc_with_glog",
    match_any = [
        ":brpc_with_glog_deprecated_flag",
        ":brpc_with_glog_new_flag",
    ],
    visibility = ["//visibility:public"],
)

config_setting(
    name = "brpc_with_glog_deprecated_flag",
    define_values = {"with_glog": "true"},
)

config_setting(
    name = "brpc_with_glog_new_flag",
    define_values = {"BRPC_WITH_GLOG": "true"},
)
