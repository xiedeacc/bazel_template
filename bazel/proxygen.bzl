def is_external():
    if native.repository_name() != "@":
        return True
    else:
        return False

def _proxygen_cpp_gen_impl(ctx):
    output_files = []
    for file in ctx.attr.out_files:
        output_files.append(ctx.actions.declare_file(file))

    if ctx.attr.is_external:
        repo_dir = "external/{}".format(ctx.label.workspace_name)
    else:
        repo_dir = "."

    # print("{}/{}".format(ctx.bin_dir.path, repo_dir))
    if ctx.attr.txt_file:
        ctx.actions.run_shell(
            inputs = ctx.files.data,
            outputs = output_files,
            tools = [ctx.executable.tool],
            command = "{} {} {} {}".format(ctx.executable.tool.path, ctx.file.txt_file.path, repo_dir, output_files[0].dirname),
        )
    else:
        ctx.actions.run_shell(
            inputs = ctx.files.data,
            outputs = output_files,
            tools = [ctx.executable.tool],
            command = "{} {}/{}".format(ctx.executable.tool.path, ctx.bin_dir.path, repo_dir),
        )

    return DefaultInfo(files = depset(output_files))

proxygen_cpp_gen = rule(
    implementation = _proxygen_cpp_gen_impl,
    attrs = {
        "data": attr.label_list(allow_files = True, mandatory = True),
        "out_files": attr.string_list(mandatory = True),
        "is_external": attr.bool(mandatory = True),
        "tool": attr.label(
            allow_files = True,
            executable = True,
            cfg = "exec",
            mandatory = True,
        ),
        "txt_file": attr.label(allow_single_file = True, mandatory = False),
    },
)
