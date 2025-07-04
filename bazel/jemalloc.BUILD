load("@bazel_template//bazel:common.bzl", "GLOBAL_COPTS", "GLOBAL_LOCAL_DEFINES", "extract_symbols", "template_rule")

package(default_visibility = ["//visibility:public"])

COPTS = GLOBAL_COPTS + select({
    "@platforms//os:linux": [
        "-I$(GENDIR)/external/libunwind/include",
        "-Iexternal/libunwind/src",
        "-Iexternal/libunwind/include",
        "-I$(GENDIR)/external/libunwind/include/tdep",
        "-Iexternal/libunwind/include/tdep",
        "-Iexternal/libunwind/src/mi",
    ],
    "//conditions:default": [],
}) + select({
    "@platforms//os:windows": [
        "/std:c11",
        "/Iexternal/jemalloc/include",
        "/I$(GENDIR)/external/jemalloc/include",
    ],
    "//conditions:default": [
        "-std=gnu11",
        "-pipe",
        "-g3",
        "-fvisibility=hidden",
        "-Wimplicit-fallthrough",
        "-Wdeprecated-declarations",
        "-O3",
        "-funroll-loops",
        "-Iexternal/jemalloc/include",
        "-I$(GENDIR)/external/jemalloc/include",
    ],
})

LOCAL_DEFINES = GLOBAL_LOCAL_DEFINES + [
    "_REENTRANT",
] + select({
    "@platforms//os:linux": ["_GNU_SOURCE"],
    "//conditions:default": [],
})

public_symbol = [
    "aligned_alloc",
    "calloc",
    "dallocx",
    "free",
    "free_sized",
    "free_aligned_sized",
    "mallctl",
    "mallctlbymib",
    "mallctlnametomib",
    "malloc",
    "malloc_conf",
    "malloc_conf_2_conf_harder",
    "malloc_message",
    "malloc_stats_print",
    "malloc_usable_size",
    "mallocx",
    "smallocx_a25b9b8ba91881964be3083db349991bbbbf1661",
    "nallocx",
    "posix_memalign",
    "rallocx",
    "realloc",
    "sallocx",
    "sdallocx",
    "xallocx",
    "memalign",
    "valloc",
    "pvalloc",
]

sys_symbol = [
    "__libc_calloc",
    "__libc_free",
    "__libc_malloc",
    "__libc_memalign",
    "__libc_realloc",
    "__libc_valloc",
    "__libc_pvalloc",
    "pthread_create",
]

genrule(
    name = "public_symbols_txt",
    outs = ["include/jemalloc/internal/public_symbols.txt"],
    cmd = "\n".join(["cat <<'EOF' > $@"] + [
        e + ":" + e
        for e in public_symbol
    ] + ["EOF"]),
)

sh_binary(
    name = "jemalloc_mangle_sh",
    srcs = ["include/jemalloc/jemalloc_mangle.sh"],
)

genrule(
    name = "jemalloc_mangle_h",
    srcs = [":public_symbols_txt"],
    outs = ["include/jemalloc/jemalloc_mangle.h"],
    cmd = "$(location :jemalloc_mangle_sh) $(location :public_symbols_txt) je_ > $@",
    tools = [":jemalloc_mangle_sh"],
)

genrule(
    name = "jemalloc_mangle_jet_h",
    srcs = [":public_symbols_txt"],
    outs = ["include/jemalloc/jemalloc_mangle_jet.h"],
    cmd = "$(location :jemalloc_mangle_sh) $(location :public_symbols_txt) jet_ > $@",
    tools = [":jemalloc_mangle_sh"],
)

sh_binary(
    name = "jemalloc_rename_sh",
    srcs = ["include/jemalloc/jemalloc_rename.sh"],
)

genrule(
    name = "jemalloc_rename_h",
    srcs = [":public_symbols_txt"],
    outs = ["include/jemalloc/jemalloc_rename.h"],
    cmd = "$(location :jemalloc_rename_sh) $(location :public_symbols_txt) > $@",
    tools = [":jemalloc_rename_sh"],
)

sh_binary(
    name = "public_namespace_sh",
    srcs = ["include/jemalloc/internal/public_namespace.sh"],
)

genrule(
    name = "public_namespace_h",
    srcs = [":public_symbols_txt"],
    outs = ["include/jemalloc/internal/public_namespace.h"],
    cmd = "$(location :public_namespace_sh) $(location :public_symbols_txt) > $@",
    tools = [":public_namespace_sh"],
)

sh_binary(
    name = "public_unnamespace_sh",
    srcs = ["include/jemalloc/internal/public_unnamespace.sh"],
)

genrule(
    name = "public_unnamespace_h",
    srcs = [":public_symbols_txt"],
    outs = ["include/jemalloc/internal/public_unnamespace.h"],
    cmd = "$(location :public_unnamespace_sh) $(location :public_symbols_txt) > $@",
    tools = [":public_unnamespace_sh"],
)

template_rule(
    name = "jemalloc_preamble_h",
    src = "include/jemalloc/internal/jemalloc_preamble.h.in",
    out = "include/jemalloc/internal/jemalloc_preamble.h",
    substitutions = {
        "@private_namespace@": "je_",
        "@install_suffix@": "",
    },
)

