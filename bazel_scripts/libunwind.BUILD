load("@bazel_template//bazel_scripts:common.bzl", "extract_symbols", "template_rule")

package(default_visibility = ["//visibility:public"])

# NOTICE: header sequence refer XXX.Plo, Plo file generated by make
config_setting(
    name = "linux_x86_64",
    constraint_values = [
        "@platforms//os:linux",
        "@platforms//cpu:x86_64",
    ],
)

config_setting(
    name = "linux_aarch64",
    constraint_values = [
        "@platforms//os:linux",
        "@platforms//cpu:aarch64",
    ],
)

COPTS = [
    "-g",
    "-O3",
    "-fPIC",
    "-fexceptions",
    "-Wall",
    "-Wsign-compare",
    "-isystem external/zlib",
    "-Iexternal/libunwind/src",
    "-I$(BINDIR)/external/libunwind/include",
    "-Iexternal/libunwind/include",
    "-I$(BINDIR)/external/libunwind/include/tdep",
    "-Iexternal/libunwind/include/tdep",
    "-Iexternal/libunwind/src/mi",
]

LINUX_X86_64_COPTS = [
    "-Iexternal/libunwind/src/x86_64",
    #"-Iexternal/libunwind/include",
]

LINUX_AARCH64_COPTS = [
    "-Iexternal/libunwind/src/aarch64",
    #"-Iexternal/libunwind/include",
]

LOCAL_DEFINES = [
    "HAVE_CONFIG_H",
    "_GNU_SOURCE",
    "NDEBUG",
    "__EXTENSIONS__",
]

template_rule(
    name = "libunwind_h",
    src = "include/libunwind.h.in",
    out = "include/libunwind.h",
    substitutions = select({
        ":linux_x86_64": {
            "@arch@": "x86_64",
        },
        ":linux_aarch64": {
            "@arch@": "aarch64",
        },
    }),
)

template_rule(
    name = "tdep_libunwind_i_h",
    src = "include/tdep/libunwind_i.h.in",
    out = "include/tdep/libunwind_i.h",
    substitutions = select({
        ":linux_x86_64": {
            "@arch@": "x86_64",
        },
        ":linux_aarch64": {
            "@arch@": "aarch64",
        },
    }),
)

