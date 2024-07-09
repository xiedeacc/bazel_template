cc_library(
    name = "fmt",
    hdrs = glob(["include/fmt/*.h"]),
    copts = [
        "-g",
        "-O3",
    ],
    includes = [
        "include",
    ],
    local_defines = [
        "FMT_HEADER_ONLY",
    ],
    visibility = ["//visibility:public"],
)
