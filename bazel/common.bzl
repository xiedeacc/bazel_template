load("@bazel_tools//tools/cpp:toolchain_utils.bzl", "find_cpp_toolchain", "use_cpp_toolchain")

GLOBAL_COPTS = select({
    "@bazel_template//bazel:cross_compiling_for_osx_gcc": [
        "-Wall",
        "-Wextra",
        "-O2",
        "-g",
        "-mmacosx-version-min=10.15",
        "-pthread",
    ],
    "@bazel_template//bazel:cross_compiling_for_osx_clang": [
        "-Wall",
        "-Wextra",
        "-O2",
        "-g",
        "-stdlib=libc++",
        "-mmacosx-version-min=10.15",
        "-pthread",
    ],
    "@bazel_template//bazel:not_cross_compiling_on_osx": [
        "-Wall",
        "-Wextra",
        "-O2",
        "-g",
        "-stdlib=libc++",
        "-mmacosx-version-min=10.15",
        "-pthread",
    ],
    "@bazel_template//bazel:cross_compiling_for_windows_gcc": [
        "-Wall",
        "-Wextra",
        "-O2",
        "-g",
        "-pthread",
    ],
    "@bazel_template//bazel:not_cross_compiling_on_windows": [
        "/GS",  #enable buffer security checks
        "/Gy",  #enables function-level linking
        "/Wall",
        "/Qpar",  #enables automatic parallelization of loops
        "/O2",  #enables full optimization for speed
        "/Ob2",  #enables aggressive inlining of functions.
        "/Oi",  #enables the use of intrinsic functions
        "/Ot",  #optimizes code for speed by favoring fast execution over smaller code size
        "/WX-",  #Disable Treat Warnings as Errors:
        "/FC",  #displays the full path of source file names in diagnostic messages
        "/GR",  #Enable Run-Time Type Information (RTTI):
        "/EHs",  #enables C++ exception handling and assumes that functions declared with throw() do not throw exceptions
        "/permissive-",  #enable standards-conforming behavior in the compiler
        "/fp:precise",
        "/GF",  #Enable String Pooling
        "/Zc:referenceBinding",  #Ensures that temporary object lifetimes are correctly extended in reference binding scenarios according to the C++ standard
        "/Zc:implicitNoexcept",  #Treats move constructors and move assignment operators as noexcept by default if they do not explicitly throw
        "/Zc:strictStrings",  # Enforces const-correctness for string literals, preventing accidental modification
        "/Zc:threadSafeInit",
        "/Zc:throwingNew",
        "/favor:blend",
        "/Gw",  #Enables data-level optimization across multiple translation units, improving runtime performance.
        "/Zc:rvalueCast",  #enforces standard behavior for rvalue casts
        "/Zc:wchar_t",
        #"/Zi",
        "/Zc:forScope",  #Enforce Standard for Loop Scope
        "/Zc:inline",  #enforces standard C++ behavior for inline functions
        "/Gd",  #__cdecl Calling Convention
        "/Gm-",  #disables minimal rebuild
        "/diagnostics:column",
        "/nologo",  #suppresses the display of the startup banner and informational messages during compilation
        #"/MD",
        "/MP",
        #"/errorReport:prompt",  #permission to send error reports to Microsoft
        "/showIncludes",  #
    ],
    "//conditions:default": [
        "-Wall",
        "-Wextra",
        "-O2",
        "-g",
        "-pthread",
        "-Wpointer-arith",
        "-Wstrict-prototypes",
        "-Wdeclaration-after-statement",
        "-Wswitch-enum",
        "-Wshadow",
        "-Wcast-align",
        "-Wcast-qual",
        "-Wundef",
    ],
})

GLOBAL_LOCAL_DEFINES = select({
    "@bazel_template//bazel:cross_compiling_for_osx_gcc": [],
    "@bazel_template//bazel:cross_compiling_for_osx_clang": [],
    "@bazel_template//bazel:not_cross_compiling_on_osx": [],
    "@bazel_template//bazel:cross_compiling_for_windows_gcc": [],
    "@bazel_template//bazel:not_cross_compiling_on_windows": [
        "_MBCS",
        "WIN32",
        "_WIN32",
        "_WINDOWS",
        "NDEBUG",
        "_MSC_VER=1941",
        "WIN64",
        "_WIN64",
        "_UNICODE",
        "UNICODE",
    ],
    "//conditions:default": [],
})

GLOBAL_LINKOPTS = select({
    "@bazel_template//bazel:cross_compiling_for_osx_gcc": [
        "-mmacosx-version-min=10.15",
        "-pthread",
    ],
    "@bazel_template//bazel:cross_compiling_for_osx_clang": [
        "-mmacosx-version-min=10.15",
        "-pthread",
    ],
    "@bazel_template//bazel:not_cross_compiling_on_osx": [
        "-lc++abi",
        "-mmacosx-version-min=10.15",
        "-pthread",
    ],
    "@bazel_template//bazel:cross_compiling_for_windows_gcc": [
        "-pthread",
    ],
    "@bazel_template//bazel:not_cross_compiling_on_windows": [
        "/MACHINE:X64",
        "Ws2_32.Lib",
        "Crypt32.Lib",
        "User32.lib",
        "kernel32.lib",
        "shell32.lib",
        "advapi32.lib",
        "bcrypt.lib",
        "iphlpapi.lib",
        "gdi32.lib",
        "winspool.lib",
        "ole32.lib",
        "oleaut32.lib",
        "uuid.lib",
        "comdlg32.lib",
    ],
    "//conditions:default": ["-pthread"],
})