template_rule(
    name = "jemalloc_internal_defs_h",
    src = "include/jemalloc/internal/jemalloc_internal_defs.h.in",
    out = "include/jemalloc/internal/jemalloc_internal_defs.h",
    substitutions = {
        "#undef JEMALLOC_OVERRIDE___LIBC_FREE_SIZED\n": "/* #undef JEMALLOC_OVERRIDE___LIBC_FREE_SIZED */\n",
        "#undef JEMALLOC_OVERRIDE___LIBC_FREE_ALIGNED_SIZED\n": "/* #undef JEMALLOC_OVERRIDE___LIBC_FREE_ALIGNED_SIZED */\n",
        "#undef JEMALLOC_OVERRIDE___POSIX_MEMALIGN": "/* #undef JEMALLOC_OVERRIDE___POSIX_MEMALIGN */",
        "#undef JEMALLOC_PRIVATE_NAMESPACE": "#define JEMALLOC_PRIVATE_NAMESPACE je_",
        "#undef HAVE_CPU_SPINWAIT": "#define HAVE_CPU_SPINWAIT 1",
        "#undef LG_VADDR": "#define LG_VADDR 48",
        "#undef JEMALLOC_C11_ATOMICS": "#define JEMALLOC_C11_ATOMICS",
        "#undef JEMALLOC_GCC_ATOMIC_ATOMICS": "#define JEMALLOC_GCC_ATOMIC_ATOMICS",
        "#undef JEMALLOC_GCC_U8_ATOMIC_ATOMICS": "#define JEMALLOC_GCC_U8_ATOMIC_ATOMICS",
        "#undef JEMALLOC_GCC_SYNC_ATOMICS": "#define JEMALLOC_GCC_SYNC_ATOMICS",
        "#undef JEMALLOC_GCC_U8_SYNC_ATOMICS": "#define JEMALLOC_GCC_U8_SYNC_ATOMICS",
        "#undef JEMALLOC_HAVE_BUILTIN_CLZ": "#define JEMALLOC_HAVE_BUILTIN_CLZ",
        "#undef JEMALLOC_HAVE_PTHREAD_ATFORK": "#define JEMALLOC_HAVE_PTHREAD_ATFORK",
        "#undef JEMALLOC_HAVE_PTHREAD_GETNAME_NP": "#define JEMALLOC_HAVE_PTHREAD_GETNAME_NP",
        "#undef JEMALLOC_HAVE_PTHREAD_SET_NAME_NP": "/* #undef JEMALLOC_HAVE_PTHREAD_SET_NAME_NP */",
        "#undef JEMALLOC_HAVE_PTHREAD_GET_NAME_NP": "/* #undef JEMALLOC_HAVE_PTHREAD_GET_NAME_NP */",
        # Newline required because of substitution conflicts.
        "#undef JEMALLOC_HAVE_CLOCK_REALTIME": "#define JEMALLOC_HAVE_CLOCK_REALTIME",
        "#undef JEMALLOC_MALLOC_THREAD_CLEANUP": "/* #undef JEMALLOC_MALLOC_THREAD_CLEANUP */",
        "#undef JEMALLOC_MUTEX_INIT_CB": "/* #undef JEMALLOC_MUTEX_INIT_CB */",
        "#undef JEMALLOC_TLS_MODEL": "#define JEMALLOC_TLS_MODEL __attribute__((tls_model(\"initial-exec\")))",
        "#undef JEMALLOC_DEBUG": "/* #undef JEMALLOC_DEBUG */",
        "#undef JEMALLOC_STATS": "#define JEMALLOC_STATS",
        "#undef JEMALLOC_EXPERIMENTAL_SMALLOCX_API": "/* #undef JEMALLOC_EXPERIMENTAL_SMALLOCX_API */",
        "#undef JEMALLOC_PROF\n": "#define JEMALLOC_PROF\n",
        "#undef JEMALLOC_PROF_LIBGCC\n": "/* #undef JEMALLOC_PROF_LIBGCC */\n",
        "#undef JEMALLOC_PROF_GCC\n": "/* #undef JEMALLOC_PROF_GCC */\n",
        "#undef JEMALLOC_PAGEID": "/* #undef JEMALLOC_PAGEID */",
        "#undef JEMALLOC_FILL": "#define JEMALLOC_FILL",
        "#undef JEMALLOC_UTRACE\n": "/* #undef JEMALLOC_UTRACE */\n",
        "#undef JEMALLOC_UTRACE_LABEL\n": "/* #undef JEMALLOC_UTRACE_LABEL */\n",
        "#undef JEMALLOC_XMALLOC": "/* #undef JEMALLOC_XMALLOC */",
        "#undef JEMALLOC_LAZY_LOCK": "/* #undef JEMALLOC_LAZY_LOCK */",
        "#undef LG_QUANTUM": "/* #undef LG_QUANTUM */",
        "#undef LG_PAGE": "#define LG_PAGE 12",
        "#undef CONFIG_LG_SLAB_MAXREGS": "/* #undef CONFIG_LG_SLAB_MAXREGS */",
        "#undef LG_HUGEPAGE": "#define LG_HUGEPAGE 21",
        "#undef JEMALLOC_MAPS_COALESCE": "#define JEMALLOC_MAPS_COALESCE",
        "#undef JEMALLOC_RETAIN": "#define JEMALLOC_RETAIN",
        "#undef JEMALLOC_INTERNAL_UNREACHABLE": "#define JEMALLOC_INTERNAL_UNREACHABLE __builtin_unreachable",
        # Newline required because of substitution conflicts.
        "#undef JEMALLOC_INTERNAL_FFSLL\n": "#define JEMALLOC_INTERNAL_FFSLL __builtin_ffsll\n",
        "#undef JEMALLOC_INTERNAL_FFSL\n": "#define JEMALLOC_INTERNAL_FFSL __builtin_ffsl\n",
        "#undef JEMALLOC_INTERNAL_FFS\n": "#define JEMALLOC_INTERNAL_FFS __builtin_ffs\n",
        "#undef JEMALLOC_INTERNAL_POPCOUNTL\n": "#define JEMALLOC_INTERNAL_POPCOUNTL __builtin_popcountl\n",
        "#undef JEMALLOC_INTERNAL_POPCOUNT\n": "#define JEMALLOC_INTERNAL_POPCOUNT __builtin_popcount\n",
        "#undef JEMALLOC_CACHE_OBLIVIOUS": "#define JEMALLOC_CACHE_OBLIVIOUS",
        "#undef JEMALLOC_LOG": "/* #undef JEMALLOC_LOG */",
        "#undef JEMALLOC_READLINKAT": "/* #undef JEMALLOC_READLINKAT */",
        "#undef JEMALLOC_FORCE_GETENV": "/* #undef JEMALLOC_FORCE_GETENV */",
        "#undef JEMALLOC_SYSCTL_VM_OVERCOMMIT": "/* #undef JEMALLOC_SYSCTL_VM_OVERCOMMIT */",
        "#undef JEMALLOC_HAVE_MADVISE\n": "#define JEMALLOC_HAVE_MADVISE\n",
        "#undef JEMALLOC_PURGE_MADVISE_FREE": "#define JEMALLOC_PURGE_MADVISE_FREE",
        "#undef JEMALLOC_PURGE_MADVISE_DONTNEED\n": "#define JEMALLOC_PURGE_MADVISE_DONTNEED\n",
        "#undef JEMALLOC_DEFINE_MADVISE_FREE": "/* #undef JEMALLOC_DEFINE_MADVISE_FREE */",
        "#undef JEMALLOC_MADVISE_NOCORE": "/* #undef JEMALLOC_MADVISE_NOCORE */",
        "#undef JEMALLOC_HAVE_MPROTECT": "#define JEMALLOC_HAVE_MPROTECT",
        "#undef JEMALLOC_THP": "/* #undef JEMALLOC_THP */",
        "#undef JEMALLOC_HAVE_POSIX_MADVISE": "/* #undef JEMALLOC_HAVE_POSIX_MADVISE */",
        "#undef JEMALLOC_PURGE_POSIX_MADVISE_DONTNEED\n": "/* #undef JEMALLOC_PURGE_POSIX_MADVISE_DONTNEED */\n",
        "#undef JEMALLOC_PURGE_POSIX_MADVISE_DONTNEED_ZEROS\n": "/* #undef JEMALLOC_PURGE_POSIX_MADVISE_DONTNEED_ZEROS */\n",
        "#undef JEMALLOC_HAVE_MEMCNTL": "/* #undef JEMALLOC_HAVE_MEMCNTL */",
        "#undef JEMALLOC_HAS_RESTRICT": "#define JEMALLOC_HAS_RESTRICT",
        "#undef JEMALLOC_BIG_ENDIAN": "/* #undef JEMALLOC_BIG_ENDIAN */",
        # Newline required because of substitution conflicts.
        "#undef LG_SIZEOF_INT\n": "#define LG_SIZEOF_INT 2\n",
        "#undef LG_SIZEOF_LONG\n": "#define LG_SIZEOF_LONG 3\n",
        "#undef LG_SIZEOF_LONG_LONG": "#define LG_SIZEOF_LONG_LONG 3",
        "#undef LG_SIZEOF_INTMAX_T": "#define LG_SIZEOF_INTMAX_T 3",
        "#undef JEMALLOC_GLIBC_MALLOC_HOOK": "/* #undef JEMALLOC_GLIBC_MALLOC_HOOK */",
        "#undef JEMALLOC_GLIBC_MEMALIGN_HOOK": "/* #undef JEMALLOC_GLIBC_MEMALIGN_HOOK */",
        "#undef JEMALLOC_HAVE_PTHREAD\n": "#define JEMALLOC_HAVE_PTHREAD\n",
        "#undef JEMALLOC_HAVE_DLSYM": "#define JEMALLOC_HAVE_DLSYM",
        "#undef JEMALLOC_EXPORT": "/* #undef JEMALLOC_EXPORT */",
        "#undef JEMALLOC_CONFIG_MALLOC_CONF": "#define JEMALLOC_CONFIG_MALLOC_CONF \"\"",
        "#undef JEMALLOC_OPT_SAFETY_CHECKS": "/* #undef JEMALLOC_OPT_SAFETY_CHECKS */",
        "#undef JEMALLOC_ENABLE_CXX": "#define JEMALLOC_ENABLE_CXX",
        "#undef JEMALLOC_OPT_SIZE_CHECKS": "/* #undef JEMALLOC_OPT_SIZE_CHECKS */",
        "#undef JEMALLOC_UAF_DETECTION": "/* #undef JEMALLOC_UAF_DETECTION */",
        "#undef JEMALLOC_HAVE_ASM_VOLATILE": "#define JEMALLOC_HAVE_ASM_VOLATILE",
        "#undef JEMALLOC_HAVE_RDTSCP": "#define JEMALLOC_HAVE_RDTSCP",
    } | select({
        "@platforms//os:linux": {
            "#undef JEMALLOC_PREFIX": "/* #undef JEMALLOC_PREFIX */",
            "#undef JEMALLOC_CPREFIX": "/* #undef JEMALLOC_CPREFIX */",
            "#undef JEMALLOC_PROF_LIBUNWIND\n": "#define JEMALLOC_PROF_LIBUNWIND\n",
            "#undef JEMALLOC_OVERRIDE___LIBC_CALLOC": "#define JEMALLOC_OVERRIDE___LIBC_CALLOC",
            "#undef JEMALLOC_OVERRIDE___LIBC_FREE\n": "#define JEMALLOC_OVERRIDE___LIBC_FREE\n",
            "#undef JEMALLOC_OVERRIDE___LIBC_MALLOC": "#define JEMALLOC_OVERRIDE___LIBC_MALLOC",
            "#undef JEMALLOC_OVERRIDE___LIBC_MEMALIGN": "#define JEMALLOC_OVERRIDE___LIBC_MEMALIGN",
            "#undef JEMALLOC_OVERRIDE___LIBC_REALLOC": "#define JEMALLOC_OVERRIDE___LIBC_REALLOC",
            "#undef JEMALLOC_OVERRIDE___LIBC_VALLOC": "#define JEMALLOC_OVERRIDE___LIBC_VALLOC",
            "#undef JEMALLOC_OVERRIDE___LIBC_PVALLOC": "#define JEMALLOC_OVERRIDE___LIBC_PVALLOC",
            "#undef JEMALLOC_OS_UNFAIR_LOCK": "/* #undef JEMALLOC_OS_UNFAIR_LOCK */",
            "#undef JEMALLOC_USE_SYSCALL": "#define JEMALLOC_USE_SYSCALL",
            "#undef JEMALLOC_HAVE_SECURE_GETENV": "#define JEMALLOC_HAVE_SECURE_GETENV",
            "#undef JEMALLOC_HAVE_ISSETUGID": "/* #undef JEMALLOC_HAVE_ISSETUGID */",
            "#undef JEMALLOC_HAVE_PTHREAD_SETNAME_NP": "#define JEMALLOC_HAVE_PTHREAD_SETNAME_NP",
            "#undef JEMALLOC_HAVE_CLOCK_MONOTONIC_COARSE": "#define JEMALLOC_HAVE_CLOCK_MONOTONIC_COARSE",
            "#undef JEMALLOC_HAVE_CLOCK_MONOTONIC\n": "#define JEMALLOC_HAVE_CLOCK_MONOTONIC\n",
            "#undef JEMALLOC_HAVE_MACH_ABSOLUTE_TIME": "/* #undef JEMALLOC_HAVE_MACH_ABSOLUTE_TIME */",
            "#undef JEMALLOC_THREADED_INIT": "#define JEMALLOC_THREADED_INIT",
            "#undef JEMALLOC_HAVE_PRCTL": "#define JEMALLOC_HAVE_PRCTL",
            "#undef JEMALLOC_TLS\n": "#define JEMALLOC_TLS\n",
            "#undef JEMALLOC_ZONE": "/* #undef JEMALLOC_ZONE */",
            "#undef JEMALLOC_PROC_SYS_VM_OVERCOMMIT_MEMORY": "#define JEMALLOC_PROC_SYS_VM_OVERCOMMIT_MEMORY",
            "#undef JEMALLOC_HAVE_MADVISE_HUGE": "#define JEMALLOC_HAVE_MADVISE_HUGE",
            "#undef JEMALLOC_PURGE_MADVISE_DONTNEED_ZEROS": "#define JEMALLOC_PURGE_MADVISE_DONTNEED_ZEROS",
            "#undef JEMALLOC_MADVISE_DONTDUMP": "#define JEMALLOC_MADVISE_DONTDUMP",
            "#undef JEMALLOC_HAVE_MALLOC_SIZE": "/* #undef JEMALLOC_HAVE_MALLOC_SIZE */",
            "#undef JEMALLOC_HAS_ALLOCA_H": "#define JEMALLOC_HAS_ALLOCA_H",
            "#undef JEMALLOC_HAVE_PTHREAD_MUTEX_ADAPTIVE_NP": "#define JEMALLOC_HAVE_PTHREAD_MUTEX_ADAPTIVE_NP",
            "#undef JEMALLOC_HAVE_SCHED_GETCPU": "#define JEMALLOC_HAVE_SCHED_GETCPU",
            "#undef JEMALLOC_HAVE_SCHED_SETAFFINITY": "#define JEMALLOC_HAVE_SCHED_SETAFFINITY",
            "#undef JEMALLOC_HAVE_PTHREAD_SETAFFINITY_NP": "#define JEMALLOC_HAVE_PTHREAD_SETAFFINITY_NP",
            "#undef JEMALLOC_BACKGROUND_THREAD": "#define JEMALLOC_BACKGROUND_THREAD",
            "#undef JEMALLOC_IS_MALLOC": "#define JEMALLOC_IS_MALLOC",
            "#undef JEMALLOC_STRERROR_R_RETURNS_CHAR_WITH_GNU_SOURCE": "#define JEMALLOC_STRERROR_R_RETURNS_CHAR_WITH_GNU_SOURCE",
            "#undef JEMALLOC_HAVE_VM_MAKE_TAG": "/* #undef JEMALLOC_HAVE_VM_MAKE_TAG */",
            "#undef JEMALLOC_ZERO_REALLOC_DEFAULT_FREE": "#define JEMALLOC_ZERO_REALLOC_DEFAULT_FREE",
            "#undef JEMALLOC_DSS": "#define JEMALLOC_DSS",
        },
        "@platforms//os:osx": {
            "#undef JEMALLOC_PREFIX": "#define JEMALLOC_PREFIX \"je_\"",
            "#undef JEMALLOC_CPREFIX": "#define JEMALLOC_CPREFIX \"JE_\"",
            "#undef JEMALLOC_DSS": "/* #undef JEMALLOC_DSS */",
            "#undef JEMALLOC_PROF_LIBUNWIND\n": "/* #undef JEMALLOC_PROF_LIBUNWIND */\n",
            "#undef JEMALLOC_OVERRIDE___LIBC_CALLOC": "/* #undef JEMALLOC_OVERRIDE___LIBC_CALLOC */",
            "#undef JEMALLOC_OVERRIDE___LIBC_FREE\n": "/* #undef JEMALLOC_OVERRIDE___LIBC_FREE */ \n",
            "#undef JEMALLOC_OVERRIDE___LIBC_MALLOC": "/* #undef JEMALLOC_OVERRIDE___LIBC_MALLOC */",
            "#undef JEMALLOC_OVERRIDE___LIBC_MEMALIGN": "/* #undef JEMALLOC_OVERRIDE___LIBC_MEMALIGN */",
            "#undef JEMALLOC_OVERRIDE___LIBC_REALLOC": "/* #undef JEMALLOC_OVERRIDE___LIBC_REALLOC */",
            "#undef JEMALLOC_OVERRIDE___LIBC_VALLOC": "/* #undef JEMALLOC_OVERRIDE___LIBC_VALLOC */",
            "#undef JEMALLOC_OVERRIDE___LIBC_PVALLOC": "/* #undef JEMALLOC_OVERRIDE___LIBC_PVALLOC */",
            "#undef JEMALLOC_OS_UNFAIR_LOCK": "#define JEMALLOC_OS_UNFAIR_LOCK",
            "#undef JEMALLOC_USE_SYSCALL": "/* #undef JEMALLOC_USE_SYSCALL */",
            "#undef JEMALLOC_HAVE_SECURE_GETENV": "/* #undef JEMALLOC_HAVE_SECURE_GETENV */",
            "#undef JEMALLOC_HAVE_ISSETUGID": "#define JEMALLOC_HAVE_ISSETUGID",
            "#undef JEMALLOC_HAVE_PTHREAD_SETNAME_NP": "/* #undef JEMALLOC_HAVE_PTHREAD_SETNAME_NP */",
            "#undef JEMALLOC_HAVE_CLOCK_MONOTONIC_COARSE": "/* #undef JEMALLOC_HAVE_CLOCK_MONOTONIC_COARSE */",
            "#undef JEMALLOC_HAVE_CLOCK_MONOTONIC\n": "/* #undef JEMALLOC_HAVE_CLOCK_MONOTONIC */\n",
            "#undef JEMALLOC_HAVE_MACH_ABSOLUTE_TIME": "#define JEMALLOC_HAVE_MACH_ABSOLUTE_TIME",
            "#undef JEMALLOC_THREADED_INIT": "/* #undef JEMALLOC_THREADED_INIT */",
            "#undef JEMALLOC_HAVE_PRCTL": "/* #undef JEMALLOC_HAVE_PRCTL */",
            "#undef JEMALLOC_TLS\n": "/* #undef JEMALLOC_TLS */\n",
            "#undef JEMALLOC_ZONE": "#define JEMALLOC_ZONE",
            "#undef JEMALLOC_PROC_SYS_VM_OVERCOMMIT_MEMORY": "/* #undef JEMALLOC_PROC_SYS_VM_OVERCOMMIT_MEMORY */",
            "#undef JEMALLOC_HAVE_MADVISE_HUGE": "/* #undef JEMALLOC_HAVE_MADVISE_HUGE */",
            "#undef JEMALLOC_PURGE_MADVISE_DONTNEED_ZEROS": "/* #undef JEMALLOC_PURGE_MADVISE_DONTNEED_ZEROS */",
            "#undef JEMALLOC_MADVISE_DONTDUMP": "/* #undef JEMALLOC_MADVISE_DONTDUMP */",
            "#undef JEMALLOC_HAVE_MALLOC_SIZE": "#define JEMALLOC_HAVE_MALLOC_SIZE",
            "#undef JEMALLOC_HAS_ALLOCA_H": "/* #undef JEMALLOC_HAS_ALLOCA_H */",
            "#undef JEMALLOC_HAVE_PTHREAD_MUTEX_ADAPTIVE_NP": "/* #undef JEMALLOC_HAVE_PTHREAD_MUTEX_ADAPTIVE_NP */",
            "#undef JEMALLOC_HAVE_SCHED_GETCPU": "/* #undef JEMALLOC_HAVE_SCHED_GETCPU */",
            "#undef JEMALLOC_HAVE_SCHED_SETAFFINITY": "/* #undef JEMALLOC_HAVE_SCHED_SETAFFINITY */",
            "#undef JEMALLOC_HAVE_PTHREAD_SETAFFINITY_NP": "/* #undef JEMALLOC_HAVE_PTHREAD_SETAFFINITY_NP */",
            "#undef JEMALLOC_BACKGROUND_THREAD": "/* #undef JEMALLOC_BACKGROUND_THREAD */",
            "#undef JEMALLOC_IS_MALLOC": "/* #undef JEMALLOC_IS_MALLOC */",
            "#undef JEMALLOC_STRERROR_R_RETURNS_CHAR_WITH_GNU_SOURCE": "/* #undef JEMALLOC_STRERROR_R_RETURNS_CHAR_WITH_GNU_SOURCE */",
            "#undef JEMALLOC_HAVE_VM_MAKE_TAG": "#define JEMALLOC_HAVE_VM_MAKE_TAG",
            "#undef JEMALLOC_ZERO_REALLOC_DEFAULT_FREE": "/* #undef JEMALLOC_ZERO_REALLOC_DEFAULT_FREE */",
        },
        "@platforms//os:windows": {
            "#undef JEMALLOC_PREFIX": "#define JEMALLOC_PREFIX \"je_\"",
            "#undef JEMALLOC_CPREFIX": "#define JEMALLOC_CPREFIX \"JE_\"",
            "#undef JEMALLOC_OVERRIDE___LIBC_CALLOC": "/* #undef JEMALLOC_OVERRIDE___LIBC_CALLOC */",
            "#undef JEMALLOC_OVERRIDE___LIBC_FREE\n": "/* #undef JEMALLOC_OVERRIDE___LIBC_FREE */\n",
            "#undef JEMALLOC_OVERRIDE___LIBC_MALLOC": "/* #undef JEMALLOC_OVERRIDE___LIBC_MALLOC */",
            "#undef JEMALLOC_OVERRIDE___LIBC_MEMALIGN": "/* #undef JEMALLOC_OVERRIDE___LIBC_MEMALIGN */",
            "#undef JEMALLOC_OVERRIDE___LIBC_REALLOC": "/* #undef JEMALLOC_OVERRIDE___LIBC_REALLOC */",
            "#undef JEMALLOC_OVERRIDE___LIBC_VALLOC": "/* #undef JEMALLOC_OVERRIDE___LIBC_VALLOC */",
            "#undef JEMALLOC_OVERRIDE___LIBC_PVALLOC": "/* #undef JEMALLOC_OVERRIDE___LIBC_PVALLOC */",
            "#define LG_VADDR 48": "#define LG_VADDR 57",
            "#undef JEMALLOC_OS_UNFAIR_LOCK": "/* #undef JEMALLOC_OS_UNFAIR_LOCK */",
            "#undef JEMALLOC_USE_SYSCALL": "/* #undef JEMALLOC_USE_SYSCALL */",
            "#undef JEMALLOC_HAVE_SECURE_GETENV": "/* #undef JEMALLOC_HAVE_SECURE_GETENV */",
            "#undef JEMALLOC_HAVE_ISSETUGID": "/* #undef JEMALLOC_HAVE_ISSETUGID */",
            "#define JEMALLOC_HAVE_PTHREAD_ATFORK": "/* #undef JEMALLOC_HAVE_PTHREAD_ATFORK */",
            "#undef JEMALLOC_HAVE_PTHREAD_SETNAME_NP": "/* #undef JEMALLOC_HAVE_PTHREAD_SETNAME_NP */",
            "#define JEMALLOC_HAVE_PTHREAD_GETNAME_NP": "/* #undef JEMALLOC_HAVE_PTHREAD_GETNAME_NP */",
            "#undef JEMALLOC_HAVE_CLOCK_MONOTONIC_COARSE": "/* #undef JEMALLOC_HAVE_CLOCK_MONOTONIC_COARSE */",
            "#undef JEMALLOC_HAVE_CLOCK_MONOTONIC\n": "#define JEMALLOC_HAVE_CLOCK_MONOTONIC\n",
            "#undef JEMALLOC_HAVE_MACH_ABSOLUTE_TIME": "/* #undef JEMALLOC_HAVE_MACH_ABSOLUTE_TIME */",
            "#undef JEMALLOC_THREADED_INIT": "/* #undef JEMALLOC_THREADED_INIT */",
            "#define JEMALLOC_PROF": "/* #undef JEMALLOC_PROF */",
            "#undef JEMALLOC_PROF_LIBUNWIND\n": "/* #undef JEMALLOC_PROF_LIBUNWIND */\n",
            #"#undef JEMALLOC_PROF_LIBUNWIND\n": "#define JEMALLOC_PROF_LIBUNWIND\n",
            "#undef JEMALLOC_HAVE_PRCTL": "/* #undef JEMALLOC_HAVE_PRCTL */",
            "#undef JEMALLOC_DSS": "/* #undef JEMALLOC_DSS */",
            "#define JEMALLOC_MAPS_COALESCE": "/* #undef JEMALLOC_MAPS_COALESCE */",
            "#undef JEMALLOC_TLS": "/* #undef JEMALLOC_TLS */",
            "#undef JEMALLOC_ZONE": "/* #undef JEMALLOC_ZONE */",
            "#undef JEMALLOC_PROC_SYS_VM_OVERCOMMIT_MEMORY": "/* #undef JEMALLOC_PROC_SYS_VM_OVERCOMMIT_MEMORY */",
            "#define JEMALLOC_HAVE_MADVISE": "/* #undef JEMALLOC_HAVE_MADVISE */",
            "#undef JEMALLOC_HAVE_MADVISE_HUGE": "/* #undef JEMALLOC_HAVE_MADVISE_HUGE */",
            "#define JEMALLOC_PURGE_MADVISE_FREE": "/* #undef JEMALLOC_PURGE_MADVISE_FREE */",
            "#define JEMALLOC_PURGE_MADVISE_DONTNEED": "/* #undef JEMALLOC_PURGE_MADVISE_DONTNEED */",
            "#undef JEMALLOC_PURGE_MADVISE_DONTNEED_ZEROS": "/* #undef JEMALLOC_PURGE_MADVISE_DONTNEED_ZEROS */",
            "#undef JEMALLOC_MADVISE_DONTDUMP": "/* #undef JEMALLOC_MADVISE_DONTDUMP */",
            "#define JEMALLOC_HAVE_MPROTECT": "/* #undef JEMALLOC_HAVE_MPROTECT */",
            "#undef JEMALLOC_HAVE_MALLOC_SIZE": "/* #undef JEMALLOC_HAVE_MALLOC_SIZE */",
            "#undef JEMALLOC_HAS_ALLOCA_H": "/* #undef JEMALLOC_HAS_ALLOCA_H */",
            "#define LG_SIZEOF_LONG 3": "#define LG_SIZEOF_LONG 2",
            "#define JEMALLOC_HAVE_PTHREAD": "/* #undef JEMALLOC_HAVE_PTHREAD */",
            "#define JEMALLOC_HAVE_DLSYM": "/* #undef JEMALLOC_HAVE_DLSYM */",
            "#undef JEMALLOC_HAVE_PTHREAD_MUTEX_ADAPTIVE_NP": "#define JEMALLOC_HAVE_PTHREAD_MUTEX_ADAPTIVE_NP",
            "#undef JEMALLOC_HAVE_SCHED_GETCPU": "/* #undef JEMALLOC_HAVE_SCHED_GETCPU */",
            "#undef JEMALLOC_HAVE_SCHED_SETAFFINITY": "/* #undef JEMALLOC_HAVE_SCHED_SETAFFINITY */",
            "#undef JEMALLOC_HAVE_PTHREAD_SETAFFINITY_NP": "/* #undef JEMALLOC_HAVE_PTHREAD_SETAFFINITY_NP */",
            "#undef JEMALLOC_BACKGROUND_THREAD": "/* #undef JEMALLOC_BACKGROUND_THREAD */",
            "#undef JEMALLOC_IS_MALLOC": "/* #undef JEMALLOC_IS_MALLOC */",
            "#undef JEMALLOC_STRERROR_R_RETURNS_CHAR_WITH_GNU_SOURCE": "/* #undef JEMALLOC_STRERROR_R_RETURNS_CHAR_WITH_GNU_SOURCE */",
            "#undef JEMALLOC_HAVE_VM_MAKE_TAG": "/* #undef JEMALLOC_HAVE_VM_MAKE_TAG */",
            "#undef JEMALLOC_ZERO_REALLOC_DEFAULT_FREE": "#define JEMALLOC_ZERO_REALLOC_DEFAULT_FREE",
            "#define JEMALLOC_HAVE_ASM_VOLATILE": "/* #undef JEMALLOC_HAVE_ASM_VOLATILE */",
            "#define JEMALLOC_HAVE_RDTSCP": "/* #undef JEMALLOC_HAVE_RDTSCP */",
        },
        "//conditions:default": {
        },
    }) | select({
        "@platforms//cpu:aarch64": {
            "#undef CPU_SPINWAIT": "#define CPU_SPINWAIT __asm__ volatile(\"isb\")",
        },
        "//conditions:default": {
            "#undef CPU_SPINWAIT": "#define CPU_SPINWAIT __asm__ volatile(\"pause\")",
        },
    }),
)

