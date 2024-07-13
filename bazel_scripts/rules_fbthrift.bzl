def _fbthrift_cpp_gen_impl(ctx):
    suffixs = [
        "_clients.h",
        "_constants.cpp",
        "_constants.h",
        "_data.cpp",
        "_data.h",
        "_for_each_field.h",
        "_handlers.h",
        "_metadata.h",
        "_types.cpp",
        "_types.h",
        "_types.tcc",
        "_types_custom_protocol.h",
        "_types_fwd.h",
        "_visit_by_thrift_field_metadata.h",
        "_visit_union.h",
        "_visitation.h",
    ]
    output_files = []
    for thrift_file in ctx.files.srcs:
        file_name = thrift_file.basename.removesuffix(".thrift")
        outputs = []
        for suffix in suffixs:
            output_file = ctx.actions.declare_file("{}/gen-cpp2/{}{}".format(ctx.attr.out_dir, file_name, suffix))
            outputs.append(output_file)
            output_files.append(output_file)
            print(output_file.path)
        ctx.actions.run(
            inputs = [thrift_file] + ctx.files.data,
            outputs = outputs,
            arguments = [
                "--gen",
                ctx.attr.gen_para,
                "-o",
                ctx.attr.out_dir,
            ] + ctx.attr.includes + [
                thrift_file.path,
            ],
            executable = ctx.executable.thrift,
        )

    return DefaultInfo(files = depset(output_files))

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
