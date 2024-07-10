load("@hedron_compile_commands//:refresh_compile_commands.bzl", "refresh_compile_commands")

package(default_visibility = ["//visibility:public"])

config_setting(
    name = "windows_x86_64",
    constraint_values = [
        "@platforms//os:windows",
        "@platforms//cpu:x86_64",
    ],
    visibility = ["//visibility:public"],
)

platform(
    name = "linux_aarch64",
    constraint_values = [
        "@platforms//cpu:aarch64",
        "@platforms//os:linux",
    ],
)

platform(
    name = "linux_x86_64",
    constraint_values = [
        "@platforms//cpu:x86_64",
        "@platforms//os:linux",
    ],
)

refresh_compile_commands(
    name = "refresh_compile_commands",
    targets = {
        "//...": "",
        #"//:my_output_1": "--important_flag1 --important_flag2=true",
    },
)

cc_library(
    name = "mvfst",
    srcs = glob(
        [
            "quic/**/*.cpp",
        ],
        exclude = [
            "quic/samples/**",
            "quic/**/test/**",
            "quic/docs/**",
            "quic/tools/tperf/**/*.cpp",
        ],
    ),
    hdrs = glob(
        [
            "quic/**/*.h",
        ],
        exclude = [
            "quic/samples/**/*.h",
            "quic/**/test/**",
            "quic/docs/**",
            "quic/tools/tperf/**/*.h",
        ],
    ),
    copts = [
        "-isystem .",
        "-isystem $(BINDIR)/external/folly",
        "-isystem external/folly",
        "-isystem $(BINDIR)/external/fizz",
        "-isystem external/fizz",
        "-isystem external/libev",
        "-std=c++17",
    ],
    local_defines = [
        "MVFST_USE_LIBEV",
    ],
    deps = [
        "@fizz",
        "@libev//:ev",
    ],
)