template_rule(
    name = "jemalloc_defs_h",
    src = "include/jemalloc/jemalloc_defs.h.in",
    out = "include/jemalloc/jemalloc_defs.h",
    substitutions = {
        "#undef JEMALLOC_HAVE_ATTR": "#define JEMALLOC_HAVE_ATTR",
        "#undef JEMALLOC_HAVE_ATTR_ALLOC_SIZE": "#define JEMALLOC_HAVE_ATTR_ALLOC_SIZE",
        "#undef JEMALLOC_HAVE_ATTR_FORMAT_ARG": "#define JEMALLOC_HAVE_ATTR_FORMAT_ARG",
        "#undef JEMALLOC_HAVE_ATTR_FORMAT_PRINTF": "#define JEMALLOC_HAVE_ATTR_FORMAT_PRINTF",
        "#undef JEMALLOC_HAVE_ATTR_FALLTHROUGH": "#define JEMALLOC_HAVE_ATTR_FALLTHROUGH",
        "#undef JEMALLOC_HAVE_ATTR_COLD": "#define JEMALLOC_HAVE_ATTR_COLD",
        "#undef JEMALLOC_HAVE_ATTR_DEPRECATED": "#define JEMALLOC_HAVE_ATTR_DEPRECATED",
        "#undef JEMALLOC_OVERRIDE_VALLOC": "#define JEMALLOC_OVERRIDE_VALLOC",
        "#undef LG_SIZEOF_PTR": "#define LG_SIZEOF_PTR 3",
    } | select({
        "@platforms//os:osx": {
            "#undef JEMALLOC_HAVE_ATTR_FORMAT_GNU_PRINTF": "/* #undef JEMALLOC_HAVE_ATTR_FORMAT_GNU_PRINTF */",
            "#undef JEMALLOC_OVERRIDE_MEMALIGN": "/* #undef JEMALLOC_OVERRIDE_MEMALIGN */",
            "#undef JEMALLOC_OVERRIDE_PVALLOC": "/* #undef JEMALLOC_OVERRIDE_PVALLOC */",
            "#undef JEMALLOC_USABLE_SIZE_CONST": "#define JEMALLOC_USABLE_SIZE_CONST const",
            "#undef JEMALLOC_USE_CXX_THROW": "/* #undef JEMALLOC_USE_CXX_THROW */",
        },
        "@platforms//os:linux": {
            "#undef JEMALLOC_HAVE_ATTR_FORMAT_GNU_PRINTF": "#define JEMALLOC_HAVE_ATTR_FORMAT_GNU_PRINTF",
            "#undef JEMALLOC_OVERRIDE_MEMALIGN": "#define JEMALLOC_OVERRIDE_MEMALIGN",
            "#undef JEMALLOC_OVERRIDE_PVALLOC": "#define JEMALLOC_OVERRIDE_PVALLOC",
            "#undef JEMALLOC_USABLE_SIZE_CONST": "#define JEMALLOC_USABLE_SIZE_CONST",
            "#undef JEMALLOC_USE_CXX_THROW": "#define JEMALLOC_USE_CXX_THROW",
        },
        "@platforms//os:windows": {
            "#define JEMALLOC_HAVE_ATTR_ALLOC_SIZE": "/* #undef JEMALLOC_HAVE_ATTR_ALLOC_SIZE */",
            "#define JEMALLOC_HAVE_ATTR_FORMAT_ARG": "/* #undef JEMALLOC_HAVE_ATTR_FORMAT_ARG */",
            "#define JEMALLOC_HAVE_ATTR_FORMAT_GNU_PRINTF": "/* #undef JEMALLOC_HAVE_ATTR_FORMAT_GNU_PRINTF */",
            "#define JEMALLOC_HAVE_ATTR_FORMAT_PRINTF": "/* #undef JEMALLOC_HAVE_ATTR_FORMAT_PRINTF */",
            "#undef JEMALLOC_OVERRIDE_MEMALIGN": "/* #undef JEMALLOC_OVERRIDE_MEMALIGN */",
            "#define JEMALLOC_OVERRIDE_VALLOC": "/* #undef JEMALLOC_OVERRIDE_VALLOC */",
            "#undef JEMALLOC_OVERRIDE_PVALLOC": "/* #undef JEMALLOC_OVERRIDE_PVALLOC */",
            "#undef JEMALLOC_USABLE_SIZE_CONST": "#define JEMALLOC_USABLE_SIZE_CONST const",
            "#undef JEMALLOC_USE_CXX_THROW": "/* #undef JEMALLOC_USE_CXX_THROW */",
        },
        "//conditions:default": {
        },
    }),
)

