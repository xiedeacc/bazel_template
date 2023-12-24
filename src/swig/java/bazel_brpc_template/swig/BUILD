cc_binary(
    name = "libjni_handler.so",
    linkshared = True,
    target_compatible_with = select({
        "@platforms//os:linux": [],
        "//conditions:default": ["@platforms//:incompatible"],
    }),
    visibility = ["//visibility:public"],
    deps = [
        "//src/swig:handler_swig_java_cc",
    ],
)

java_library(
    name = "Loader",
    srcs = ["Loader.java"],
    runtime_deps = [":libjni_handler.so"],
    deps = [
        "@maven//:net_java_dev_jna_jna",
    ],
)
