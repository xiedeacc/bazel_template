load("@rules_foreign_cc//foreign_cc:configure.bzl", "configure_make", "configure_make_variant")

package(default_visibility = ["//visibility:public"])

filegroup(
    name = "all",
    srcs = glob(
        ["**"],
        exclude = [],
    ),
    visibility = ["//visibility:public"],
)

configure_make(
    name = "libunwind_build",
    args = ["-j"],
    #autogen = True,
    autoreconf = True,
    autoreconf_options = ["-fi"],
    configure_in_place = True,
    configure_options = [
        "--enable-shared=no",
        "--enable-static=yes",
        "--disable-tests",
        "--enable-documentation=no",
        #"--enable-minidebuginfo",
        #"--enable-zlibdebuginfo",
    ],
    copts = [
        #"-std=c++17",
        "-fPIC",
    ],
    includes = [
    ],
    lib_source = ":all",
    linkopts = [
        #"$$EXT_BUILD_DEPS$$/lib/liblzma.pic.a",
        #"$$EXT_BUILD_DEPS$$/lib/libz.pic.a",
    ],
    out_shared_libs = [
        #"libunwind-coredump.so",
        #"libunwind-ptrace.so",
        #"libunwind-setjmp.so",
        #"libunwind-x86_64.so",
        #"libunwind.so",
    ],
    out_static_libs = [
        "libunwind-coredump.a",
        "libunwind-ptrace.a",
        "libunwind-setjmp.a",
        "libunwind-x86_64.a",
        "libunwind.a",
    ],
    tags = ["skip_on_windows"],
    targets = [
        "all",
        "install",
    ],
    #toolchain = "@rules_foreign_cc//toolchains:preinstalled_automake_toolchain",
    deps = [
        "@lz4",
        "@xz//:lzma",
        "@zlib",
        "@zstd",
    ],
)

# Workaround for https://github.com/bazelbuild/rules_foreign_cc/issues/227
cc_library(
    name = "unwind",
    tags = ["skip_on_windows"],
    visibility = ["//visibility:public"],
    deps = [
        ":libunwind_build",
    ],
)
