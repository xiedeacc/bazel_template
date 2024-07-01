BZLMOD_ENABLED = "@@" in str(Label("//:unused"))

def _is_absolute_path(path):
    return path and path[0] == "/" and (len(path) == 1 or path[1] != "/")

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
    print(rctx.attr)
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
    implementation = _openwrt_toolchain_repo_impl,
    attrs = {
        "arch": attr.string(mandatory = True),
        "url": attr.string(mandatory = True),
        "sha256sum": attr.string(mandatory = True),
    },
    local = False,
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

def _os(rctx):
    name = rctx.os.name
    if name == "linux":
        return "linux"
    elif name == "mac os x":
        return "darwin"
    elif name.startswith("windows"):
        return "windows"
    fail("Unsupported OS: " + name)

def _arch(rctx):
    arch = rctx.os.arch
    if arch == "arm64":
        return "aarch64"
    if arch == "amd64":
        return "x86_64"
    return arch

def _os_bzl(os):
    return {"darwin": "osx", "linux": "linux"}[os]

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
        toolchain_repo_label = Label(toolchain_root + ":BUILD.bazel")  # Exact target does not matter.
        toolchain_path_prefix = _canonical_dir_path(str(rctx.path(toolchain_repo_label).dirname))
        #toolchain_path_prefix = _pkg_path_from_label(llvm_dist_label)

    print(rctx.name)
    print(toolchain_path_prefix)
    rctx.xxx()
    #suffix
    #cc_toolchain_config_bzl = "@bazel_template//scripts:openwrt_cc_toolchain_config.bzl",
    #symlinked_tools
    ##sysroot_label_str = "\"%s\"" % str(sysroot_label)
    #sysroot_label_str = ""
    #extra_compiler_files
    #extra_files_str

    ##extra_files_str = "\":internal-use-files\""
    #toolchain_path_prefix = toolchain_info.toolchain_path_prefix
    #cxx_builtin_include_directories = [
    #toolchain_path_prefix + "include",
    #toolchain_path_prefix + "aarch64-openwrt-linux-musl/include/c++/12.3.0",
    #toolchain_path_prefix + "aarch64-openwrt-linux-musl/sys-include",
    #]

    #sysroot_path = ""
    #sysroot_path = toolchain_info.toolchain_path_prefix
    #sysroot_prefix = "%sysroot%"
    #cxx_builtin_include_directories.extend([
    #_join(sysroot_prefix, "/include"),
    #_join(sysroot_prefix, "/usr/include"),
    #_join(sysroot_prefix, "/usr/local/include"),
    #])

    #target_arch
    #workspace_name = rctx.name
    #target_settings = ""
    #target_system_name= "aarch64-linux"
    ##target_system_name= "armeabi-linux"
    ##target_system_name= "armv7a-linux"
    ##target_system_name= "armv7-linux"
    #compile_flags
    #cxx_flags
    #link_flags
    #archive_flags
    #link_libs
    #opt_compile_flags
    #opt_link_flags
    #dbg_compile_flags
    #coverage_compile_flags
    #coverage_link_flags
    #unfiltered_compile_flags

    #filenames = []
    #for libname in _aliased_libs:
    #filename = "lib/{}.{}".format(libname, exec_dl_ext)
    #filenames.append(filename)
    #for toolname in _aliased_tools:
    #filename = "bin/{}".format(toolname)
    #filenames.append(filename)

    #for filename in filenames:
    #rctx.symlink(llvm_dist_rel_path + filename, filename)

    #rctx.template(
    #"BUILD.bazel",
    #rctx.attr._build_toolchain_tpl,
    #{
    #"%{suffix}": suffix,
    #"%{cc_toolchain_config_bzl}": cc_toolchain_config_bzl,
    #"%{target_settings}": target_settings,
    #"%{target_os_bzl}": target_os_bzl,
    #"%{sysroot_label_str}": sysroot_label_str,
    #"%{sysroot_path}": sysroot_path,
    #"%{toolchain_path_prefix}": toolchain_path_prefix,
    #"%{cxx_builtin_include_directories}": cxx_builtin_include_directories,
    #"%{cc_toolchain_config_bzl}": str(rctx.attr._cc_toolchain_config_bzl),
    #"%{cc_toolchains}": cc_toolchains_str,
    #"%{symlinked_tools}": symlinked_tools_str,
    #"%{wrapper_bin_prefix}": wrapper_bin_prefix,
    #"%{convenience_targets}": convenience_targets_str,
    #},
    #)

openwrt_toolchain_config = repository_rule(
    attrs = {
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
    toolchains = kwargs.get("toolchains")
    for chip_model, chip_model_info in toolchains.items():
        for chip_version, chip_version_info in chip_model_info.items():
            if not chip_version_info.get("url"):
                fail("must have url")
            if not chip_version_info.get("arch"):
                fail("must have arch")
            if not _is_absolute_path(chip_version_info.get("url")) and not chip_version_info.get("sha256sum"):
                fail("must have sha256sum unless url is a absolute path")
            aargs = dict()
            aargs["name"] = "{}_{}".format(chip_model, chip_version)
            aargs["url"] = chip_version_info.get("url")
            aargs["arch"] = chip_version_info.get("arch")
            aargs["sha256sum"] = chip_version_info.get("sha256sum")
            openwrt_toolchain_repo(**aargs)
            openwrt_toolchain_config(**aargs)
