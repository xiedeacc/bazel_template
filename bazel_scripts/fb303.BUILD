load("@bazel_template//bazel_scripts:rules_fbthrift_dir.bzl", "fbthrift_cpp_gen")

package(default_visibility = ["//visibility:public"])

config_setting(
    name = "windows_x86_64",
    constraint_values = [
        "@platforms//os:windows",
        "@platforms//cpu:x86_64",
    ],
    visibility = ["//visibility:public"],
)

config_setting(
    name = "linux_aarch64",
    constraint_values = [
        "@platforms//cpu:aarch64",
        "@platforms//os:linux",
    ],
)

config_setting(
    name = "linux_x86_64",
    constraint_values = [
        "@platforms//cpu:x86_64",
        "@platforms//os:linux",
    ],
)

platform(
    name = "linux_aarch64_platform",
    constraint_values = [
        "@platforms//cpu:aarch64",
        "@platforms//os:linux",
    ],
)

platform(
    name = "linux_x86_64_platform",
    constraint_values = [
        "@platforms//cpu:x86_64",
        "@platforms//os:linux",
    ],
)

COPTS = [
    "-isystem external/fb303",
    "-isystem $(BINDIR)/external/fb303",
    "-isystem external/double-conversion",
    "-isystem external/xxhash",
    "-isystem external/com_googlesource_code_re2",
    "-isystem external/fatal",
    "-isystem external/fbthrift",
    "-isystem $(BINDIR)/external/folly",
    "-isystem $(BINDIR)/external/fizz",
    "-isystem external/folly",
    "-isystem external/fizz",
    "-isystem external/wangle",
    "-isystem external/mvfst",
    "-std=c++17",
    "-fsized-deallocation",
]

LOCAL_DEFINES = [
    "NDEBUG",
]

fbthrift_cpp_gen(
    name = "fb303_core_thrift_cpp",
    srcs = [
        "fb303/thrift/fb303_core.thrift",
    ],
    data = ["@fbthrift//:fbthrift_libraries"],
    gen_para = "mstch_cpp2:include_prefix=fb303/thrift",
    includes = [
        ## buildifier: leave-alone
        "-I",
        ".",
        ## buildifier: leave-alone
    ],
    out_dir = "fb303/thrift",
)

cc_library(
    name = "fb303",
    srcs = [
        ":fb303_core_thrift_cpp",
    ] + glob(
        [
            "fb303/**/*.cpp",
        ],
        exclude = [
            "fb303/**/test/*.cpp",
        ],
    ),
    hdrs = [
        ":fb303_core_thrift_cpp",
    ] + glob(
        [
            "fb303/**/*.h",
        ],
        exclude = [
        ],
    ),
    copts = COPTS,
    deps = [
        "@fbthrift//:lib",
        "@yaml-cpp",
    ],
)
