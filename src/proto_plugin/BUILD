load("//bazel:build.bzl", "cc_test")
load("//bazel:cpplint.bzl", "cpplint")
load("//bazel:pb_code_gen_plugin.bzl", "cc_proto_plugin")

package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "pb_code_generator",
    srcs = [
        "pb_code_generator.cc",
        "pb_code_generator.h",
    ],
    linkstatic = True,
    deps = [
        "//src/util",
        "@com_google_absl//absl/strings",
        "@com_google_protobuf//:protoc_lib",
    ],
)

cc_proto_plugin(
    name = "pb_code_generator_plugin",
    src = "//src/proto:service.proto",
    outs = [
        "proto_plugin/print_proto_message_name.cc",
    ],
    data = [
        "//src/proto_plugin/template:print_proto_message_name.cc",
    ],
    plugin = ":pb_code_generator",
    proto_deps = [
        "//src/proto:error.proto",
    ],
    proto_path = "src/proto",
    protoc = "@com_google_protobuf//:protoc",
    deps = [
    ],
)

cpplint()