template_rule(
    name = "jemalloc_macros_h",
    src = "include/jemalloc/jemalloc_macros.h.in",
    out = "include/jemalloc/jemalloc_macros.h",
    substitutions = {
        "@jemalloc_version@": "5.3.0-196-ga25b9b8ba91881964be3083db349991bbbbf1661",
        "@jemalloc_version_major@": "5",
        "@jemalloc_version_minor@": "3",
        "@jemalloc_version_bugfix@": "0",
        "@jemalloc_version_nrev@": "196",
        "@jemalloc_version_gid@": "a25b9b8ba91881964be3083db349991bbbbf1661",
    },
)

template_rule(
    name = "jemalloc_protos_h",
    src = "include/jemalloc/jemalloc_protos.h.in",
    out = "include/jemalloc/jemalloc_protos.h",
    substitutions = {
        "@je_@": "je_",
        "@install_suffix@": "",
    },
)

template_rule(
    name = "jemalloc_protos_jet_h",
    src = "include/jemalloc/jemalloc_protos.h.in",
    out = "include/jemalloc/jemalloc_protos_jet.h",
    substitutions = {
        "@je_@": "jet_",
        "@install_suffix@": "",
    },
)

template_rule(
    name = "jemalloc_typedefs_h",
    src = "include/jemalloc/jemalloc_typedefs.h.in",
    out = "include/jemalloc/jemalloc_typedefs.h",
    substitutions = {},
)

