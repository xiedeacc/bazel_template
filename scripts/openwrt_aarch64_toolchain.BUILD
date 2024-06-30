package(default_visibility = ["//visibility:public"])

filegroup(
    name = "root",
    srcs = glob(["**"]),
)

filegroup(
    name = "header",
    srcs = [
        "aarch64-openwrt-linux-musl/include",
        "aarch64-openwrt-linux-musl/sys-include",
        "include/**",
    ],
)

filegroup(
    name = "lib",
    srcs = [
        "lib/**",
    ],
)

filegroup(
    name = "ar",
    srcs = ["bin/aarch64-openwrt-linux-musl-ar"],
)

filegroup(
    name = "ld",
    srcs = ["bin/aarch64-openwrt-linux-musl-ld.bin"],
)

filegroup(
    name = "cov",
    srcs = ["bin/aarch64-openwrt-linux-musl-gcov.bin"],
)

filegroup(
    name = "cpp",
    srcs = ["bin/aarch64-openwrt-linux-musl-cpp.bin"],
)

filegroup(
    name = "gcc",
    srcs = ["bin/aarch64-openwrt-linux-musl-gcc.bin"],
)

filegroup(
    name = "nm",
    srcs = ["bin/aarch64-openwrt-linux-musl-nm.bin"],
)

filegroup(
    name = "objcopy",
    srcs = ["bin/aarch64-openwrt-linux-musl-objcopy.bin"],
)

filegroup(
    name = "objdump",
    srcs = ["bin/aarch64-openwrt-linux-musl-objdump.bin"],
)

filegroup(
    name = "strip",
    srcs = ["bin/aarch64-openwrt-linux-musl-strip.bin"],
)
