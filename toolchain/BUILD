load("@rules_cc//cc:defs.bzl", "cc_toolchain", "cc_toolchain_suite")
load(":clang_linux_aarch64_cc_toolchain_config.bzl", clang_linux_aarch64_cc_toolchain_config = "cc_toolchain_config")
#load(":gcc_linux_aarch64_cc_toolchain_config.bzl", gcc_linux_aarch64_cc_toolchain_config = "cc_toolchain_config")

load(":clang_linux_x86_64_cc_toolchain_config.bzl", clang_linux_x86_64_cc_toolchain_config = "cc_toolchain_config")

package(default_visibility = ["//visibility:public"])

filegroup(name = "empty")

platform(
    name = "aarch64_target",
    constraint_values = [
        "@platforms//cpu:aarch64",
        "@platforms//os:linux",
    ],
)

######### clang_linux_x86_64 #########
toolchain(
    name = "clang_toolchain_for_linux_x86_64",
    exec_compatible_with = [
        "@platforms//cpu:x86_64",
        "@platforms//os:linux",
    ],
    target_compatible_with = [
        "@platforms//cpu:x86_64",
        "@platforms//os:linux",
    ],
    toolchain = ":clang_linux_x86_64_toolchain",
    toolchain_type = "@bazel_tools//tools/cpp:toolchain_type",
)

cc_toolchain(
    name = "clang_linux_x86_64_toolchain",
    all_files = ":empty",
    compiler_files = ":empty",
    dwp_files = ":empty",
    linker_files = ":empty",
    objcopy_files = ":empty",
    strip_files = ":empty",
    supports_param_files = 0,
    toolchain_config = ":clang_linux_x86_64_toolchain_config",
    toolchain_identifier = "clang_linux_x86_64_toolchain",
)

clang_linux_x86_64_toolchain_root = "/usr/local/llvm/18"

clang_linux_x86_64_cc_toolchain_config(
    name = "clang_linux_x86_64_toolchain_config",
    abi_libc_version = "local",
    abi_version = "local",
    compile_flags = [
        "-fstack-protector",
        "-Wall",
        "-Wunused-but-set-parameter",
        "-Wno-free-nonheap-object",
        "-fno-omit-frame-pointer",
    ],
    compiler = "clang",
    conly_flags = [],
    coverage_compile_flags = ["--coverage"],
    coverage_link_flags = ["--coverage"],
    cpu = "k8",
    cxx_builtin_include_directories = [
        "{}/include/x86_64-unknown-linux-gnu/c++/v1".format(clang_linux_x86_64_toolchain_root),
        "{}/include/c++/v1".format(clang_linux_x86_64_toolchain_root),
        "{}/lib/clang/18/include".format(clang_linux_x86_64_toolchain_root),
        "/usr/local/include",
        "/usr/include/x86_64-linux-gnu",
        "/usr/include",
    ],
    cxx_flags = ["-std=c++17"],
    dbg_compile_flags = ["-g"],
    host_system_name = "local",
    link_flags = [
        "-fuse-ld=gold",
        "-B{}/bin".format(clang_linux_x86_64_toolchain_root),
        "-Wl,-no-as-needed",
        "-Wl,-z,relro,-z,now",
    ],
    link_libs = [
        "-Wl,--push-state,-as-needed",
        "-lstdc++",
        "-Wl,--pop-state",
        "-Wl,--push-state,-as-needed",
        "-lm",
        "-Wl,--pop-state",
    ],
    opt_compile_flags = [
        "-g0",
        "-O2",
        "-D_FORTIFY_SOURCE=1",
        "-DNDEBUG",
        "-ffunction-sections",
        "-fdata-sections",
    ],
    opt_link_flags = ["-Wl,--gc-sections"],
    supports_start_end_lib = True,
    target_libc = "local",
    target_system_name = "local",
    tool_paths = {
        "ar": "{}/bin/llvm-ar".format(clang_linux_x86_64_toolchain_root),
        "ld": "{}/bin/lld".format(clang_linux_x86_64_toolchain_root),
        "llvm-cov": "{}/bin/llvm-cov".format(clang_linux_x86_64_toolchain_root),
        "llvm-profdata": "{}/bin/llvm-profdata".format(clang_linux_x86_64_toolchain_root),
        "cpp": "{}/bin/clang++".format(clang_linux_x86_64_toolchain_root),
        "clang++": "{}/bin/clang++".format(clang_linux_x86_64_toolchain_root),
        "gcc": "{}/bin/clang".format(clang_linux_x86_64_toolchain_root),
        "clang": "{}/bin/clang".format(clang_linux_x86_64_toolchain_root),
        "dwp": "{}/bin/llvm-dwp".format(clang_linux_x86_64_toolchain_root),
        "gcov": "{}/bin/llvm-cov".format(clang_linux_x86_64_toolchain_root),
        "nm": "{}/bin/llvm-nm".format(clang_linux_x86_64_toolchain_root),
        "objcopy": "{}/bin/llvm-objcopy".format(clang_linux_x86_64_toolchain_root),
        "objdump": "{}/bin/llvm-objdump".format(clang_linux_x86_64_toolchain_root),
        "strip": "{}/bin/llvm-strip".format(clang_linux_x86_64_toolchain_root),
    },
    toolchain_identifier = "local",
    unfiltered_compile_flags = [
        "-Wno-builtin-macro-redefined",
        "-D__DATE__=\"redacted\"",
        "-D__TIMESTAMP__=\"redacted\"",
        "-D__TIME__=\"redacted\"",
    ],
)