sh_binary(
    name = "jemalloc_sh",
    srcs = ["include/jemalloc/jemalloc.sh"],
)

genrule(
    name = "jemalloc_h",
    srcs = [
        ":jemalloc_defs_h",
        ":jemalloc_macros_h",
        ":jemalloc_mangle_h",
        ":jemalloc_protos_h",
        ":jemalloc_rename_h",
        ":jemalloc_typedefs_h",
    ],
    outs = ["include/jemalloc/jemalloc.h"],
    cmd = "$(location :jemalloc_sh) $$(dirname $(location :jemalloc_defs_h))/../../ > $@",
    tools = [":jemalloc_sh"],
)

sh_binary(
    name = "private_symbols_sh",
    srcs = ["include/jemalloc/internal/private_symbols.sh"],
)

genrule(
    name = "private_symbols_awk",
    srcs = [],
    outs = ["include/jemalloc/internal/private_symbols.awk"],
    cmd = """
            $(location :private_symbols_sh) "" {list} > $@
          """.format(list = " ".join(public_symbol + sys_symbol)),
    tools = [":private_symbols_sh"],
)

genrule(
    name = "private_symbols_jet_awk",
    srcs = [],
    outs = ["include/jemalloc/internal/private_symbols_jet.awk"],
    cmd = """
            $(location :private_symbols_sh) "" {list} > $@
          """.format(list = " ".join(["jet_" + e for e in public_symbol] + sys_symbol)),
    tools = [":private_symbols_sh"],
)

