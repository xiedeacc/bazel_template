load("@hedron_compile_commands//:refresh_compile_commands.bzl", "refresh_compile_commands")
load("@bazel_problem//bazel_script:common.bzl", "run_command")
load("@bazel_problem//bazel_script:common.bzl", "build_openssl")

package(default_visibility = ["//visibility:public"])

exports_files(["CPPLINT.cfg"])

refresh_compile_commands(
    name = "refresh_compile_commands",
    targets = {
        "//...": "",
        #"//:my_output_1": "--important_flag1 --important_flag2=true",
    },
)

build_openssl(
    name = "openssl_build",
    lib = "libopenssl.a",
)
