#cc_binary(
#name = "libjni_config_manger.so",
#linkshared = True,
#target_compatible_with = select({
#"@platforms//os:linux": [],
#"//conditions:default": ["@platforms//:incompatible"],
#}),
#visibility = ["//visibility:public"],
#deps = [
#"//src/swig:config_manager_swig_java_cc",
#],
#)

#java_library(
#name = "Loader",
#srcs = ["Loader.java"],
#runtime_deps = select({
#"@platforms//os:linux": [":libjni_config_manger.so"],
#"@platforms//os:windows": [],
#}),
#deps = [
#"@maven//:net_java_dev_jna_jna",
#],
#)
