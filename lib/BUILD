package(default_visibility = ["//visibility:public"])

filegroup(
    name = "boost_context_lib",
    srcs = [
        "libboost_context-gcc14-mt-x64-1_83.dll.a",
    ],
)

cc_library(
    name = "libunwind",
    srcs = ["libunwind/lib/libunwind.a"],
    hdrs = glob(["libunwind/include/**/*.h"]),
    includes = ["libunwind/include"],
)