######### clang_linux_aarch64 #########
toolchain(
    name = "clang_toolchain_for_linux_aarch64",
    exec_compatible_with = [
        "@platforms//cpu:x86_64",
        "@platforms//os:linux",
    ],
    target_compatible_with = [
        "@platforms//cpu:aarch64",
        "@platforms//os:linux",
    ],
    toolchain = ":clang_linux_aarch64_toolchain",
    toolchain_type = "@bazel_tools//tools/cpp:toolchain_type",
)

cc_toolchain(
    name = "clang_linux_aarch64_toolchain",
    all_files = ":empty",
    compiler_files = ":empty",
    dwp_files = ":empty",
    linker_files = ":empty",
    objcopy_files = ":empty",
    strip_files = ":empty",
    supports_param_files = 0,
    toolchain_config = ":clang_linux_aarch64_toolchain_config",
    toolchain_identifier = "clang_linux_aarch64_toolchain",
)

clang_linux_aarch64_toolchain_root = "/usr/local/llvm/18"

clang_linux_aarch64_cc_toolchain_config(
    name = "clang_linux_aarch64_toolchain_config",
    abi_libc_version = "glibc",
    abi_version = "clang",
    compile_flags = [
        "-fstack-protector",
        "-Wall",
        "-Wunused-but-set-parameter",
        "-Wno-free-nonheap-object",
        "-fno-omit-frame-pointer",
    ],
    compiler = "clang",
    conly_flags = [],
    coverage_compile_flags = ["--coverage"],
    coverage_link_flags = ["--coverage"],
    cpu = "aarch64",
    cxx_builtin_include_directories = [
        "{}/include/x86_64-unknown-linux-gnu/c++/v1".format(clang_linux_aarch64_toolchain_root),
        "{}/include/c++/v1".format(clang_linux_aarch64_toolchain_root),
        "{}/lib/clang/18/include".format(clang_linux_aarch64_toolchain_root),
    ],
    cxx_flags = ["-std=c++17"],
    dbg_compile_flags = ["-g"],
    host_system_name = "local",
    link_flags = [
        "-fuse-ld=gold",
        "-B{}/bin".format(clang_linux_aarch64_toolchain_root),
        "-Wl,-no-as-needed",
        "-Wl,-z,relro,-z,now",
    ],
    link_libs = [
        "-Wl,--push-state,-as-needed",
        "-lstdc++",
        "-Wl,--pop-state",
        "-Wl,--push-state,-as-needed",
        "-lm",
        "-Wl,--pop-state",
    ],
    opt_compile_flags = [
        "-g0",
        "-O2",
        "-D_FORTIFY_SOURCE=1",
        "-DNDEBUG",
        "-ffunction-sections",
        "-fdata-sections",
    ],
    opt_link_flags = ["-Wl,--gc-sections"],
    supports_start_end_lib = True,
    target_libc = "local",
    target_system_name = "local",
    tool_paths = {
        "ar": "{}/bin/llvm-ar".format(clang_linux_aarch64_toolchain_root),
        "ld": "{}/bin/lld".format(clang_linux_aarch64_toolchain_root),
        "llvm-cov": "{}/bin/llvm-cov".format(clang_linux_aarch64_toolchain_root),
        "llvm-profdata": "{}/bin/llvm-profdata".format(clang_linux_aarch64_toolchain_root),
        "cpp": "{}/bin/clang++".format(clang_linux_aarch64_toolchain_root),
        "clang++": "{}/bin/clang++".format(clang_linux_aarch64_toolchain_root),
        "gcc": "{}/bin/clang".format(clang_linux_aarch64_toolchain_root),
        "clang": "{}/bin/clang".format(clang_linux_aarch64_toolchain_root),
        "dwp": "{}/bin/llvm-dwp".format(clang_linux_aarch64_toolchain_root),
        "gcov": "{}/bin/llvm-cov".format(clang_linux_aarch64_toolchain_root),
        "nm": "{}/bin/llvm-nm".format(clang_linux_aarch64_toolchain_root),
        "objcopy": "{}/bin/llvm-objcopy".format(clang_linux_aarch64_toolchain_root),
        "objdump": "{}/bin/llvm-objdump".format(clang_linux_aarch64_toolchain_root),
        "strip": "{}/bin/llvm-strip".format(clang_linux_aarch64_toolchain_root),
    },
    toolchain_identifier = "local",
    unfiltered_compile_flags = [
        "-Wno-builtin-macro-redefined",
        "-D__DATE__=\"redacted\"",
        "-D__TIMESTAMP__=\"redacted\"",
        "-D__TIME__=\"redacted\"",
    ],
)

