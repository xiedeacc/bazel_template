cc_binary(
    name = "main",
    srcs = ["main.cc"],
    copts = [
        "-std=c++17",
    ],
    deps = [
        "@boost//:algorithm",
        "@openssl",
    ],
)

cc_library(
    name = "util",
    hdrs = ["util.h"],
    copts = ["-std=c++17"],
    deps = ["@boost//:algorithm"],
)

cc_binary(
    name = "conf_gen",
    srcs = ["conf_gen.cc"],
    copts = ["-std=c++17"],
    deps = [":util"],
)

cc_binary(
    name = "conf_replace",
    srcs = ["conf_replace.cc"],
    copts = ["-std=c++17"],
    deps = [":util"],
)

cc_binary(
    name = "gen_build",
    srcs = ["gen_build.cc"],
    copts = ["-std=c++17"],
    deps = [":util"],
)
