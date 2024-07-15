def _fbthrift_cpp_gen_impl(ctx):
    out_dir = ctx.actions.declare_directory(ctx.attr.out_dir)
    for thrift_file in ctx.files.srcs:
        ctx.actions.run(
            inputs = [thrift_file] + ctx.files.data,
            outputs = [out_dir],
            arguments = [
                "-r",
                "--legacy-strict",
                "--gen",
                ctx.attr.gen_para,
                "-o",
                out_dir.path,
            ] + ctx.attr.includes + [
                thrift_file.path,
            ],
            executable = ctx.executable.thrift,
        )

    return DefaultInfo(files = depset([out_dir]))

fbthrift_cpp_gen = rule(
    implementation = _fbthrift_cpp_gen_impl,
    attrs = {
        "srcs": attr.label_list(allow_files = [".thrift"], mandatory = True),
        "data": attr.label_list(mandatory = True, allow_files = [".thrift", ".h"]),
        "thrift": attr.label(executable = True, cfg = "host", default = "@fbthrift//:thrift1"),
        "gen_para": attr.string(mandatory = True),
        "includes": attr.string_list(mandatory = True),
        "out_dir": attr.string(mandatory = True),
    },
)
