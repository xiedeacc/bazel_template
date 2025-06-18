load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "rapidjson",
    hdrs = glob(["include/rapidjson/**/*.h"]),
    defines = ["RAPIDJSON_HAS_STDSTRING=1"],
    includes = ["include"],
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
)