extract_symbols(
    name = "private_symbols",
    awk_script = ":private_symbols_awk",
    deps = [
        ":jemalloc_headers",
        ":jemalloc_public",
    ],
)

sh_binary(
    name = "private_namespace_sh",
    srcs = ["include/jemalloc/internal/private_namespace.sh"],
)

genrule(
    name = "private_namespace_h",
    srcs = [":private_symbols"],
    outs = ["include/jemalloc/internal/private_namespace.h"],
    cmd = "$(location :private_namespace_sh) $(locations :private_symbols) > $@",
    tools = [":private_namespace_sh"],
)

cc_library(
    name = "jemalloc_headers",
    hdrs = ["include/jemalloc/jemalloc.h"],
    includes = ["include"],
)

cc_library(
    name = "jemalloc_public",
    srcs = glob(
        ["src/*.c"],
        exclude = ["src/zone.c"],
    ) + select({
        "@bazel_template//bazel:linux_x86_64": [],
        "@bazel_template//bazel:linux_aarch64": [],
        "@bazel_template//bazel:osx_x86_64": ["src/zone.c"],
        "@bazel_template//bazel:osx_aarch64": ["src/zone.c"],
        "//conditions:default": [],
    }),
    hdrs = [
        ":jemalloc_defs_h",
        ":jemalloc_h",
        ":jemalloc_internal_defs_h",
        ":jemalloc_macros_h",
        ":jemalloc_mangle_h",
        ":jemalloc_preamble_h",
        ":jemalloc_protos_h",
        ":jemalloc_rename_h",
        ":jemalloc_typedefs_h",
    ] + glob([
        "include/jemalloc/internal/*.h",
        "include/jemalloc/*.h",
    ]) + select({
        "@platforms//os:windows": [
            "include/msvc_compat/C99/stdbool.h",
            "include/msvc_compat/C99/stdint.h",
            "include/msvc_compat/strings.h",
            "include/msvc_compat/windows_extra.h",
        ],
        "//conditions:default": [],
    }),
    copts = COPTS + select({
        "@platforms//os:windows": [
            "/Iexternal/jemalloc/include/msvc_compat",
        ],
        "//conditions:default": [],
    }),
    linkopts = ["-lpthread"],
    local_defines = [
        "JEMALLOC_NO_PRIVATE_NAMESPACE",
    ] + LOCAL_DEFINES,
    deps = [":jemalloc_headers"] + select({
        "@platforms//os:linux": ["@libunwind//:unwind"],
        "@platforms//os:windows": [],
        "@platforms//os:osx": [],
        "//conditions:default": [],
    }),
)