genrule(
    name = "config_h",
    outs = ["include/config.h"],
    cmd = "\n".join([
        "cat <<'EOF' >$@",
        "/* include/config.h.  Generated from config.h.in by configure.  */",
        "/* include/config.h.in.  Generated from configure.ac by autoheader.  */",
        "",
        "/* Block signals before mutex operations */",
        "#define CONFIG_BLOCK_SIGNALS /**/",
        "",
        "/* Enable Debug Frame */",
        "#define CONFIG_DEBUG_FRAME /**/",
        "",
        "/* Support for Microsoft ABI extensions */",
        "/* #undef CONFIG_MSABI_SUPPORT */",
        "",
        "/* Define if the weak 'backtrace' symbol is provided. */",
        "#define CONFIG_WEAK_BACKTRACE 1",
        "",
        "/* Define to 1 if you want every memory access validated */",
        "#define CONSERVATIVE_CHECKS 1",
        "",
        "/* Define to 1 if you have the <asm/ptrace.h> header file. */",
        "#define HAVE_ASM_PTRACE_H 1",
        "",
        "/* Define to 1 if you have the <asm/ptrace_offsets.h> header file. */",
        "/* #undef HAVE_ASM_PTRACE_OFFSETS_H */",
        "",
        "/* Define to 1 if you have the <asm/vsyscall.h> header file. */",
        "#define HAVE_ASM_VSYSCALL_H 1",
        "",
        "/* Define to 1 if you have the <byteswap.h> header file. */",
        "#define HAVE_BYTESWAP_H 1",
        "",
        "/* Define to 1 if you have the declaration of `PTRACE_CONT', and to 0 if you",
        "   don't. */",
        "#define HAVE_DECL_PTRACE_CONT 1",
        "",
        "/* Define to 1 if you have the declaration of `PTRACE_POKEDATA', and to 0 if",
        "   you don't. */",
        "#define HAVE_DECL_PTRACE_POKEDATA 1",
        "",
        "/* Define to 1 if you have the declaration of `PTRACE_POKEUSER', and to 0 if",
        "   you don't. */",
        "#define HAVE_DECL_PTRACE_POKEUSER 1",
        "",
        "/* Define to 1 if you have the declaration of `PTRACE_SETREGSET', and to 0 if",
        "   you don't. */",
        "#define HAVE_DECL_PTRACE_SETREGSET 1",
        "",
        "/* Define to 1 if you have the declaration of `PTRACE_SINGLESTEP', and to 0 if",
        "   you don't. */",
        "#define HAVE_DECL_PTRACE_SINGLESTEP 1",
        "",
        "/* Define to 1 if you have the declaration of `PTRACE_SYSCALL', and to 0 if",
        "   you don't. */",
        "#define HAVE_DECL_PTRACE_SYSCALL 1",
        "",
        "/* Define to 1 if you have the declaration of `PTRACE_TRACEME', and to 0 if",
        "   you don't. */",
        "#define HAVE_DECL_PTRACE_TRACEME 1",
        "",
        "/* Define to 1 if you have the declaration of `PT_CONTINUE', and to 0 if you",
        "   don't. */",
        "#define HAVE_DECL_PT_CONTINUE 1",
        "",
        "/* Define to 1 if you have the declaration of `PT_GETFPREGS', and to 0 if you",
        "   don't. */",
        "#define HAVE_DECL_PT_GETFPREGS 1",
        "",
        "/* Define to 1 if you have the declaration of `PT_GETREGS', and to 0 if you",
        "   don't. */",
        "#define HAVE_DECL_PT_GETREGS 1",
        "",
        "/* Define to 1 if you have the declaration of `PT_IO', and to 0 if you don't.",
        "   */",
        "#define HAVE_DECL_PT_IO 0",
        "",
        "/* Define to 1 if you have the declaration of `PT_STEP', and to 0 if you",
        "   don't. */",
        "#define HAVE_DECL_PT_STEP 1",
        "",
        "/* Define to 1 if you have the declaration of `PT_SYSCALL', and to 0 if you",
        "   don't. */",
        "#define HAVE_DECL_PT_SYSCALL 1",
        "",
        "/* Define to 1 if you have the declaration of `PT_TRACE_ME', and to 0 if you",
        "   don't. */",
        "#define HAVE_DECL_PT_TRACE_ME 1",
        "",
        "/* Define to 1 if you have the <dlfcn.h> header file. */",
        "#define HAVE_DLFCN_H 1",
        "",
        "/* Define to 1 if you have the `dlmodinfo' function. */",
        "/* #undef HAVE_DLMODINFO */",
        "",
        "/* Define to 1 if you have the `dl_iterate_phdr' function. */",
        "#define HAVE_DL_ITERATE_PHDR 1",
        "",
        "/* Define to 1 if you have the `dl_phdr_removals_counter' function. */",
        "/* #undef HAVE_DL_PHDR_REMOVALS_COUNTER */",
        "",
        "/* Define to 1 if the system has the type `elf_fpregset_t'. */",
        "#define HAVE_ELF_FPREGSET_T 1",
        "",
        "/* Define to 1 if you have the <elf.h> header file. */",
        "#define HAVE_ELF_H 1",
        "",
        "/* Define to 1 if you have the <endian.h> header file. */",
        "#define HAVE_ENDIAN_H 1",
        "",
        "/* Define to 1 if you have the <execinfo.h> header file. */",
        "#define HAVE_EXECINFO_H 1",
        "",
        "/* Define to 1 if you have the `execvpe' function. */",
        "#define HAVE_EXECVPE 1",
        "",
        "/* Define to 1 if you have the `getunwind' function. */",
        "/* #undef HAVE_GETUNWIND */",
        "",
        "/* Define to 1 if you have the <ia64intrin.h> header file. */",
        "/* #undef HAVE_IA64INTRIN_H */",
        "",
        "/* Define to 1 if you have the <inttypes.h> header file. */",
        "#define HAVE_INTTYPES_H 1",
        "",
        "/* Define to 1 if you have the <link.h> header file. */",
        "#define HAVE_LINK_H 1",
        "",
        "/* Define if you have liblzma */",
        "#define HAVE_LZMA 1",
        "",
        "/* Define to 1 if you have the `mincore' function. */",
        "#define HAVE_MINCORE 1",
        "",
        "/* Define to 1 if you have the `pipe2' function. */",
        "#define HAVE_PIPE2 1",
        "",
        "/* Define to 1 if the system has the type `procfs_status'. */",
        "/* #undef HAVE_PROCFS_STATUS */",
        "",
        "/* Define to 1 if you have the `sigaltstack' function. */",
        "#define HAVE_SIGALTSTACK 1",
        "",
        "/* Define to 1 if you have the <signal.h> header file. */",
        "#define HAVE_SIGNAL_H 1",
        "",
        "/* Define to 1 if you have the <stdint.h> header file. */",
        "#define HAVE_STDINT_H 1",
        "",
        "/* Define to 1 if you have the <stdio.h> header file. */",
        "#define HAVE_STDIO_H 1",
        "",
        "/* Define to 1 if you have the <stdlib.h> header file. */",
        "#define HAVE_STDLIB_H 1",
        "",
        "/* Define to 1 if you have the <strings.h> header file. */",
        "#define HAVE_STRINGS_H 1",
        "",
        "/* Define to 1 if you have the <string.h> header file. */",
        "#define HAVE_STRING_H 1",
        "",
        "/* Define to 1 if `dlpi_subs' is a member of `struct dl_phdr_info'. */",
        "/* #undef HAVE_STRUCT_DL_PHDR_INFO_DLPI_SUBS */",
        "",
        "/* Define to 1 if the system has the type `struct elf_prstatus'. */",
        "#define HAVE_STRUCT_ELF_PRSTATUS 1",
        "",
        "/* Define to 1 if the system has the type `struct prstatus'. */",
        "/* #undef HAVE_STRUCT_PRSTATUS */",
        "",
        "/* Define to 1 if you have the <sys/elf.h> header file. */",
        "/* #undef HAVE_SYS_ELF_H */",
        "",
        "/* Define to 1 if you have the <sys/endian.h> header file. */",
        "/* #undef HAVE_SYS_ENDIAN_H */",
        "",
        "/* Define to 1 if you have the <sys/link.h> header file. */",
        "/* #undef HAVE_SYS_LINK_H */",
        "",
        "/* Define to 1 if you have the <sys/param.h> header file. */",
        "#define HAVE_SYS_PARAM_H 1",
        "",
        "/* Define to 1 if you have the <sys/procfs.h> header file. */",
        "#define HAVE_SYS_PROCFS_H 1",
        "",
        "/* Define to 1 if you have the <sys/ptrace.h> header file. */",
        "#define HAVE_SYS_PTRACE_H 1",
        "",
        "/* Define to 1 if you have the <sys/stat.h> header file. */",
        "#define HAVE_SYS_STAT_H 1",
        "",
        "/* Define to 1 if you have the <sys/syscall.h> header file. */",
        "#define HAVE_SYS_SYSCALL_H 1",
        "",
        "/* Define to 1 if you have the <sys/types.h> header file. */",
        "#define HAVE_SYS_TYPES_H 1",
        "",
        "/* Define to 1 if you have the <sys/uc_access.h> header file. */",
        "/* #undef HAVE_SYS_UC_ACCESS_H */",
        "",
        "/* Define to 1 if you have the `ttrace' function. */",
        "/* #undef HAVE_TTRACE */",
        "",
        "/* Define to 1 if you have the <unistd.h> header file. */",
        "#define HAVE_UNISTD_H 1",
        "",
        "/* Define if you have libz */",
        "#define HAVE_ZLIB 1",
        "",
        "/* Defined if __builtin_unreachable() is available */",
        "#define HAVE__BUILTIN_UNREACHABLE 1",
        "",
        "/* Defined if __builtin___clear_cache() is available */",
        "#define HAVE__BUILTIN___CLEAR_CACHE 1",
        "",
        "/* Define to 1 if --enable-per-thread-cache */",
        "/* #undef HAVE___CACHE_PER_THREAD */",
        "",
        "/* Define to the sub-directory where libtool stores uninstalled libraries. */",
        "#define LT_OBJDIR \".libs/\"",
        "",
        "/* Name of package */",
        "#define PACKAGE \"libunwind\"",
        "",
        "/* Define to the address where bug reports for this package should be sent. */",
        "#define PACKAGE_BUGREPORT \"https://github.com/libunwind/libunwind\"",
        "",
        "/* Define to the full name of this package. */",
        "#define PACKAGE_NAME \"libunwind\"",
        "",
        "/* Define to the full name and version of this package. */",
        "#define PACKAGE_STRING \"libunwind 1.9-pre\"",
        "",
        "/* Define to the one symbol short name of this package. */",
        "#define PACKAGE_TARNAME \"libunwind\"",
        "",
        "/* Define to the home page for this package. */",
        "#define PACKAGE_URL \"\"",
        "",
        "/* Define to the version of this package. */",
        "#define PACKAGE_VERSION \"1.9-pre\"",
        "",
        "/* The size of `off_t', as computed by sizeof. */",
        "#define SIZEOF_OFF_T 8",
        "",
        "/* Define to 1 if all of the C90 standard headers exist (not just the ones",
        "   required in a freestanding environment). This macro is provided for",
        "   backward compatibility; new code need not use it. */",
        "#define STDC_HEADERS 1",
        "",
        "/* Version number of package */",
        "#define VERSION \"1.9-pre\"",
        "",
        "/* Define to empty if `const' does not conform to ANSI C. */",
        "/* #undef const */",
        "",
        "/* Define to `__inline__' or `__inline' if that's what the C compiler",
        "   calls it, or to nothing if 'inline' is not supported under any name.  */",
        "#ifndef __cplusplus",
        "/* #undef inline */",
        "#endif",
        "",
        "/* Define to `unsigned int' if <sys/types.h> does not define. */",
        "/* #undef size_t */",
        "EOF",
    ]),
)

