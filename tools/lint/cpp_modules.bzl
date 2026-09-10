"""A clang-tidy lint aspect that can also see C++20 named modules.

Why this exists
---------------

rules_lint's `lint_clang_tidy_aspect` runs clang-tidy over a target's `srcs`
with that target's compile flags. Two things break once a target uses C++20
named modules:

  * a source that says `import bazel_template.x;` fails with "module 'bazel_template.x' not found",
    because clang-tidy has no binary module interface for it. Bazel does build
    one, but on Windows it is an MSVC `.ifc` that clang cannot read, and a BMI
    clang *could* read would have to come from a clang of exactly clang-tidy's
    own version -- the host here ships clang-tidy.exe and clang-format.exe with
    no clang driver at all;
  * a `.cppm` listed in `module_interfaces` is not in `srcs`, so it is never
    linted. Today the module interface is invisible to the gate.

So this aspect gives clang-tidy source instead of a BMI. Every target that
declares a module interface gets a *stub header* for it (the interface with
its module declarations neutralised) from //tools/lint's module_rewriter. A
linted target that can reach a module then gets a *tree*: a directory holding
a rewritten copy of each of its sources, each of its interfaces and every
header of this repository in its compile closure, each at its own repository
path, with `import` lines turned into includes of the stubs. clang-tidy
compiles the copies with the tree searched first for quoted includes, so a
header that itself imports a module works too. The rewrites are line- and
column-preserving and clang_tidy_driver maps the tree paths in the report back
to the files a developer edits, so a finding still points at
`src/.../greeter.cppm:15:23`. See module_rewriter.py for what that emulation
does and does not cover.

Targets with no modules anywhere in their deps take the original code path:
they are handed to rules_lint's own `clang_tidy_action`, unchanged, so the
existing gate keeps its behaviour, its checks and its output group names.

The flag-translation helpers below (`_update_flag` and friends) are copied from
@aspect_rules_lint//lint:clang_tidy.bzl, whose versions are private to that
file. They must keep producing the same command line as rules_lint does, since
both paths lint the same code base with the same .clang-tidy.
"""

load("@aspect_rules_lint//lint:clang_tidy.bzl", "clang_tidy_action")
load("@aspect_rules_lint//lint/private:lint_aspect.bzl", "LintOptionsInfo", "OPTIONAL_SARIF_PARSER_TOOLCHAIN", "OUTFILE_FORMAT", "noop_lint_action", "output_files", "parse_to_sarif_action", "patch_and_output_files", "should_visit")
load("@aspect_rules_lint//lint/private:patcher_action.bzl", "patcher_attrs")
load("@bazel_skylib//rules/directory:providers.bzl", "DirectoryInfo")
load("@bazel_tools//tools/build_defs/cc:action_names.bzl", "ACTION_NAMES")
load("@bazel_tools//tools/cpp:toolchain_utils.bzl", "find_cpp_toolchain")
load("@rules_cc//cc:defs.bzl", "CcInfo", "cc_common")

# Reused verbatim so that reports, output group names and `aspect lint` tooling
# cannot tell the two code paths apart.
_MNEMONIC = "AspectRulesLintClangTidy"

_DISABLED_FEATURES = ["layering_check"]

_HEADER_EXTENSIONS = ["h", "hh", "hpp", "hxx", "inc", "ipp", "tcc"]

# .clang-tidy's HeaderFilterRegex covers `src/` and excludes `bazel-out`, which
# is exactly where a tree lives. In tree mode the filters are given on the
# command line instead (which overrides the file): only rewritten copies of
# repository headers count, so generated headers stay excluded as before, and
# the stubs never report (they are system headers anyway).
_TREE_HEADER_FILTER = "_lint_modules[/\\\\][^/\\\\]+[/\\\\]tree[/\\\\](src|test_data)[/\\\\]"
_TREE_EXCLUDE_HEADER_FILTER = "(external|_virtual_includes|lintstub)"

CppModuleLintInfo = provider(
    doc = "Lint stubs for the C++20 named modules a target makes available.",
    fields = {
        "maps": "depset of JSON files, one per module interface, naming the module and its stub header",
        "stubs": "depset of the stub headers themselves, needed as inputs to any lint action that includes them",
    },
)

# ---------------------------------------------------------------------------
# Copied from @aspect_rules_lint//lint:clang_tidy.bzl (private there).
# ---------------------------------------------------------------------------