cc_library(
    name = "jemalloc_private",
    srcs = glob(
        ["src/*.c"],
        exclude = ["src/zone.c"],
    ) + select({
        "@bazel_template//bazel:linux_x86_64": [],
        "@bazel_template//bazel:linux_aarch64": [],
        "@bazel_template//bazel:osx_x86_64": ["src/zone.c"],
        "@bazel_template//bazel:osx_aarch64": ["src/zone.c"],
        "//conditions:default": [],
    }),
    hdrs = [
        ":jemalloc_defs_h",
        ":jemalloc_h",
        ":jemalloc_internal_defs_h",
        ":jemalloc_macros_h",
        ":jemalloc_mangle_h",
        ":jemalloc_preamble_h",
        ":jemalloc_protos_h",
        ":jemalloc_rename_h",
        ":jemalloc_typedefs_h",
        ":private_namespace_h",
    ] + glob([
        "include/jemalloc/internal/*.h",
        "include/jemalloc/*.h",
    ]) + select({
        "@platforms//os:windows": [
            "include/msvc_compat/C99/stdbool.h",
            "include/msvc_compat/C99/stdint.h",
            "include/msvc_compat/strings.h",
            "include/msvc_compat/windows_extra.h",
        ],
        "//conditions:default": [],
    }),
    copts = COPTS + select({
        "@platforms//os:windows": [
            "/Iexternal/jemalloc/include/msvc_compat",
        ],
        "//conditions:default": [],
    }),
    linkopts = ["-lpthread"],
    local_defines = LOCAL_DEFINES,
    deps = [":jemalloc_headers"] + select({
        "@platforms//os:linux": ["@libunwind//:unwind"],
        "@platforms//os:windows": [],
        "@platforms//os:osx": [],
        "//conditions:default": [],
    }),
)

cc_library(
    name = "jemalloc_cpp",
    srcs = [
        "src/jemalloc_cpp.cpp",
    ],
    hdrs = [],
    copts = [
        "-Wall",
        "-Wextra",
        "-g3",
        "-Wimplicit-fallthrough",
        "-Wdeprecated-declarations",
        "-O3",
    ],
    includes = ["include"],
    linkopts = [
        "-lpthread",
        "-lm",
    ],
    local_defines = LOCAL_DEFINES,
    deps = [":jemalloc_private"],
)

cc_library(
    name = "jemalloc",
    deps = [
        ":jemalloc_cpp",
        ":jemalloc_private",
    ],
)