template_rule(
    name = "libunwind-common_h",
    src = "include/libunwind-common.h.in",
    out = "include/libunwind-common.h",
    substitutions = {
        "#define UNW_VERSION_MAJOR	@PKG_MAJOR@": "#define UNW_VERSION_MAJOR	1",
        "#define UNW_VERSION_MINOR	@PKG_MINOR@": "#define UNW_VERSION_MINOR	9",
        "#define UNW_VERSION_EXTRA	@PKG_EXTRA@": "#define UNW_VERSION_EXTRA	-pre",
        "#define UNW_VERSION_CODE(maj,min)	(((maj) << 16) | (min))": "#define UNW_VERSION_CODE(maj,min)	(((maj) << 16) | (min))",
        "#define UNW_VERSION	UNW_VERSION_CODE(UNW_VERSION_MAJOR, UNW_VERSION_MINOR)": "#define UNW_VERSION	UNW_VERSION_CODE(UNW_VERSION_MAJOR, UNW_VERSION_MINOR)",
        "#ifdef __sun": "#ifdef __sun",
        "#undef _U": "#undef _U",
        "#endif": "#endif",
        "#define UNW_PASTE2(x,y)	x##y": "#define UNW_PASTE2(x,y)	x##y",
        "#define UNW_PASTE(x,y)	UNW_PASTE2(x,y)": "#define UNW_PASTE(x,y)	UNW_PASTE2(x,y)",
        "#define UNW_OBJ(fn)	UNW_PASTE(UNW_PREFIX, fn)": "#define UNW_OBJ(fn)	UNW_PASTE(UNW_PREFIX, fn)",
        "#define UNW_ARCH_OBJ(fn) UNW_PASTE(UNW_PASTE(UNW_PASTE(_U,UNW_TARGET),_), fn)": "#define UNW_ARCH_OBJ(fn) UNW_PASTE(UNW_PASTE(UNW_PASTE(_U,UNW_TARGET),_), fn)",
        "#ifdef UNW_LOCAL_ONLY": "#ifdef UNW_LOCAL_ONLY",
        "# define UNW_PREFIX	UNW_PASTE(UNW_PASTE(_UL,UNW_TARGET),_)": "# define UNW_PREFIX	UNW_PASTE(UNW_PASTE(_UL,UNW_TARGET),_)",
        "#else /* !UNW_LOCAL_ONLY */": "#else /* !UNW_LOCAL_ONLY */",
        "# define UNW_PREFIX	UNW_PASTE(UNW_PASTE(_U,UNW_TARGET),_)": "# define UNW_PREFIX	UNW_PASTE(UNW_PASTE(_U,UNW_TARGET),_)",
        "#endif /* !UNW_LOCAL_ONLY */": "#endif /* !UNW_LOCAL_ONLY */",
        "#define UNW_NUM_EH_REGS		UNW_TDEP_NUM_EH_REGS": "#define UNW_NUM_EH_REGS		UNW_TDEP_NUM_EH_REGS",
        "#define unw_getcontext(uc)		unw_tdep_getcontext(uc)": "#define unw_getcontext(uc)		unw_tdep_getcontext(uc)",
        "#define unw_is_fpreg(r)			unw_tdep_is_fpreg(r)": "#define unw_is_fpreg(r)			unw_tdep_is_fpreg(r)",
        "#define UNW_PI_FLAG_FIRST_TDEP_BIT	16": "#define UNW_PI_FLAG_FIRST_TDEP_BIT	16",
        "#define UNW_PI_FLAG_DEBUG_FRAME	32": "#define UNW_PI_FLAG_DEBUG_FRAME	32",
        "#if defined(NEED_LAST_IP)": "#if defined(NEED_LAST_IP)",
        "#define unw_local_addr_space		UNW_OBJ(local_addr_space)": "#define unw_local_addr_space		UNW_OBJ(local_addr_space)",
        "#define unw_create_addr_space		UNW_OBJ(create_addr_space)": "#define unw_create_addr_space		UNW_OBJ(create_addr_space)",
        "#define unw_destroy_addr_space		UNW_OBJ(destroy_addr_space)": "#define unw_destroy_addr_space		UNW_OBJ(destroy_addr_space)",
        "#define unw_get_accessors		UNW_ARCH_OBJ(get_accessors)": "#define unw_get_accessors		UNW_ARCH_OBJ(get_accessors)",
        "#define unw_get_accessors_int		UNW_ARCH_OBJ(get_accessors_int)": "#define unw_get_accessors_int		UNW_ARCH_OBJ(get_accessors_int)",
        "#define unw_init_local			UNW_OBJ(init_local)": "#define unw_init_local			UNW_OBJ(init_local)",
        "#define unw_init_local2			UNW_OBJ(init_local2)": "#define unw_init_local2			UNW_OBJ(init_local2)",
        "#define unw_init_remote			UNW_OBJ(init_remote)": "#define unw_init_remote			UNW_OBJ(init_remote)",
        "#define unw_step			UNW_OBJ(step)": "#define unw_step			UNW_OBJ(step)",
        "#define unw_resume			UNW_OBJ(resume)": "#define unw_resume			UNW_OBJ(resume)",
        "#define unw_get_proc_info		UNW_OBJ(get_proc_info)": "#define unw_get_proc_info		UNW_OBJ(get_proc_info)",
        "#define unw_get_proc_info_by_ip		UNW_OBJ(get_proc_info_by_ip)": "#define unw_get_proc_info_by_ip		UNW_OBJ(get_proc_info_by_ip)",
        "#define unw_get_proc_info_in_range	UNW_OBJ(get_proc_info_in_range)": "#define unw_get_proc_info_in_range	UNW_OBJ(get_proc_info_in_range)",
        "#define unw_reg_states_iterate		UNW_OBJ(reg_states_iterate)": "#define unw_reg_states_iterate		UNW_OBJ(reg_states_iterate)",
        "#define unw_apply_reg_state		UNW_OBJ(apply_reg_state)": "#define unw_apply_reg_state		UNW_OBJ(apply_reg_state)",
        "#define unw_get_reg			UNW_OBJ(get_reg)": "#define unw_get_reg			UNW_OBJ(get_reg)",
        "#define unw_set_reg			UNW_OBJ(set_reg)": "#define unw_set_reg			UNW_OBJ(set_reg)",
        "#define unw_get_fpreg			UNW_OBJ(get_fpreg)": "#define unw_get_fpreg			UNW_OBJ(get_fpreg)",
        "#define unw_set_fpreg			UNW_OBJ(set_fpreg)": "#define unw_set_fpreg			UNW_OBJ(set_fpreg)",
        "#define unw_get_save_loc		UNW_OBJ(get_save_loc)": "#define unw_get_save_loc		UNW_OBJ(get_save_loc)",
        "#define unw_is_signal_frame		UNW_OBJ(is_signal_frame)": "#define unw_is_signal_frame		UNW_OBJ(is_signal_frame)",
        "#define unw_is_plt_entry        UNW_OBJ(is_plt_entry)": "#define unw_is_plt_entry        UNW_OBJ(is_plt_entry)",
        "#define unw_get_proc_name		UNW_OBJ(get_proc_name)": "#define unw_get_proc_name		UNW_OBJ(get_proc_name)",
        "#define unw_get_proc_name_by_ip		UNW_OBJ(get_proc_name_by_ip)": "#define unw_get_proc_name_by_ip		UNW_OBJ(get_proc_name_by_ip)",
        "#define unw_get_elf_filename		UNW_OBJ(get_elf_filename)": "#define unw_get_elf_filename		UNW_OBJ(get_elf_filename)",
        "#define unw_get_elf_filename_by_ip		UNW_OBJ(get_elf_filename_by_ip)": "#define unw_get_elf_filename_by_ip		UNW_OBJ(get_elf_filename_by_ip)",
        "#define unw_set_caching_policy		UNW_OBJ(set_caching_policy)": "#define unw_set_caching_policy		UNW_OBJ(set_caching_policy)",
        "#define unw_set_cache_size		UNW_OBJ(set_cache_size)": "#define unw_set_cache_size		UNW_OBJ(set_cache_size)",
        "#define unw_set_iterate_phdr_function	UNW_OBJ(set_iterate_phdr_function)": "#define unw_set_iterate_phdr_function	UNW_OBJ(set_iterate_phdr_function)",
        "#define unw_regname			UNW_ARCH_OBJ(regname)": "#define unw_regname			UNW_ARCH_OBJ(regname)",
        "#define unw_flush_cache			UNW_ARCH_OBJ(flush_cache)": "#define unw_flush_cache			UNW_ARCH_OBJ(flush_cache)",
        "#define unw_strerror			UNW_ARCH_OBJ(strerror)": "#define unw_strerror			UNW_ARCH_OBJ(strerror)",
    },
)

