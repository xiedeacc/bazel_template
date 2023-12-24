load("//tools:cpplint.bzl", "cpplint")
load("//tools:build.bzl", "cc_test")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "service_impl",
    srcs = ["service_impl.cc"],
    hdrs = ["service_impl.h"],
    deps = [
        "//src/proto:cc_service",
        "//src/util:config_util",
        "@brpc",
        "@brpc//:bvar",
    ],
)

cc_binary(
    name = "debug_client",
    srcs = ["debug_client.cc"],
    defines = [
        "BOOST_THREAD_BUILD_LIB=1",
        "BOOST_THREAD_PLATFORM_PTHREAD",
        "BOOST_THREAD_POSIX",
        "BOOST_THREAD_USES_CHRONO",
        "BOOST_THREAD_PROVIDES_FUTURE_CONTINUATION=1",
        "BOOST_THREAD_PROVIDES_EXECUTORS=1",
        "BOOST_THREAD_USES_MOVE=1",
        "GOOGLE_LOGGING",
    ],
    linkopts = [
        "-pthread",
    ],
    linkstatic = 1,
    deps = [
        "//src/proto:cc_service",
        "//src/util:config_util",
        "@boost//:thread",
    ],
)

cc_binary(
    name = "server",
    srcs = ["server.cc"],
    linkopts = [
        "-pthread",
    ],
    linkstatic = 1,
    deps = [
        "//src/server:service_impl",
        "@jemalloc",
    ],
)

cpplint()
