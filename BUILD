load("@hedron_compile_commands//:refresh_compile_commands.bzl", "refresh_compile_commands")

package(default_visibility = ["//visibility:public"])

exports_files([".clang-tidy"])

refresh_compile_commands(
    name = "refresh_compile_commands",
    # Skip sources in external repos. To emit their entries the script re-runs
    # the compiler for dependency scanning, and boost ships an .S assembled by
    # MASM, which neither understands the C++ flags nor writes the .obj -- the
    # script then fails parsing that output as makefile deps. Our own sources
    # are what an IDE needs anyway; headers from deps still resolve through the
    # include paths recorded here.
    exclude_external_sources = True,
    exclude_headers = "external",
    targets = {
        "//src/...": "",
    },
)

filegroup(
    name = "clang_tidy_config",
    srcs = [".clang-tidy"],
)