cc_library(
    name = "invisible_header",
    # buildifier: leave-alone
    hdrs = [
        "include/compiler.h",
        ":config_h",
        "include/dwarf.h",
        "include/dwarf_i.h",
        "include/dwarf-eh.h",
        "include/libunwind_i.h",
        "include/mempool.h",
        "include/remote.h",
        "include/tdep/dwarf-config.h",
        ":tdep_libunwind_i_h",
        "include/tdep/jmpbuf.h",
        "src/elf64.h",
        "src/elfxx.c",
        "src/elfxx.h",
        "src/os-linux.h",
        "src/unwind/unwind-internal.h",
        "src/ptrace/_UPT_internal.h",
        "src/setjmp/setjmp_i.h",
        "src/coredump/ucd_file_table.h",
        "src/coredump/_UCD_internal.h",
        #"src/coredump/_UCD_lib.h",
    ] + glob([
        "src/dwarf/G*.c",
        "src/mi/G*.c",
    ]) + select({
        ":linux_x86_64": [
            "src/x86_64/init.h",
            "src/x86_64/ucontext_i.h",
            "src/x86_64/unwind_i.h",
            "include/tdep-x86_64/dwarf-config.h",
            "include/tdep-x86_64/libunwind_i.h",
            "include/tdep-x86_64/jmpbuf.h",
        ] + glob(
            ["src/x86_64/G*.c"],
        ),
        ":linux_aarch64": [
            "src/aarch64/init.h",
            "src/aarch64/ucontext_i.h",
            "src/aarch64/unwind_i.h",
            "include/tdep-aarch64/dwarf-config.h",
            "include/tdep-aarch64/libunwind_i.h",
            "include/tdep-aarch64/jmpbuf.h",
        ] + glob(
            ["src/aarch64/G*.c"],
        ),
    }),
    # buildifier: leave-alone
    visibility = ["//visibility:private"],
)