########## gcc_linux_x86_64 #########
#toolchain(
#name = "gcc_toolchain_for_linux_x86_64",
#exec_compatible_with = [
#"@platforms//cpu:x86_64",
#"@platforms//os:linux",
#],
#target_compatible_with = [
#"@platforms//cpu:x86_64",
#"@platforms//os:linux",
#],
#toolchain = ":gcc_linux_x86_64_toolchain",
#toolchain_type = "@bazel_tools//tools/cpp:toolchain_type",
#)

#cc_toolchain(
#name = "gcc_linux_x86_64_toolchain",
#all_files = ":empty",
#compiler_files = ":empty",
#dwp_files = ":empty",
#linker_files = ":empty",
#objcopy_files = ":empty",
#strip_files = ":empty",
#supports_param_files = 0,
#toolchain_config = ":gcc_linux_x86_64_toolchain_config",
#toolchain_identifier = "gcc_linux_x86_64_toolchain",
#)

#gcc_linux_x86_64_toolchain_root = "/usr/local/gcc/14"

#gcc_linux_x86_64_cc_toolchain_config(
#name = "gcc_linux_x86_64_toolchain_config",
#abi_libc_version = "local",
#abi_version = "local",
#builtin_sysroot = gcc_linux_x86_64_toolchain_root,
#compile_flags = [
#"-fstack-protector",
#"-Wall",
#"-Wunused-but-set-parameter",
#"-Wno-free-nonheap-object",
#"-fno-omit-frame-pointer",
#],
#compiler = "gcc",
#conly_flags = [],
#coverage_compile_flags = ["--coverage"],
#coverage_link_flags = ["--coverage"],
#cpu = "k8",
#cxx_builtin_include_directories = [
#"{}/include".format(gcc_linux_x86_64_toolchain_root),
#"{}/x86_64-openwrt-linux-musl/include/c++/12.3.0".format(gcc_linux_x86_64_toolchain_root),
#"{}/x86_64-openwrt-linux-musl/sys-include".format(gcc_linux_x86_64_toolchain_root),
#],
#cxx_flags = ["-std=c++17"],
#dbg_compile_flags = ["-g"],
#host_system_name = "local",
#link_flags = [
#"-L{}/lib".format(gcc_linux_x86_64_toolchain_root),
#"-B{}/x86_64-openwrt-linux-musl/bin".format(gcc_linux_x86_64_toolchain_root),
#"-Wl,-no-as-needed",
#"-Wl,-z,relro,-z,now",
#"-pass-exit-codes",
#],
#link_libs = [
#"-Wl,--push-state,-as-needed",
#"-lstdc++",
#"-Wl,--pop-state",
#"-Wl,--push-state,-as-needed",
#"-lm",
#"-Wl,--pop-state",
#],
#opt_compile_flags = [
#"-g0",
#"-O2",
#"-D_FORTIFY_SOURCE=1",
#"-DNDEBUG",
#"-ffunction-sections",
#"-fdata-sections",
#],
#opt_link_flags = ["-Wl,--gc-sections"],
#supports_start_end_lib = False,
#target_libc = "local",
#target_system_name = "local",
#tool_paths = {
#"ar": "{}/bin/ar".format(gcc_linux_x86_64_toolchain_root),
#"ld": "{}/bin/ld".format(gcc_linux_x86_64_toolchain_root),
#"llvm-cov": "{}/bin/gcov".format(gcc_linux_x86_64_toolchain_root),
#"gcov": "{}/bin/gcov".format(gcc_linux_x86_64_toolchain_root),
#"cpp": "{}/bin/cpp".format(gcc_linux_x86_64_toolchain_root),
#"gcc": "{}/bin/gcc".format(gcc_linux_x86_64_toolchain_root),
#"nm": "{}/bin/nm".format(gcc_linux_x86_64_toolchain_root),
#"objcopy": "{}/bin/objcopy".format(gcc_linux_x86_64_toolchain_root),
#"objdump": "{}/bin/objdump".format(gcc_linux_x86_64_toolchain_root),
#"strip": "{}/bin/strip".format(gcc_linux_x86_64_toolchain_root),
#},
#toolchain_identifier = "local",
#unfiltered_compile_flags = [
#"-fno-canonical-system-headers",
#"-Wno-builtin-macro-redefined",
#"-D__DATE__=\"redacted\"",
#"-D__TIMESTAMP__=\"redacted\"",
#"-D__TIME__=\"redacted\"",
#],
#)