def _update_flag(flag):
    unsupported_flags = [
        "-fno-canonical-system-headers",
        "-fstack-usage",
        "/nologo",
        "/COMPILER_MSVC",
        "/showIncludes",
        "/experimental:external",
    ]
    unsupported_prefixes = ["/wd", "-W", "/W", "/external"]
    if flag in unsupported_flags:
        return []
    for prefix in unsupported_prefixes:
        if flag.startswith(prefix):
            return []

    flags = [flag]
    if flag.startswith("/std:"):
        flags = ["-std=" + flag.removeprefix("/std:")]
    elif flag.startswith("/D"):
        flags = ["-" + flag[1:]]
    elif flag.startswith("/FI"):
        flags = ["-include", flag.removeprefix("/FI")]
    elif flag.startswith("/I"):
        # Joined form: clang 22 no longer pairs a bare "-iquote" line in the
        # --config file with the line after it.
        flags = ["-iquote" + flag.removeprefix("/I")]
    elif flag in ["/MD", "/MDd", "/MT", "/MTd"]:
        flags = ["-D_MT"]
    elif flag.startswith("/"):
        return []
    return flags

def _safe_flags(ctx, flags):
    safe_flags = []
    for flag in flags:
        safe_flags.extend(_update_flag(flag))
    return safe_flags

def _feature_configuration(ctx):
    return cc_common.configure_features(
        ctx = ctx,
        cc_toolchain = find_cpp_toolchain(ctx),
        requested_features = ctx.features,
        unsupported_features = ctx.disabled_features + _DISABLED_FEATURES,
    )

def _compile_variables(ctx, feature_configuration, user_flags):
    return cc_common.create_compile_variables(
        feature_configuration = feature_configuration,
        cc_toolchain = find_cpp_toolchain(ctx),
        user_compile_flags = user_flags,
    )

def _toolchain_env(ctx, user_flags):
    feature_configuration = _feature_configuration(ctx)
    env = {}
    env.update(cc_common.get_environment_variables(
        feature_configuration = feature_configuration,
        action_name = ACTION_NAMES.cpp_compile,
        variables = _compile_variables(ctx, feature_configuration, user_flags),
    ))
    return env

def _toolchain_flags(ctx, user_flags):
    feature_configuration = _feature_configuration(ctx)
    return cc_common.get_memory_inefficient_command_line(
        feature_configuration = feature_configuration,
        action_name = ACTION_NAMES.cpp_compile,
        variables = _compile_variables(ctx, feature_configuration, user_flags),
    )

def _prefixed(items, prefix):
    return [prefix + arg for arg in items]

def _angle_includes_option(ctx):
    return "-isystem" if ctx.attr._angle_includes_are_system else "-I"

def _compiler_args(ctx, compilation_context):
    """The compile command line for a generated C++ translation unit."""
    args = []
    if ctx.attr._gcc_install_dir:
        for directory in ctx.attr._gcc_install_dir[0].files.to_list():
            args.append("--gcc-install-dir=" + directory.path)

    rule_flags = list(getattr(ctx.rule.attr, "copts", [])) + list(getattr(ctx.rule.attr, "cxxopts", []))
    user_flags = ctx.fragments.cpp.cxxopts + ctx.fragments.cpp.copts
    args.extend(_safe_flags(ctx, _toolchain_flags(ctx, user_flags) + rule_flags) + ["-xc++"])

    for define in compilation_context.defines.to_list():
        args.append("-D" + define)
    for define in compilation_context.local_defines.to_list():
        args.append("-D" + define)
    for define in getattr(ctx.rule.attr, "defines", []):
        args.append("-D" + define)
    for define in getattr(ctx.rule.attr, "local_defines", []):
        args.append("-D" + define)

    args.extend(_prefixed(compilation_context.framework_includes.to_list(), "-F"))
    args.extend(_prefixed(compilation_context.includes.to_list(), "-I"))
    args.extend(_prefixed(compilation_context.quote_includes.to_list(), "-iquote"))
    args.extend(_prefixed(compilation_context.system_includes.to_list(), _angle_includes_option(ctx)))
    args.extend(_prefixed(compilation_context.external_includes.to_list(), "-isystem"))
    return args

# ---------------------------------------------------------------------------
# Module rewriting
# ---------------------------------------------------------------------------

def _should_visit(rule, rule_kinds):
    """Which targets the aspect lints.

    rules_lint's own rule visits cc_binary and cc_library, which is why no test
    source in this repository has ever been linted. Turning that on wholesale
    would light up the gate (gtest's TEST macro alone trips cert-err58-cpp), so
    a test opts in one target at a time with `tags = ["lint"]`. "no-lint" still
    opts any target out.
    """
    if "lint" in rule.attr.tags and "no-lint" not in rule.attr.tags:
        return True
    return should_visit(rule, rule_kinds)

