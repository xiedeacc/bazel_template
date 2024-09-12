cc_binary(
    name = "libjni_config_manger.so",
    linkshared = True,
    tags = ["ignore"],
    target_compatible_with = select({
        "@platforms//os:linux": [],
        "@platforms//os:windows": [],
        "@platforms//os:osx": [],
        "//conditions:default": ["@platforms//:incompatible"],
    }),
    visibility = ["//visibility:public"],
    deps = [
        "//src/swig:config_manager_swig_java_cc",
    ],
)

java_library(
    name = "Loader",
    srcs = ["Loader.java"],
    tags = ["ignore"],
    runtime_deps = select({
        "@platforms//os:linux": [":libjni_config_manger.so"],
        "@platforms//os:windows": [":libjni_config_manger.so"],
        "@platforms//os:osx": [":libjni_config_manger.so"],
    }),
    deps = [
        "@maven//:net_java_dev_jna_jna",
    ],
)
