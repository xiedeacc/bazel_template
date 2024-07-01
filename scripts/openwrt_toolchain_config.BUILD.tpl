# CC toolchain for cc-clang-%{suffix}.
package(default_visibility = ["//visibility:public"])

load("@bazel_skylib//rules:native_binary.bzl", "native_binary")
load("@rules_cc//cc:defs.bzl", "cc_toolchain", "cc_toolchain_suite")
load("@toolchains_llvm//toolchain/internal:system_module_map.bzl", "system_module_map")
load("%{cc_toolchain_config_bzl}", "cc_toolchain_config")

filegroup(
    name = "internal-use-symlinked-tools",
    srcs = [
        %{symlinked_tools}
    ],
    visibility = ["//visibility:private"],
)

filegroup(
    name = "internal-use-wrapped-tools",
    srcs = [
        "bin/cc_wrapper.sh",
    ],
    visibility = ["//visibility:private"],
)

filegroup(
    name = "internal-use-files",
    srcs = [
        ":internal-use-symlinked-tools",
        ":internal-use-wrapped-tools",
    ],
    visibility = ["//visibility:private"],
)

filegroup(
    name = "sysroot-components-%{suffix}",
    srcs = [
        %{sysroot_label_str}
    ],
)

filegroup(
    name = "compiler-components-%{suffix}",
    srcs = [
        "%{toolchain_path_prefix}bin/aarch64-openwrt-linux-musl-cc.bin",
        "%{toolchain_path_prefix}bin/aarch64-openwrt-linux-musl-c++.bin",
        "%{toolchain_path_prefix}bin/aarch64-openwrt-linux-musl-cpp.bin",
        "%{toolchain_path_prefix}bin/aarch64-openwrt-linux-musl-g++.bin",
        "%{toolchain_path_prefix}bin/aarch64-openwrt-linux-musl-gcc.bin",
        "%{toolchain_path_prefix}include",
        "%{toolchain_path_prefix}aarch64-openwrt-linux-musl/include",
        "%{toolchain_path_prefix}aarch64-openwrt-linux-musl/sys-include",
        ":sysroot-components-%{suffix}",
        %{extra_compiler_files}
    ],
)

filegroup(
    name = "linker-components-%{suffix}",
    srcs = [
        "%{toolchain_path_prefix}bin/aarch64-openwrt-linux-musl-ar.bin",
        "%{toolchain_path_prefix}bin/aarch64-openwrt-linux-musl-ld.bin",
        "%{toolchain_path_prefix}lib",
        ":sysroot-components-%{suffix}",
    ],
)

filegroup(
    name = "all-components-%{suffix}",
    srcs = [
        "%{toolchain_path_prefix}bin",
        ":compiler-components-%{suffix}",
        ":linker-components-%{suffix}",
    ],
)

filegroup(name = "all-files-%{suffix}", srcs = [":all-components-%{suffix}", %{extra_files_str}])
filegroup(name = "archiver-files-%{suffix}", srcs = ["%{toolchain_path_prefix}bin/aarch64-openwrt-linux-musl-ar.bin", %{extra_files_str}])
filegroup(name = "assembler-files-%{suffix}", srcs = ["%{toolchain_path_prefix}bin/aarch64-openwrt-linux-musl-as.bin", %{extra_files_str}])
filegroup(name = "compiler-files-%{suffix}", srcs = [":compiler-components-%{suffix}", %{extra_files_str}])
filegroup(name = "linker-files-%{suffix}", srcs = [":linker-components-%{suffix}", %{extra_files_str}])
filegroup(name = "objcopy-files-%{suffix}", srcs = ["%{toolchain_path_prefix}bin/aarch64-openwrt-linux-musl-objcopy.bin", %{extra_files_str}])
filegroup(name = "strip-files-%{suffix}", srcs = ["%{toolchain_path_prefix}bin/aarch64-openwrt-linux-musl-strip.bin", %{extra_files_str}])

filegroup(
    name = "include-components-%{suffix}",
    srcs = [
        ":compiler-components-%{suffix}",
        ":sysroot-components-%{suffix}",
    ],
)

system_module_map(
    name = "module-%{suffix}",
    cxx_builtin_include_files = ":include-components-%{suffix}",
    cxx_builtin_include_directories = %{cxx_builtin_include_directories},
    sysroot_path = "%{sysroot_path}",
)

toolchain(
    name = "cc-toolchain-%{suffix}",
    exec_compatible_with = [
        "@platforms//cpu:x86_64",
        "@platforms//os:linux",
    ],
    target_compatible_with = [
        "@platforms//cpu:%{target_arch}",
        "@platforms//os:linux",
    ],
    target_settings = %{target_settings},
    toolchain = ":gcc-%{suffix}",
    toolchain_type = "@bazel_tools//tools/cpp:toolchain_type",
)

cc_toolchain(
    name = "gcc-%{suffix}",
    all_files = "all-files-%{suffix}",
    ar_files = "archiver-files-%{suffix}",
    as_files = "assembler-files-%{suffix}",
    compiler_files = "compiler-files-%{suffix}",
    linker_files = "linker-files-%{suffix}",
    objcopy_files = "objcopy-files-%{suffix}",
    strip_files = "strip-files-%{suffix}",
    toolchain_config = "local-%{suffix}",
    module_map = "module-%{suffix}",
)

cc_toolchain_config(
    name = "local-%{suffix}",
    toolchain_identifier = "",
    target_system_name = "%{target_system_name}",
    tool_paths = {
        "ar": "{}/bin/aarch64-openwrt-linux-musl-ar".format(toolchain_path_prefix),
        "ld": "{}/bin/aarch64-openwrt-linux-musl-ld.bin".format(toolchain_path_prefix),
        "llvm-cov": "{}/bin/aarch64-openwrt-linux-musl-gcov.bin".format(toolchain_path_prefix),
        "gcov": "{}/bin/aarch64-openwrt-linux-musl-gcov.bin".format(toolchain_path_prefix),
        "cpp": "{}/bin/aarch64-openwrt-linux-musl-cpp.bin".format(toolchain_path_prefix),
        "gcc": "{}/bin/aarch64-openwrt-linux-musl-gcc.bin".format(toolchain_path_prefix),
        "nm": "{}/bin/aarch64-openwrt-linux-musl-nm.bin".format(toolchain_path_prefix),
        "objcopy": "{}/bin/aarch64-openwrt-linux-musl-objcopy.bin".format(toolchain_path_prefix),
        "objdump": "{}/bin/aarch64-openwrt-linux-musl-objdump.bin".format(toolchain_path_prefix),
        "strip": "{}/bin/aarch64-openwrt-linux-musl-strip.bin".format(toolchain_path_prefix),
    }
    compiler_configuration = {
      "compile_flags": %{compile_flags},
      "cxx_flags": %{cxx_flags},
      "link_flags": %{link_flags},
      "archive_flags": %{archive_flags},
      "link_libs": %{link_libs},
      "opt_compile_flags": %{opt_compile_flags},
      "opt_link_flags": %{opt_link_flags},
      "dbg_compile_flags": %{dbg_compile_flags},
      "coverage_compile_flags": %{coverage_compile_flags},
      "coverage_link_flags": %{coverage_link_flags},
      "unfiltered_compile_flags": %{unfiltered_compile_flags},
    },
    sysroot_path,
    cxx_builtin_include_directories = %{cxx_builtin_include_directories},
)