cc_library(
    name = "unwind-elf64",
    srcs = [
        "src/elf64.c",
        "src/mi/init.c",
    ],
    hdrs = [
        "include/unwind.h",
        ":libunwind_h",
        ":libunwind-common_h",
        "include/libunwind-coredump.h",
        "include/libunwind-dynamic.h",
        "include/libunwind-ptrace.h",
    ] + select({
        ":linux_x86_64": [
            "include/libunwind-x86_64.h",
        ],
        ":linux_aarch64": [
            "include/libunwind-aarch64.h",
        ],
    }),
    copts = COPTS + select({
        ":linux_x86_64": COPTS + LINUX_X86_64_COPTS,
        ":linux_aarch64": COPTS + LINUX_AARCH64_COPTS,
    }),
    local_defines = LOCAL_DEFINES,
    deps = [
        ":invisible_header",
        "@xz",
        "@zlib",
        "@zstd",
    ],
)

cc_library(
    name = "unwind-ptrace",
    srcs = [
        "src/mi/init.c",
    ] + glob(
        ["src/ptrace/*.c"],
    ),
    copts = COPTS + select({
        ":linux_x86_64": LINUX_X86_64_COPTS,
        ":linux_aarch64": LINUX_AARCH64_COPTS,
        "//conditions:default": [],
    }),
    local_defines = LOCAL_DEFINES,
    visibility = ["//visibility:public"],
    deps = [
        ":unwind-elf64",
    ],
)

