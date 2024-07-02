load(
    "@bazel_template//scripts:openwrt_toolchain_config_impl.bzl",
    _is_absolute_path = "is_absolute_path",
    _openwrt_toolchain_config_impl = "openwrt_toolchain_config_impl",
)
load(
    "@bazel_template//scripts:openwrt_toolchain_repo_impl.bzl",
    _openwrt_toolchain_repo_impl = "openwrt_toolchain_repo_impl",
)

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
    aargs = dict()
    for chip_model, chip_model_info in toolchains.items():
        for chip_version, chip_version_info in chip_model_info.items():
            if not chip_version_info.get("url"):
                fail("must have url")
            if not chip_version_info.get("arch"):
                fail("must have arch")
            if not _is_absolute_path(chip_version_info.get("url")) and not chip_version_info.get("sha256sum"):
                fail("must have sha256sum unless url is a absolute path")
            aargs["name"] = "{}_{}".format(chip_model, chip_version)
            aargs["url"] = chip_version_info.get("url")
            aargs["arch"] = chip_version_info.get("arch")
            aargs["sha256sum"] = chip_version_info.get("sha256sum")

            #openwrt_toolchain_repo(**aargs)
    openwrt_toolchain_config(name = "_test", **aargs)
