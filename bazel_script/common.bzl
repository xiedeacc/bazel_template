def _run_command_impl(ctx):
    output_file = ctx.outputs.out
    openssl_path = "{}/external/openssl".format(ctx.genfiles_dir.path)
    command = "cd {} && ./Configure".format(openssl_path)
    print(command)
    ctx.actions.run_shell(
        outputs = [output_file],
        arguments = ["no-shared", "no-dso", "no-tests"],
        command = command,
    )

run_command = rule(
    implementation = _run_command_impl,
    attrs = {
        "out": attr.output(),
    },
)

def _build_openssl_impl(ctx):
    output_lib = ctx.outputs.lib

    openssl_dir = ctx.path(ctx.attr.openssl_dir)
    build_script = """
        set -e
        cd {openssl_dir}
        ./config
        make -j
        cp libssl.a libcrypto.a {output_lib}
    """.format(openssl_dir = openssl_dir, output_lib = output_lib.path)

    ctx.actions.run_shell(
        inputs = [ctx.attr.openssl_dir],
        outputs = [output_lib],
        command = build_script,
    )

build_openssl = rule(
    implementation = _build_openssl_impl,
    attrs = {
        "openssl_dir": attr.label(
            default = "@openssl//:BUILD.bazel",
            allow_single_file = True,
        ),
        "lib": attr.output(mandatory = True),
    },
)

def template_rule_impl(ctx):
    jemalloc_private_namespace = ctx.var.get("jemalloc_private_namespace", "je_")
    substitutions = {} | ctx.attr.substitutions
    substitutions.update([("#undef JEMALLOC_PRIVATE_NAMESPACE", "#define JEMALLOC_PRIVATE_NAMESPACE {}je_".format(jemalloc_private_namespace))])
    substitutions.update([("@private_namespace@", "{}je_".format(jemalloc_private_namespace))])

    ctx.actions.expand_template(
        template = ctx.file.src,
        output = ctx.outputs.out,
        substitutions = substitutions,
    )

template_rule = rule(
    attrs = {
        "src": attr.label(
            mandatory = True,
            allow_single_file = True,
        ),
        "substitutions": attr.string_dict(mandatory = True),
        "out": attr.output(mandatory = True),
    },
    # output_to_genfiles is required for header files.
    output_to_genfiles = True,
    implementation = template_rule_impl,
)

def _extract_symbols_impl(ctx):
    awk_script = ctx.file.awk_script

    # Gather all object files from the cc_library target
    obj_files = []
    for lib in ctx.attr.deps:
        cc_info = lib[CcInfo]

        #print(cc_info.compilation_context.)
        for linker_input in cc_info.linking_context.linker_inputs.to_list():
            for library in linker_input.libraries:
                for obj_file in library.pic_objects:
                    obj_files.append(obj_file)

    output_files = []
    for obj_file in obj_files:
        output_file = ctx.actions.declare_file(obj_file.basename + ".sym")
        output_files.append(output_file)

        # Create a command to extract symbols from all object files and process with awk
        ctx.actions.run_shell(
            inputs = obj_files + [awk_script],
            outputs = [output_file],
            command = "nm {} | awk -f {} >> {}".format(obj_file.path, awk_script.path, output_file.path),
        )

    return [DefaultInfo(files = depset(output_files))]

extract_symbols = rule(
    implementation = _extract_symbols_impl,
    attrs = {
        "deps": attr.label_list(
            allow_files = False,
            mandatory = True,
            #providers = ["CcInfo"],
        ),
        "awk_script": attr.label(allow_single_file = True),
    },
)