def _is_source(file):
    return file.is_source and file.extension in ["c", "cc", "cpp", "cxx", "c++", "C"]

def _filter_srcs(rule):
    if not hasattr(rule.attr, "srcs"):
        return []
    if "lint-genfiles" in rule.attr.tags:
        return rule.files.srcs
    return [s for s in rule.files.srcs if _is_source(s)]

def _module_interfaces(ctx):
    return getattr(ctx.rule.files, "module_interfaces", [])

def _dep_modules(ctx):
    maps = []
    stubs = []
    for attr in ["deps", "implementation_deps"]:
        for dep in getattr(ctx.rule.attr, attr, []):
            if CppModuleLintInfo in dep:
                maps.append(dep[CppModuleLintInfo].maps)
                stubs.append(dep[CppModuleLintInfo].stubs)
    return depset(transitive = maps), depset(transitive = stubs)

def _repository_headers(compilation_context):
    """Checked-in headers of this repository in a target's compile closure.

    Generated headers are left out on purpose: they are not linted today
    (.clang-tidy excludes bazel-out), and they are found through the ordinary
    include paths as before.
    """
    return [
        file
        for file in compilation_context.headers.to_list()
        if file.is_source and file.extension in _HEADER_EXTENSIONS and not file.short_path.startswith("../")
    ]

def _stub_action(ctx, interface, stub_out, map_out, dep_maps):
    args = ctx.actions.args()
    args.add("--source", interface)
    args.add("--stub-out", stub_out)
    args.add("--map-out", map_out)
    args.add_all(dep_maps, before_each = "--module-map")

    ctx.actions.run(
        inputs = depset([interface], transitive = [dep_maps]),
        outputs = [stub_out, map_out],
        executable = ctx.executable._module_rewriter,
        arguments = [args],
        mnemonic = "CppModuleLintStub",
        progress_message = "Preparing a lint stub for %{label}:" + interface.basename,
    )

def _tree_action(ctx, tree, copies, maps):
    """Write rewritten copies of `copies` ((file, path under tree) pairs)."""
    args = ctx.actions.args()
    args.set_param_file_format("multiline")
    args.use_param_file("@%s", use_always = True)
    args.add("--tree-out", tree.path)
    for file, rel in copies:
        args.add("--copy", file.path + "=" + rel)
    args.add_all(maps, before_each = "--module-map")

    ctx.actions.run(
        inputs = depset([file for file, _ in copies], transitive = [maps]),
        outputs = [tree],
        executable = ctx.executable._module_rewriter,
        arguments = [args],
        mnemonic = "CppModuleLintRewrite",
        progress_message = "Preparing %{label} for clang-tidy",
    )

def _declare_sibling(ctx, source, suffix):
    """Declare a generated file beside `source` in the package's bin dir.

    Keyed by the declaring target as well as the file: two targets may list the
    same source, and two actions may not write the same output.
    """
    name = source.basename
    if name.rfind(".") > 0:
        name = name[:name.rfind(".")]
    return ctx.actions.declare_file(
        "_lint_modules/" + ctx.label.name + "/" + name + suffix,
        sibling = source,
    )

# ---------------------------------------------------------------------------
# The lint action for a rewritten translation unit
# ---------------------------------------------------------------------------

