load("@bazel_template//bazel:common.bzl", "GLOBAL_COPTS", "GLOBAL_LINKOPTS", "GLOBAL_LOCAL_DEFINES")
load("//bazel:build.bzl", "cc_test")
load("//bazel:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

COPTS = GLOBAL_COPTS + select({
    "@bazel_template//bazel:not_cross_compiling_on_windows": [
        "/Iexternal/double-conversion",
        "/Iexternal/folly",
        "/Iexternal/double-conversion",
        "/Iexternal/proxygen",
        "/I$(GENDIR)/external/proxygen",
        "/I$(GENDIR)/external/fizz",
        "/Iexternal/libsodium/src/libsodium/include",
        "/Iexternal/fizz",
        "/Iexternal/wangle",
        "/I$(GENDIR)/external/folly",
    ],
    "//conditions:default": [
        "-isystem external/double-conversion",
        "-isystem external/folly",
        "-isystem external/double-conversion",
        "-isystem external/proxygen",
        "-isystem $(GENDIR)/external/proxygen",
        "-isystem $(GENDIR)/external/fizz",
        "-isystem external/libsodium/src/libsodium/include",
        "-isystem external/fizz",
        "-isystem external/wangle",
        "-isystem $(GENDIR)/external/folly",
    ],
}) + select({
    "@platforms//os:linux": [],
    "@platforms//os:osx": [],
    "@platforms//os:windows": [],
    "//conditions:default": [],
})

LOCAL_DEFINES = GLOBAL_LOCAL_DEFINES + select({
    "@bazel_template//bazel:not_cross_compiling_on_windows": [
        "GLOG_NO_ABBREVIATED_SEVERITIES",
    ],
    "//conditions:default": ["BUILD_TRACING=0"],
}) + select({
    "@platforms//os:linux": [],
    "@platforms//os:osx": [],
    "@platforms//os:windows": [],
    "//conditions:default": [],
})

LINKOPTS = GLOBAL_LINKOPTS + select({
    "@bazel_template//bazel:not_cross_compiling_on_windows": [],
    "//conditions:default": [],
}) + select({
    "@platforms//os:linux": [],
    "@platforms//os:osx": [],
    "@platforms//os:windows": [],
    "//conditions:default": [],
})

cc_binary(
    name = "http_server",
    srcs = ["http_server.cc"],
    copts = COPTS,
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        #":version_info",
        "//src/server/http_handler",
        "@folly",
        "@openssl//:crypto",
        "@openssl//:ssl",
        "@proxygen",
    ],
)

cc_binary(
    name = "grpc_server",
    srcs = ["grpc_server.cc"],
    copts = COPTS,
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":grpc_server_impl",
        #":version_info",
        #"@jemalloc",
        "@openssl//:crypto",
        "@openssl//:ssl",
        "@libsodium//:sodium",
    ],
)

cc_library(
    name = "grpc_server_impl",
    srcs = ["grpc_server_impl.cc"],
    hdrs = [
        "grpc_server_impl.h",
        "grpc_service_handlers.h",
    ],
    copts = COPTS,
    defines = [
        "BOOST_BIND_GLOBAL_PLACEHOLDERS",
        "BOOST_THREAD_PROVIDES_FUTURE_CONTINUATION=1",
        "BOOST_THREAD_PROVIDES_EXECUTORS",
        "BOOST_THREAD_BUILD_LIB=1",
        "BOOST_THREAD_USES_CHRONO",
        "BOOST_THREAD_USES_MOVE",
    ] + select({
        "@platforms//os:windows": [
            "BOOST_THREAD_PLATFORM_WIN32",
            "BOOST_THREAD_WIN32",
        ],
        "//conditions:default": [
            "BOOST_THREAD_PLATFORM_PTHREAD",
            "BOOST_THREAD_POSIX",
        ],
    }),
    local_defines = LOCAL_DEFINES,
    deps = [
        "//src/common:blocking_list",
        "//src/common:garbage_collector",
        #"//src/context:grpc_context",
        "//src/proto:cc_grpc_service",
        "//src/context:server_run_info",
        "@boost//:thread",
        "//src/util",
        "//src/util:config_manager",
        "//src/async_grpc",
    ],
)

# version_info(
#     name = "version_info_h",
#     out = "version_info.h",
#     exec_tool = "//bazel:gen_local_config_git",
# )

# cc_library(
#     name = "version_info",
#     hdrs = [":version_info_h"],
#     visibility = ["//visibility:public"],
# )

# cc_test(
#     name = "version_info_test",
#     srcs = ["version_info_test.cc"],
#     deps = [
#         ":version_info",
#         "@com_github_glog_glog//:glog",
#         "@com_google_googletest//:gtest",
#     ],
# )

cpplint()