########## gcc_linux_aarch64 #########
#toolchain(
#name = "gcc_toolchain_for_linux_aarch64",
#exec_compatible_with = [
#"@platforms//cpu:x86_64",
#"@platforms//os:linux",
#],
#target_compatible_with = [
#"@platforms//cpu:aarch64",
#"@platforms//os:linux",
#],
#toolchain = ":gcc_linux_aarch64_toolchain",
#toolchain_type = "@bazel_tools//tools/cpp:toolchain_type",
#)

#cc_toolchain(
#name = "gcc_linux_aarch64_toolchain",
#all_files = ":empty",
#compiler_files = ":empty",
#dwp_files = ":empty",
#linker_files = ":empty",
#objcopy_files = ":empty",
#strip_files = ":empty",
#supports_param_files = 0,
#toolchain_config = ":gcc_linux_aarch64_toolchain_config",
#toolchain_identifier = "gcc_linux_aarch64_toolchain",
#)

#gcc_linux_aarch64_toolchain_root = "/usr/local/gcc/14"

#gcc_linux_aarch64_cc_toolchain_config(
#name = "gcc_linux_aarch64_toolchain_config",
#abi_libc_version = "local",
#abi_version = "local",
#builtin_sysroot = gcc_linux_aarch64_toolchain_root,
#compile_flags = [
#"-fstack-protector",
#"-Wall",
#"-Wunused-but-set-parameter",
#"-Wno-free-nonheap-object",
#"-fno-omit-frame-pointer",
#],
#compiler = "gcc",
#conly_flags = [],
#coverage_compile_flags = ["--coverage"],
#coverage_link_flags = ["--coverage"],
#cpu = "k8",
#cxx_builtin_include_directories = [
#"{}/include".format(gcc_linux_aarch64_toolchain_root),
#"{}/include/c++/12.3.0".format(gcc_linux_aarch64_toolchain_root),
#],
#cxx_flags = ["-std=c++17"],
#dbg_compile_flags = ["-g"],
#host_system_name = "local",
#link_flags = [
#"-L{}/lib".format(gcc_linux_aarch64_toolchain_root),
#"-B{}/bin".format(gcc_linux_aarch64_toolchain_root),
#"-Wl,-no-as-needed",
#"-Wl,-z,relro,-z,now",
#"-pass-exit-codes",
#],
#link_libs = [
#"-Wl,--push-state,-as-needed",
#"-lstdc++",
#"-Wl,--pop-state",
#"-Wl,--push-state,-as-needed",
#"-lm",
#"-Wl,--pop-state",
#],
#opt_compile_flags = [
#"-g0",
#"-O2",
#"-D_FORTIFY_SOURCE=1",
#"-DNDEBUG",
#"-ffunction-sections",
#"-fdata-sections",
#],
#opt_link_flags = ["-Wl,--gc-sections"],
#supports_start_end_lib = False,
#target_libc = "local",
#target_system_name = "local",
#tool_paths = {
#"ar": "{}/bin/ar".format(gcc_linux_aarch64_toolchain_root),
#"ld": "{}/bin/ld".format(gcc_linux_aarch64_toolchain_root),
#"llvm-cov": "{}/bin/gcov".format(gcc_linux_aarch64_toolchain_root),
#"gcov": "{}/bin/gcov".format(gcc_linux_aarch64_toolchain_root),
#"cpp": "{}/bin/cpp".format(gcc_linux_aarch64_toolchain_root),
#"gcc": "{}/bin/gcc".format(gcc_linux_aarch64_toolchain_root),
#"nm": "{}/bin/nm".format(gcc_linux_aarch64_toolchain_root),
#"objcopy": "{}/bin/objcopy".format(gcc_linux_aarch64_toolchain_root),
#"objdump": "{}/bin/objdump".format(gcc_linux_aarch64_toolchain_root),
#"strip": "{}/bin/strip".format(gcc_linux_aarch64_toolchain_root),
#},
#toolchain_identifier = "local",
#unfiltered_compile_flags = [
#"-fno-canonical-system-headers",
#"-Wno-builtin-macro-redefined",
#"-D__DATE__=\"redacted\"",
#"-D__TIMESTAMP__=\"redacted\"",
#"-D__TIME__=\"redacted\"",
#],
#)