def _module_lint_action(ctx, compilation_context, tree, unit, stubs, stdout, exit_code):
    tidy_args = ctx.actions.args()
    tidy_args.add("--clang-tidy", ctx.executable._clang_tidy)
    tidy_args.add("--out", stdout)
    if exit_code:
        tidy_args.add("--exit-code-file", exit_code)
    tidy_args.add("--map", unit.path + "=" + unit.original.short_path)
    tidy_args.add("--strip-prefix", tree.path)
    tidy_args.add("--")
    if ctx.files._global_config:
        tidy_args.add("--config-file=" + ctx.files._global_config[0].path)
    tidy_args.add("--header-filter=" + _TREE_HEADER_FILTER)
    tidy_args.add("--exclude-header-filter=" + _TREE_EXCLUDE_HEADER_FILTER)
    tidy_args.add_all(ctx.attr._args)
    tidy_args.add(unit.path)
    tidy_args.add("--")

    compiler_args = ctx.actions.args()

    # The tree must win over the checked-in files for quoted includes. clang
    # processes the options in a --config file before the ones on the command
    # line, so this has to be the first entry of that file; --extra-arg-before
    # would land behind the toolchain's own -iquote entries.
    compiler_args.add("-iquote" + tree.path)
    compiler_args.add_all(_compiler_args(ctx, compilation_context))
    # clang reads a whole command line out of a --config file. rules_lint does
    # the same; the joined form is required here because there is no shell to
    # split "--config <file>" into two arguments.
    compiler_args.use_param_file("--config=%s", use_always = True)

    inputs = [tree] + ctx.files._configs + ctx.files._global_config + ctx.files._deps
    outputs = [stdout] + ([exit_code] if exit_code else [])

    ctx.actions.run(
        inputs = depset(inputs, transitive = [compilation_context.headers, stubs]),
        outputs = outputs,
        executable = ctx.executable._clang_tidy_driver,
        tools = [ctx.executable._clang_tidy, find_cpp_toolchain(ctx).all_files],
        arguments = [tidy_args, compiler_args],
        env = _toolchain_env(ctx, ctx.fragments.cpp.cxxopts + ctx.fragments.cpp.copts),
        mnemonic = _MNEMONIC,
        progress_message = "Linting %{label}:" + unit.original.basename + " with clang-tidy",
    )

# ---------------------------------------------------------------------------
# Aspect
# ---------------------------------------------------------------------------

def _lint_one(ctx, compilation_context, tree, unit, output, stubs):
    """Lint one translation unit, module-aware or not, into `output`."""
    raw = ctx.actions.declare_file(OUTFILE_FORMAT.format(
        label = ctx.label.name + "_rules_lint/" + unit.original.short_path,
        mnemonic = _MNEMONIC,
        suffix = "raw_machine_report",
    ))
    if tree:
        _module_lint_action(ctx, compilation_context, tree, unit, stubs, output.human.out, output.human.exit_code)
        _module_lint_action(ctx, compilation_context, tree, unit, stubs, raw, output.machine.exit_code)
    else:
        clang_tidy_action(
            ctx,
            compilation_context,
            ctx.executable,
            [unit.original],
            output.human.out,
            output.human.exit_code,
            patch = getattr(output, "patch", None),
            args = ctx.attr._args,
        )
        clang_tidy_action(ctx, compilation_context, ctx.executable, [unit.original], raw, output.machine.exit_code, args = ctx.attr._args)
    parse_to_sarif_action(ctx, _MNEMONIC, raw, output.machine.out)

def _clang_tidy_aspect_impl(target, ctx):
    if CcInfo not in target:
        return []

    dep_maps, dep_stubs = _dep_modules(ctx)
    interfaces = _module_interfaces(ctx)

    # Build this target's own module stubs first: consumers need them even
    # when this target itself is not linted.
    own_maps = []
    own_stubs = []
    for interface in interfaces:
        stub = _declare_sibling(ctx, interface, ".lintstub.h")
        module_map = _declare_sibling(ctx, interface, ".lintmodule.json")
        _stub_action(ctx, interface, stub, module_map, dep_maps)
        own_maps.append(module_map)
        own_stubs.append(stub)

    modules_info = CppModuleLintInfo(
        maps = depset(own_maps, transitive = [dep_maps]),
        stubs = depset(own_stubs, transitive = [dep_stubs]),
    )

    # The aspect propagates through deps to collect module stubs, which takes
    # it into every external repository. Those are not ours to lint, and two
    # external targets listing the same source would declare the same report
    # file and fail analysis with an action conflict.
    if ctx.label.workspace_root:
        return [modules_info]

    if not _should_visit(ctx.rule, ctx.attr._rule_kinds):
        return [modules_info]

    compilation_context = target[CcInfo].compilation_context
    if hasattr(ctx.rule.attr, "implementation_deps"):
        compilation_context = cc_common.merge_compilation_contexts(
            compilation_contexts = [compilation_context] +
                                   [dep[CcInfo].compilation_context for dep in ctx.rule.attr.implementation_deps],
        )

    # A target only needs the module treatment if a module is reachable from
    # it. Everything else goes down rules_lint's original path untouched.
    all_maps = modules_info.maps
    all_stubs = modules_info.stubs
    module_mode = bool(interfaces) or bool(all_maps.to_list())

    srcs = _filter_srcs(ctx.rule)
    tree = None
    units = []
    if module_mode:
        tree = ctx.actions.declare_directory("_lint_modules/" + ctx.label.name + "/tree")
        copies = [(header, header.short_path) for header in _repository_headers(compilation_context)]
        for src in srcs:
            copies.append((src, src.short_path))
            units.append(struct(path = tree.path + "/" + src.short_path, original = src))
        for interface in interfaces:
            # Keep the module interface's own extension out of clang's way: a
            # `.cppm` input is compiled as a module unit, and the copy is not
            # one any more.
            copies.append((interface, interface.short_path + ".cc"))
            units.append(struct(path = tree.path + "/" + interface.short_path + ".cc", original = interface))
        _tree_action(ctx, tree, copies, all_maps)
    else:
        units = [struct(path = src.path, original = src) for src in srcs]

    if not units:
        outputs, info = patch_and_output_files(_MNEMONIC, target, ctx)
        noop_lint_action(ctx, outputs)
        return [modules_info, info]

    originals = [unit.original for unit in units]
    if ctx.attr._options[LintOptionsInfo].fix and not module_mode:
        outputs, info = patch_and_output_files(_MNEMONIC, target, ctx, files_to_lint = originals)
    else:
        outputs, info = output_files(_MNEMONIC, target, ctx, files_to_lint = originals)

    for output, unit in zip(outputs, units):
        _lint_one(ctx, compilation_context, tree, unit, output, all_stubs)

    return [modules_info, info]