def dict_union(x, y):
    z = {}
    z.update(x)
    z.update(y)
    return z

def _template_rule_impl(ctx):
    ctx.actions.expand_template(
        template = ctx.file.src,
        output = ctx.outputs.out,
        substitutions = ctx.attr.substitutions,
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
    output_to_genfiles = True,
    implementation = _template_rule_impl,
)

def _extract_symbols_impl(ctx):
    awk_script = ctx.file.awk_script
    obj_files = []
    for lib in ctx.attr.deps:
        cc_info = lib[CcInfo]
        for linker_input in cc_info.linking_context.linker_inputs.to_list():
            for library in linker_input.libraries:
                for obj_file in library.objects:
                    if "/external/jemalloc" in obj_file.path:
                        obj_files.append(obj_file)

    cc_toolchain = find_cpp_toolchain(ctx)
    output_files = []
    for obj_file in obj_files:
        output_file = ctx.actions.declare_file(obj_file.basename + ".sym")
        output_files.append(output_file)

        ctx.actions.run_shell(
            inputs = obj_files + [awk_script],
            outputs = [output_file],
            command = "{} {} | awk -f {} >> {}".format(cc_toolchain.nm_executable, obj_file.path, awk_script.path, output_file.path),
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
        "_cc_toolchain": attr.label(default = Label("@bazel_tools//tools/cpp:current_cc_toolchain")),
    },
    toolchains = use_cpp_toolchain(),
    fragments = ["cpp"],
)

def _local_config_git_genrule_impl(ctx):
    ctx.actions.run_shell(
        outputs = [ctx.outputs.out],
        inputs = [f for t in ctx.attr.srcs for f in t.files.to_list()],
        tools = [ctx.executable.exec_tool],
        arguments = [f.path for t in ctx.attr.srcs for f in t.files.to_list()] +
                    [ctx.outputs.out.path],
        command = "%s %s" % (ctx.executable.exec_tool.path, ctx.attr.arguments),
        execution_requirements = {"no-remote-exec": ""},
        use_default_shell_env = True,
    )

_local_config_git_genrule_internal = rule(
    implementation = _local_config_git_genrule_impl,
    attrs = {
        "out": attr.output(),
        "exec_tool": attr.label(
            executable = True,
            #cfg = _local_exec_transition,
            cfg = "exec",
            allow_files = True,
        ),
        "arguments": attr.string(),
        "srcs": attr.label_list(
            allow_files = True,
        ),
    },
)

def _local_config_git_genrule(**kwargs):
    _local_config_git_genrule_internal(
        **kwargs
    )

def version_info(name, out, exec_tool):
    _local_config_git_genrule(
        name = name,
        out = out,
        exec_tool = exec_tool,
        srcs = [
            "@local_config_git//:gen/spec.json",
            "@local_config_git//:gen/head",
            "@local_config_git//:gen/branch_ref",
        ],
        arguments = "--generate \"$@\"",
    )

def os(repository_ctx):
    name = repository_ctx.os.name
    if name == "linux":
        return "linux"
    elif name == "mac os x":
        return "darwin"
    elif name.startswith("windows"):
        return "windows"
    fail("Unsupported OS: " + name)

def _get_python_bin_path(repository_ctx):
    if repository_ctx.attr.python_bin_path != "":
        return repository_ctx.attr.python_bin_path
    python_bin_path = repository_ctx.which("python3")
    if python_bin_path != None:
        return str(python_bin_path)
    python_bin_path = repository_ctx.which("python")
    if python_bin_path != None:
        return str(python_bin_path)
    fail("find python error, install python and put it into PATH")

def _gen_local_config_git_impl(repository_ctx):
    local_config_git_build = """
# Description:
# Exports generated files used to generate tensorflow/core/util/version_info.cc

package(default_visibility = ["//visibility:public"])

licenses(["notice"])

exports_files(
    glob(["gen/*"]),
)
    """
    repository_ctx.file(
        "BUILD.bazel",
        content = local_config_git_build,
        executable = False,
    )

    workspace_root_path = str(repository_ctx.path(
        Label("//:BUILD"),
    ))[:-len("BUILD")]

    python_script_path = repository_ctx.path(
        Label("//bazel:gen_local_config_git.py"),
    )
    generated_files_path = repository_ctx.path("gen")

    result = repository_ctx.execute([
        _get_python_bin_path(repository_ctx),
        python_script_path,
        "--configure",
        workspace_root_path,
        "--gen_root_path",
        generated_files_path,
    ], quiet = False)

    if not result.return_code == 0:
        fail(result.stderr)

gen_local_config_git = repository_rule(
    implementation = _gen_local_config_git_impl,
    attrs = {
        "python_bin_path": attr.string(mandatory = False),
    },
)
