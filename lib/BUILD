package(default_visibility = ["//visibility:public"])
filegroup(
    name = "boost_context_lib",
    srcs = [
        "libboost_context-mgw14-mt-x64-1_83.a",
    ]
)
filegroup(
    name = "boost_context_lib_with_debug",
    srcs = [
        "libboost_context-mgw14-mt-d-x64-1_83.a",
    ]
)

cc_library(
    name = "libunwind",
    hdrs = glob(["include/**/*.h"]),
    srcs = ["lib/libunwind.a"],
)
