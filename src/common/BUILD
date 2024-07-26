load("//bazel:build.bzl", "cc_test")
load("//bazel:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "blocking_list",
    #    srcs = ["blocking_queue.cc"],
    hdrs = ["blocking_list.h"],
    deps = [
    ],
)

cc_library(
    name = "dependency_map",
    srcs = ["dependency_map.cc"],
    hdrs = ["dependency_map.h"],
)

cc_library(
    name = "module",
    srcs = ["module.cc"],
    hdrs = ["module.h"],
    deps = [
        ":dependency_map",
        "@com_github_gflags_gflags//:gflags",
        "@com_github_glog_glog//:glog",
    ],
)

cc_library(
    name = "garbage_collector",
    srcs = ["garbage_collector.cc"],
    hdrs = [
        "garbage_collector.h",
    ],
    defines = [
        "BOOST_THREAD_BUILD_LIB=1",
        "BOOST_THREAD_PLATFORM_PTHREAD",
        "BOOST_THREAD_POSIX",
        "BOOST_THREAD_USES_CHRONO",
    ],
    deps = [
        ":module",
        "//src/util",
    ],
)

cc_library(
    name = "net",
    srcs = [
        "host_spec.cc",
        "ip_address.cc",
    ],
    hdrs = [
        "host_spec.h",
        "ip_address.h",
    ],
    deps = [
        "@com_google_absl//absl/strings",
    ],
)

cc_test(
    name = "ip_address_test",
    timeout = "short",
    srcs = [
        "ip_address_test.cc",
    ],
    deps = [
        ":net",
    ],
)

cc_test(
    name = "host_spec_test",
    timeout = "short",
    srcs = [
        "host_spec_test.cc",
    ],
    deps = [
        ":net",
    ],
)

cpplint()
