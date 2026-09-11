load("@hedron_compile_commands//:refresh_compile_commands.bzl", "refresh_compile_commands")

package(default_visibility = ["//visibility:public"])

exports_files([".clang-tidy"])

filegroup(
    name = "clang_tidy_config",
    srcs = [".clang-tidy"],
)

refresh_compile_commands(
    name = "refresh_compile_commands",
    # boost ships an .S assembled by MASM, which neither accepts the C++
    # flags nor writes the object file, so header scanning over external
    # sources fails; skipping their headers avoids it.
    exclude_headers = "external",
    targets = {
        "//src/...": "",
    },
)