DEFAULT_RULE_KINDS = ["cc_binary", "cc_library"]

def lint_clang_tidy_aspect(
        binary,
        configs = [],
        global_config = [],
        gcc_install_dir = [],
        deps = [],
        header_filter = "",
        lint_target_headers = False,
        angle_includes_are_system = True,
        verbose = False,
        rule_kinds = DEFAULT_RULE_KINDS,
        args = []):
    """Create a clang-tidy lint aspect that understands C++20 named modules.

    The arguments mirror `@aspect_rules_lint//lint:clang_tidy.bzl`'s factory of
    the same name, so switching between the two is a one-line change.

    Args:
        binary: the clang-tidy binary to run.
        configs: .clang-tidy files to make available to clang-tidy's config search.
        global_config: a single .clang-tidy file passed on the command line.
        gcc_install_dir: optional `Directory` passed as --gcc-install-dir.
        deps: extra files needed during the clang-tidy run.
        header_filter: regex for clang-tidy's -header-filter.
        lint_target_headers: pass a -header-filter covering the target's own headers.
        angle_includes_are_system: pass angle includes as -isystem rather than -I.
        verbose: print the clang-tidy command lines being invoked.
        rule_kinds: which target kinds to visit.
        args: additional options to pass to clang-tidy.

    Returns:
        an aspect usable with `--aspects`.
    """
    if type(global_config) == "string":
        global_config = [global_config]

    return aspect(
        implementation = _clang_tidy_aspect_impl,
        # Module stubs come from the libraries that declare the interfaces, so
        # the aspect has to reach a target's dependencies to collect them.
        attr_aspects = ["deps", "implementation_deps"],
        attrs = patcher_attrs | {
            "_options": attr.label(
                default = "@aspect_rules_lint//lint:options",
                providers = [LintOptionsInfo],
            ),
            "_configs": attr.label_list(default = configs, allow_files = True),
            "_global_config": attr.label_list(default = global_config, allow_files = True),
            "_deps": attr.label_list(default = deps),
            "_gcc_install_dir": attr.label_list(default = gcc_install_dir, providers = [DirectoryInfo]),
            "_lint_target_headers": attr.bool(default = lint_target_headers),
            "_header_filter": attr.string(default = header_filter),
            "_angle_includes_are_system": attr.bool(default = angle_includes_are_system),
            "_verbose": attr.bool(default = verbose),
            "_clang_tidy": attr.label(default = binary, executable = True, cfg = "exec"),
            "_clang_tidy_wrapper": attr.label(
                default = Label("@aspect_rules_lint//lint:clang_tidy_wrapper"),
                executable = True,
                cfg = "exec",
            ),
            "_clang_tidy_driver": attr.label(
                default = Label("//tools/lint:clang_tidy_driver"),
                executable = True,
                cfg = "exec",
            ),
            "_module_rewriter": attr.label(
                default = Label("//tools/lint:module_rewriter"),
                executable = True,
                cfg = "exec",
            ),
            "_cc_toolchain": attr.label(default = Label("@bazel_tools//tools/cpp:current_cc_toolchain")),
            "_rule_kinds": attr.string_list(default = rule_kinds),
            "_args": attr.string_list(default = args),
        },
        toolchains = [
            OPTIONAL_SARIF_PARSER_TOOLCHAIN,
            "@bazel_tools//tools/cpp:toolchain_type",
        ],
        fragments = ["cpp"],
    )
