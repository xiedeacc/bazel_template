filegroup(
    name = "files",
    srcs = glob(["*/**"]),
    visibility = ["//visibility:public"],
)

alias(
    name = "sysroot",
    actual = ":files",
    visibility = ["//visibility:public"],
)