cc_library(
    name = "unwind-coredump",
    srcs = [
        "src/mi/init.c",
    ] + glob(
        ["src/coredump/*.c"],
        exclude = [
            "src/coredump/_UCD_access_reg_freebsd.c",
            "src/coredump/_UCD_access_reg_qnx.c",
            "src/coredump/_UCD_get_mapinfo_freebsd.c",
            "src/coredump/_UCD_get_mapinfo_qnx.c",
            "src/coredump/_UCD_get_mapinfo_generic.c",
        ],
    ),
    copts = COPTS + select({
        ":linux_x86_64": COPTS + LINUX_X86_64_COPTS,
        ":linux_aarch64": COPTS + LINUX_AARCH64_COPTS,
    }),
    local_defines = LOCAL_DEFINES,
    visibility = ["//visibility:public"],
    deps = [
        ":unwind-elf64",
    ],
)

cc_library(
    name = "unwind-setjmp",
    srcs = [
        "src/mi/init.c",
        "src/setjmp/longjmp.c",
        "src/setjmp/siglongjmp.c",
    ],
    copts = COPTS + select({
        ":linux_x86_64": LINUX_X86_64_COPTS,
        ":linux_aarch64": LINUX_AARCH64_COPTS,
    }),
    local_defines = LOCAL_DEFINES,
    visibility = ["//visibility:public"],
    deps = [
        ":unwind-elf64",
    ],
)

