load("//bazel_scripts:build.bzl", "cc_test")
load("//bazel_scripts:common.bzl", "version_info")
load("//bazel_scripts:cpplint.bzl", "cpplint")

cc_binary(
    name = "main",
    srcs = ["main.cc"],
    copts = [
        "-std=c++17",
    ],
    deps = [
        "@boost//:algorithm",
        "@curl",
        "@openssl",
    ],
)

#version_info(
#name = "version_info_h",
#out = "version_info.h",
#exec_tool = "//bazel_scripts:gen_local_config_git",
#)

#cc_library(
#name = "version_info",
#hdrs = [":version_info_h"],
#visibility = ["//visibility:public"],
#)

#cc_test(
#name = "version_info_test",
#srcs = ["version_info_test.cc"],
#deps = [
#":version_info",
#"@com_github_glog_glog//:glog",
#"@com_google_googletest//:gtest",
#],
#)

#cc_library(
#name = "util",
#hdrs = ["util.h"],
#copts = ["-std=c++17"],
#deps = ["@boost//:algorithm"],
#)

#cc_binary(
#name = "conf_gen",
#srcs = ["conf_gen.cc"],
#copts = ["-std=c++17"],
#deps = [":util"],
#)

#cc_binary(
#name = "conf_replace",
#srcs = ["conf_replace.cc"],
#copts = ["-std=c++17"],
#deps = [":util"],
#)

#cc_binary(
#name = "gen_build",
#srcs = ["gen_build.cc"],
#copts = ["-std=c++17"],
#deps = [":util"],
#)

cpplint()
