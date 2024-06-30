def _is_absolute_path(path):
    return val and val[0] == "/" and (len(val) == 1 or val[1] != "/")

def _download_openwrt(rctx):
    urls = [rctx.attr.url]
    res = rctx.download_and_extract(
        urls,
        sha256sum = rctx.attr.sha256sum,
        stripPrefix = rctx.attr.strip_prefix,
    )
    if rctx.attr.sha256sum != res.sha256:
        fail("need sha256sum:{}, but get:{}".format(rctx.attr.sha256sum, res.sha256))
    print(res.sha256)
    return rctx.attr

def _openwrt_toolchain_repo_impl(rctx):
    rctx.file(
        "BUILD.bazel",
        content = rctx.read(Label("//scripts:BUILD.openwrt_repo")),
        executable = False,
    )
    if _is_absolute_path(rctx.attr.url):
        return rctx.aatr

    updated_attrs = _download_openwrt(rctx)
    return updated_attrs

openwrt_toolchain_repo = repository_rule(
    attrs = {
        "name": attr.string(
            mandatory = True,
        ),
        "arch": attr.string(
            mandatory = True,
        ),
        "url": attr.string(
            mandatory = True,
        ),
        "sha256sum": attr.string(
            mandatory = True,
        ),
    },
    local = False,
    implementation = _openwrt_toolchain_repo_impl,
)

def _canonical_dir_path(path):
    if not path.endswith("/"):
        return path + "/"
    return path

def _pkg_path_from_label(label):
    if label.workspace_root:
        return label.workspace_root + "/" + label.package
    else:
        return label.package

def _openwrt_toolchain_config_impl(rctx):
    system_llvm = False
    if _is_absolute_path(toolchain_root):
        system_llvm = True
        toolchain_path_prefix = _canonical_dir_path(toolchain_root)
    if not system_llvm:
        toolchain_root = ("@" if BZLMOD_ENABLED else "") + "@openwrt_toolchain_repo_%s//" % rctx.attr.name
    print(toolchain_root)

    if system_llvm:
        toolchain_path_prefix = _canonical_dir_path(toolchain_root)
    else:
        llvm_dist_label = Label(toolchain_root + ":BUILD.bazel")  # Exact target does not matter.
        toolchain_path_prefix = _canonical_dir_path(str(rctx.path(llvm_dist_label).dirname))
        #toolchain_path_prefix = _pkg_path_from_label(llvm_dist_label)

    llvm_dist_rel_path = toolchain_path_prefix
    llvm_dist_label_prefix = toolchain_path_prefix
    wrapper_bin_prefix = "bin/"
    tools_path_prefix = toolchain_path_prefix + "bin/"
    symlinked_tools_str = ""

    workspace_name = rctx.name
    toolchain_info = struct(
        os = os,
        arch = arch,
        llvm_dist_label_prefix = llvm_dist_label_prefix,
        toolchain_path_prefix = toolchain_path_prefix,
        tools_path_prefix = tools_path_prefix,
        wrapper_bin_prefix = wrapper_bin_prefix,
        sysroot_paths_dict = sysroot_paths_dict,
        sysroot_labels_dict = sysroot_labels_dict,
        target_settings_dict = rctx.attr.target_settings,
        additional_include_dirs_dict = rctx.attr.cxx_builtin_include_directories,
        stdlib_dict = rctx.attr.stdlib,
        cxx_standard_dict = rctx.attr.cxx_standard,
        compile_flags_dict = rctx.attr.compile_flags,
        cxx_flags_dict = rctx.attr.cxx_flags,
        link_flags_dict = rctx.attr.link_flags,
        archive_flags_dict = rctx.attr.archive_flags,
        link_libs_dict = rctx.attr.link_libs,
        opt_compile_flags_dict = rctx.attr.opt_compile_flags,
        opt_link_flags_dict = rctx.attr.opt_link_flags,
        dbg_compile_flags_dict = rctx.attr.dbg_compile_flags,
        coverage_compile_flags_dict = rctx.attr.coverage_compile_flags,
        coverage_link_flags_dict = rctx.attr.coverage_link_flags,
        unfiltered_compile_flags_dict = rctx.attr.unfiltered_compile_flags,
        llvm_version = llvm_version,
        extra_compiler_files = rctx.attr.extra_compiler_files,
    )

   hcc_toolchains_str, toolchain_labels_str = _cc_toolchains_str(
        rctx,
        workspace_name,
        toolchain_info,
        use_absolute_paths_llvm,
    )

    convenience_targets_str = _convenience_targets_str(
        rctx,
        use_absolute_paths_llvm,
        llvm_dist_rel_path,
        llvm_dist_label_prefix,
        exec_dl_ext,
    )

    # Convenience macro to register all generated toolchains.
    rctx.template(
        "toolchains.bzl",
        rctx.attr._toolchains_bzl_tpl,
        {
            "%{toolchain_labels}": toolchain_labels_str,
        },
    )

    # BUILD file with all the generated toolchain definitions.
    rctx.template(
        "BUILD.bazel",
        rctx.attr._build_toolchain_tpl,
        {
            "%{cc_toolchain_config_bzl}": str(rctx.attr._cc_toolchain_config_bzl),
            "%{cc_toolchains}": cc_toolchains_str,
            "%{symlinked_tools}": symlinked_tools_str,
            "%{wrapper_bin_prefix}": wrapper_bin_prefix,
            "%{convenience_targets}": convenience_targets_str,
        },
    )

    # CC wrapper script; see comments near the definition of `wrapper_bin_prefix`.
    if os == "darwin":
        cc_wrapper_tpl = rctx.attr._darwin_cc_wrapper_sh_tpl
    else:
        cc_wrapper_tpl = rctx.attr._cc_wrapper_sh_tpl
    rctx.template(
        "bin/cc_wrapper.sh",
        cc_wrapper_tpl,
        {
            "%{toolchain_path_prefix}": toolchain_path_prefix,
        },
    )

openwrt_toolchain_config = repository_rule(
    attrs = {
        "name": attr.string(
            mandatory = True,
        ),
        "arch": attr.string(
            mandatory = True,
        ),
        "url": attr.string(
            mandatory = True,
        ),
        "sha256sum": attr.string(
            mandatory = True,
        ),
    },
    local = True,
    configure = True,
    implementation = _openwrt_toolchain_config_impl,
)

def openwrt_toolchain_setup(name, **kwargs):
    if not kwargs.get("toolchains"):
        fail("must set toolchains")
    toolchains = wargs.get("toolchains")
    for chip_model, chip_model_info in toolchains.items():
        for chip_version, chip_version_info in chip_model_info:
            if not chip_version_info.get("url"):
                fail("must have url")
            if not chip_version_info.get("arch"):
                fail("must have arch")
            if not _is_absolute_path(chip_version_info.get("url")) and not chip_version_info.get("sha256sum"):
                fail("must have sha256sum unless url is a absolute path")
            attr = {}
            attr["name"] = "{}_{}".format(chip_model, chip_version)
            atrr["url"] = chip_version_info.get("url")
            atrr["arch"] = chip_version_info.get("arch")
            atrr["sha256sum"] = chip_version_info.get("sha256sum")
            openwrt_toolchain_repo(name = "openwrt_toolchain_repo_{}_{}".format(chip_model, chip_version), **atrr)
            openwrt_toolchain_config(name = "openwrt_toolchain_config_{}_{}".format(chip_model, chip_version), **atrr)