cc_library(
    name = "unwind-dwarf-common",
    srcs = [
        "src/dwarf/global.c",
    ],
    copts = COPTS + select({
        ":linux_x86_64": LINUX_X86_64_COPTS,
        ":linux_aarch64": LINUX_AARCH64_COPTS,
        "//conditions:default": [],
    }),
    local_defines = LOCAL_DEFINES,
    visibility = ["//visibility:public"],
    deps = [
        ":unwind-elf64",
    ],
)

cc_library(
    name = "unwind-dwarf-generic",
    srcs = glob([
        "src/dwarf/G*.c",
    ]),
    copts = COPTS + select({
        ":linux_x86_64": LINUX_X86_64_COPTS,
        ":linux_aarch64": LINUX_AARCH64_COPTS,
    }),
    local_defines = LOCAL_DEFINES,
    visibility = ["//visibility:public"],
    deps = [
        ":unwind-dwarf-common",
        ":unwind-elf64",
    ],
)

cc_library(
    name = "unwind-dwarf-local",
    srcs = glob([
        "src/dwarf/L*.c",
    ]),
    copts = COPTS + select({
        ":linux_x86_64": LINUX_X86_64_COPTS,
        ":linux_aarch64": LINUX_AARCH64_COPTS,
    }),
    local_defines = LOCAL_DEFINES,
    visibility = ["//visibility:public"],
    deps = [
        ":unwind-dwarf-common",
        ":unwind-elf64",
    ],
)

#sh_binary(
#name = "aarch64_preprocess",
#srcs = ["@bazel_template//bazel_scripts:aarch64_preprocess.sh"],
#)

#genrule(
#name = "preprocessed_setcontext",
#srcs = [
#"src/aarch64/setcontext.S",
#"src/aarch64/ucontext_i.h",
#],
#outs = [
#"src/aarch64/setcontext.i",
##"src/aarch64/setcontext.o",
#],
#cmd = "$(location :aarch64_preprocess) $(location src/aarch64/setcontext.S)",
#tools = [":aarch64_preprocess"],
#)

cc_library(
    name = "context",
    srcs = select({
        ":linux_x86_64": [
            "src/x86_64/getcontext.S",
            "src/x86_64/longjmp.S",
            "src/x86_64/setcontext.S",
            "src/x86_64/siglongjmp.S",
        ],
        ":linux_aarch64": [
            #"src/aarch64/getcontext.S",
            #"src/aarch64/longjmp.S",
            #"src/aarch64/setcontext.S",
            #"src/aarch64/siglongjmp.S",
        ],
    }),
    copts = COPTS + select({
        ":linux_x86_64": LINUX_X86_64_COPTS,
        ":linux_aarch64": LINUX_AARCH64_COPTS,
    }),
    local_defines = [
        "HAVE_CONFIG_H",
    ],
    textual_hdrs = select({
        ":linux_x86_64": [
            "src/x86_64/ucontext_i.h",
        ],
        ":linux_aarch64": [
            "src/aarch64/ucontext_i.h",
        ],
    }),
    visibility = ["//visibility:public"],
    deps = [":invisible_header"],
)

