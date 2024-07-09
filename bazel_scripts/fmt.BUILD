cc_library(
    name = "fmt",
    hdrs = [
        "include/fmt/args.h",
        "include/fmt/base.h",
        "include/fmt/chrono.h",
        "include/fmt/color.h",
        "include/fmt/compile.h",
        "include/fmt/core.h",
        "include/fmt/format.h",
        "include/fmt/format-inl.h",
        "include/fmt/os.h",
        "include/fmt/ostream.h",
        "include/fmt/printf.h",
        "include/fmt/ranges.h",
        "include/fmt/std.h",
        "include/fmt/xchar.h",
    ],
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