########## gcc_openwrt_aarch64 #########
#gcc_openwrt_aarch64_toolchain_root = "/root/src/software/openwrt/openwrt-toolchain-23.05.3-rockchip-armv8_gcc-12.3.0_musl.Linux-x86_64/toolchain-aarch64_generic_gcc-12.3.0_musl"

#gcc_linux_aarch64_cc_toolchain_config(
#name = "gcc_openwrt_aarch64_toolchain_config",
#abi_libc_version = "local",
#abi_version = "local",
#builtin_sysroot = gcc_openwrt_aarch64_toolchain_root,
#compile_flags = [
#"-fstack-protector",
#"-Wall",
#"-Wunused-but-set-parameter",
#"-Wno-free-nonheap-object",
#"-fno-omit-frame-pointer",
#],
#compiler = "gcc",
#conly_flags = [],
#coverage_compile_flags = ["--coverage"],
#coverage_link_flags = ["--coverage"],
#cpu = "k8",
#cxx_builtin_include_directories = [
#"{}/include".format(gcc_openwrt_aarch64_toolchain_root),
#"{}/aarch64-openwrt-linux-musl/include/c++/12.3.0".format(gcc_openwrt_aarch64_toolchain_root),
#"{}/aarch64-openwrt-linux-musl/sys-include".format(gcc_openwrt_aarch64_toolchain_root),
#],
#cxx_flags = ["-std=c++17"],
#dbg_compile_flags = ["-g"],
#host_system_name = "local",
#link_flags = [
#"-L{}/lib".format(gcc_openwrt_aarch64_toolchain_root),
#"-B{}/aarch64-openwrt-linux-musl/bin".format(gcc_openwrt_aarch64_toolchain_root),
#"-Wl,-no-as-needed",
#"-Wl,-z,relro,-z,now",
#"-pass-exit-codes",
#],
#link_libs = [
#"-Wl,--push-state,-as-needed",
#"-lstdc++",
#"-Wl,--pop-state",
#"-Wl,--push-state,-as-needed",
#"-lm",
#"-Wl,--pop-state",
#],
#opt_compile_flags = [
#"-g0",
#"-O2",
#"-D_FORTIFY_SOURCE=1",
#"-DNDEBUG",
#"-ffunction-sections",
#"-fdata-sections",
#],
#opt_link_flags = ["-Wl,--gc-sections"],
#supports_start_end_lib = False,
#target_libc = "local",
#target_system_name = "local",
#tool_paths = {
#"ar": "{}/bin/aarch64-openwrt-linux-musl-ar".format(gcc_openwrt_aarch64_toolchain_root),
#"ld": "{}/bin/aarch64-openwrt-linux-musl-ld.bin".format(gcc_openwrt_aarch64_toolchain_root),
#"llvm-cov": "{}/bin/aarch64-openwrt-linux-musl-gcov.bin".format(gcc_openwrt_aarch64_toolchain_root),
#"gcov": "{}/bin/aarch64-openwrt-linux-musl-gcov.bin".format(gcc_openwrt_aarch64_toolchain_root),
#"cpp": "{}/bin/aarch64-openwrt-linux-musl-cpp.bin".format(gcc_openwrt_aarch64_toolchain_root),
#"gcc": "{}/bin/aarch64-openwrt-linux-musl-gcc.bin".format(gcc_openwrt_aarch64_toolchain_root),
#"nm": "{}/bin/aarch64-openwrt-linux-musl-nm.bin".format(gcc_openwrt_aarch64_toolchain_root),
#"objcopy": "{}/bin/aarch64-openwrt-linux-musl-objcopy.bin".format(gcc_openwrt_aarch64_toolchain_root),
#"objdump": "{}/bin/aarch64-openwrt-linux-musl-objdump.bin".format(gcc_openwrt_aarch64_toolchain_root),
#"strip": "{}/bin/aarch64-openwrt-linux-musl-strip.bin".format(gcc_openwrt_aarch64_toolchain_root),
#},
#toolchain_identifier = "local",
#unfiltered_compile_flags = [
#"-fno-canonical-system-headers",
#"-Wno-builtin-macro-redefined",
#"-D__DATE__=\"redacted\"",
#"-D__TIMESTAMP__=\"redacted\"",
#"-D__TIME__=\"redacted\"",
#],
#)
