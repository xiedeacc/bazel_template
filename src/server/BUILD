load("//bazel:build.bzl", "cc_test")
load("//bazel:common.bzl", "version_info")
load("//bazel:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "http_server",
    srcs = ["http_server.cc"],
    copts = [
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
    deps = [
        ":version_info",
        "//src/server/http_handler",
        "@folly",
        "@jemalloc",
        "@libunwind//:unwind",
        "@openssl//:crypto",
        "@openssl//:ssl",
        "@proxygen",
    ],
)

cc_binary(
    name = "grpc_server",
    srcs = ["grpc_server.cc"],
    copts = [
        "-isystem external/double-conversion",
        "-isystem external/folly",
        "-isystem $(GENDIR)/external/folly",
    ],
    deps = [
        ":grpc_server_impl",
        ":version_info",
        "@jemalloc",
        "@libunwind//:unwind",
        "@openssl//:crypto",
        "@openssl//:ssl",
    ],
)

cc_library(
    name = "grpc_server_impl",
    srcs = ["grpc_server_impl.cc"],
    hdrs = [
        "grpc_server_impl.h",
        "grpc_service_handlers.h",
    ],
    copts = [
        "-isystem external/double-conversion",
        "-isystem external/folly",
        "-isystem $(GENDIR)/external/folly",
    ],
    defines = [
        "BOOST_THREAD_PROVIDES_FUTURE_CONTINUATION=1",
        "BOOST_THREAD_PROVIDES_EXECUTORS",
        "BOOST_BIND_GLOBAL_PLACEHOLDERS",
        "BOOST_THREAD_USES_MOVE",
    ],
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

version_info(
    name = "version_info_h",
    out = "version_info.h",
    exec_tool = "//bazel:gen_local_config_git",
)

cc_library(
    name = "version_info",
    hdrs = [":version_info_h"],
    visibility = ["//visibility:public"],
)

cc_test(
    name = "version_info_test",
    srcs = ["version_info_test.cc"],
    deps = [
        ":version_info",
        "@com_github_glog_glog//:glog",
        "@com_google_googletest//:gtest",
    ],
)

cpplint()
