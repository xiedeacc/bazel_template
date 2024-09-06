load("@bazel_template//bazel:rules_fbthrift.bzl", "fbthrift_cpp_gen", "fbthrift_service_cpp_gen")

package(default_visibility = ["//visibility:public"])

COPTS = [
    "-isystem external/fbthrift",
    "-isystem $(GENDIR)/external/fbthrift",
    "-isystem external/fb303",
    "-isystem $(GENDIR)/external/fb303",
    "-isystem external/double-conversion",
    "-isystem external/xxhash",
    "-isystem external/com_googlesource_code_re2",
    "-isystem external/fatal",
    "-isystem external/fbthrift",
    "-isystem $(GENDIR)/external/folly",
    "-isystem $(GENDIR)/external/fizz",
    "-isystem external/folly",
    "-isystem external/fizz",
    "-isystem external/wangle",
    "-isystem external/mvfst",
]

LOCAL_DEFINES = [
    "NDEBUG",
]

fbthrift_cpp_gen(
    name = "fb303_core_thrift_cpp",
    srcs = ["fb303/thrift/fb303_core.thrift"],
    data = ["@fbthrift//:fbthrift_libraries"],
    gen_para = ["include_prefix=fb303/thrift"],
    includes = [
        ## buildifier: leave-alone
        "-I",
        "external/fb303",
        ## buildifier: leave-alone
    ],
    out_dir = "fb303/thrift",
    out_files = {
        "fb303/thrift/fb303_core.thrift": "fb303_core",
    },
)

fbthrift_service_cpp_gen(
    name = "fb303_core_thrift_service_cpp",
    srcs = ["fb303/thrift/fb303_core.thrift"],
    data = ["@fbthrift//:fbthrift_libraries"],
    gen_para = ["include_prefix=fb303/thrift"],
    includes = [
        ## buildifier: leave-alone
        "-I",
        "external/fb303",
        ## buildifier: leave-alone
    ],
    out_dir = "fb303/thrift",
    out_files = {
        "fb303/thrift/fb303_core.thrift": "BaseService",
    },
)

cc_library(
    name = "fb303",
    srcs = [
        ":fb303_core_thrift_cpp",
        ":fb303_core_thrift_service_cpp",
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
        "@fbthrift",
        "@yaml-cpp",
    ],
)