cc_library(
    name = "unwind",
    srcs = [
        "src/dl-iterate-phdr.c",
        "src/mi/_ReadSLEB.c",
        "src/mi/_ReadULEB.c",
        "src/mi/backtrace.c",
        "src/mi/dyn-cancel.c",
        "src/mi/dyn-info-list.c",
        "src/mi/dyn-register.c",
        "src/mi/flush_cache.c",
        "src/mi/init.c",
        "src/mi/mempool.c",
        "src/mi/strerror.c",
        "src/os-linux.c",
    ] + glob(
        [
            "src/mi/L*.c",
            "src/unwind/*.c",
        ],
        exclude = ["src/mi/Ldyn-remote.c"],
    ) + select({
        ":linux_x86_64": glob(
            [
                "src/x86_64/L*.c",
                "src/x86_64/is_fpreg.c",
                "src/x86_64/regname.c",
            ],
            exclude = [
                "src/x86_64/Los-freebsd.c",
                "src/x86_64/Los-solaris.c",
                "src/x86_64/Los-qnx.c",
            ],
        ),
        ":linux_aarch64": glob(
            [
                "src/aarch64/L*.c",
                "src/aarch64/is_fpreg.c",
                "src/aarch64/regname.c",
            ],
            exclude = [
                "src/aarch64/Los-freebsd.c",
                "src/aarch64/Los-solaris.c",
                "src/aarch64/Los-qnx.c",
            ],
        ),
    }),
    copts = COPTS + select({
        ":linux_x86_64": LINUX_X86_64_COPTS,
        ":linux_aarch64": LINUX_AARCH64_COPTS,
    }),
    local_defines = LOCAL_DEFINES,
    visibility = ["//visibility:public"],
    deps = [
        ":context",
        ":unwind-dwarf-local",
        ":unwind-elf64",
    ],
)

cc_library(
    name = "unwind-arch",
    srcs = select({
        ":linux_x86_64": [
            "src/mi/flush_cache.c",
            "src/mi/init.c",
            "src/mi/mempool.c",
            "src/mi/strerror.c",
            "src/os-linux.c",
            "src/x86_64/init.h",
            "src/x86_64/is_fpreg.c",
            "src/x86_64/regname.c",
        ] + glob(
            [
                "src/mi/G*.c",
                "src/x86_64/G*.c",
            ],
            exclude = [
                "src/x86_64/Gos-freebsd.c",
                "src/x86_64/Gos-solaris.c",
                "src/x86_64/Gos-qnx.c",
            ],
        ),
        ":linux_aarch64": [
            "src/mi/flush_cache.c",
            "src/mi/init.c",
            "src/mi/mempool.c",
            "src/mi/strerror.c",
            "src/os-linux.c",
            "src/aarch64/init.h",
            "src/aarch64/is_fpreg.c",
            "src/aarch64/regname.c",
        ] + glob(
            [
                "src/mi/G*.c",
                "src/aarch64/G*.c",
            ],
            exclude = [
                "src/aarch64/Gos-freebsd.c",
                "src/aarch64/Gos-solaris.c",
                "src/aarch64/Gos-qnx.c",
            ],
        ),
    }),
    copts = COPTS + select({
        ":linux_x86_64": LINUX_X86_64_COPTS,
        ":linux_aarch64": LINUX_AARCH64_COPTS,
    }),
    local_defines = LOCAL_DEFINES,
    visibility = ["//visibility:public"],
    deps = [
        ":unwind-dwarf-generic",
    ],
)

cc_library(
    name = "unwind-all",
    visibility = ["//visibility:public"],
    deps = [
        ":context",
        ":unwind",
        ":unwind-arch",
        ":unwind-coredump",
        ":unwind-dwarf-common",
        ":unwind-dwarf-generic",
        ":unwind-elf64",
        ":unwind-ptrace",
        ":unwind-setjmp",
    ],
)
