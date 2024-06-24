licenses(["notice"])  # Apache 2

cc_library(
    name = "rapidjson",
    hdrs = glob(["include/rapidjson/**/*.h"]),
    defines = ["RAPIDJSON_HAS_STDSTRING=1"],
    includes = ["include"],
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
    #visibility = ["@io_opencensus_cpp//opencensus/exporters/trace/zipkin:__pkg__"],
)
