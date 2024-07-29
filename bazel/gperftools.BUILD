load("@bazel_template//bazel:common.bzl", "extract_symbols", "template_rule")

package(default_visibility = ["//visibility:public"])

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

platform(
    name = "linux_aarch64_platform",
    constraint_values = [
        "@platforms//os:linux",
        "@platforms//cpu:aarch64",
    ],
)

COPTS = [
    "-isystem external/libunwind/include",
    "-isystem $(BINDIR)/external/gperftools",
    "-isystem $(BINDIR)/external/gperftools/src",
    "-Iexternal/gperftools/src",
    "-Iexternal/gperftools/benchmark",
    "-Wall",
    "-Wwrite-strings",
    "-Woverloaded-virtual",
    "-Wno-sign-compare",
    "-fsized-deallocation",
    "-fno-omit-frame-pointer",
    "-momit-leaf-frame-pointer",
    "-pthread",
]

genrule(
    name = "config_h_in",
    outs = ["src/config.h.in"],
    cmd = "\n".join([
        "cat <<'EOF' >$@",
        "/* src/config.h.in.  Generated from configure.ac by autoheader.  */",
        "",
        "",
        "#ifndef GPERFTOOLS_CONFIG_H_",
        "#define GPERFTOOLS_CONFIG_H_",
        "",
        "",
        "/* enable aggressive decommit by default */",
        "#undef ENABLE_AGGRESSIVE_DECOMMIT_BY_DEFAULT",
        "",
        "/* Build runtime detection for sized delete */",
        "#undef ENABLE_DYNAMIC_SIZED_DELETE",
        "",
        "/* report large allocation */",
        "#undef ENABLE_LARGE_ALLOC_REPORT",
        "",
        "/* Build sized deletion operators */",
        "#undef ENABLE_SIZED_DELETE",
        "",
        "/* Define to 1 if you have the <asm/ptrace.h> header file. */",
        "#undef HAVE_ASM_PTRACE_H",
        "",
        "/* define if the compiler supports basic C++17 syntax */",
        "#undef HAVE_CXX17",
        "",
        "/* Define to 1 if you have the <cygwin/signal.h> header file. */",
        "#undef HAVE_CYGWIN_SIGNAL_H",
        "",
        "/* Define to 1 if you have the declaration of `backtrace', and to 0 if you",
        "   don't. */",
        "#undef HAVE_DECL_BACKTRACE",
        "",
        "/* Define to 1 if you have the declaration of `backtrace_symbols', and to 0 if",
        "   you don't. */",
        "#undef HAVE_DECL_BACKTRACE_SYMBOLS",
        "",
        "/* Define to 1 if you have the declaration of `cfree', and to 0 if you don't.",
        "   */",
        "#undef HAVE_DECL_CFREE",
        "",
        "/* Define to 1 if you have the declaration of `memalign', and to 0 if you",
        "   don't. */",
        "#undef HAVE_DECL_MEMALIGN",
        "",
        "/* Define to 1 if you have the declaration of `nanosleep', and to 0 if you",
        "   don't. */",
        "#undef HAVE_DECL_NANOSLEEP",
        "",
        "/* Define to 1 if you have the declaration of `posix_memalign', and to 0 if",
        "   you don't. */",
        "#undef HAVE_DECL_POSIX_MEMALIGN",
        "",
        "/* Define to 1 if you have the declaration of `pvalloc', and to 0 if you",
        "   don't. */",
        "#undef HAVE_DECL_PVALLOC",
        "",
        "/* Define to 1 if you have the declaration of `sleep', and to 0 if you don't.",
        "   */",
        "#undef HAVE_DECL_SLEEP",
        "",
        "/* Define to 1 if you have the declaration of `valloc', and to 0 if you don't.",
        "   */",
        "#undef HAVE_DECL_VALLOC",
        "",
        "/* Define to 1 if you have the <dlfcn.h> header file. */",
        "#undef HAVE_DLFCN_H",
        "",
        "/* Define to 1 if you have the <execinfo.h> header file. */",
        "#undef HAVE_EXECINFO_H",
        "",
        "/* Define to 1 if you have the <fcntl.h> header file. */",
        "#undef HAVE_FCNTL_H",
        "",
        "/* Define to 1 if you have the <features.h> header file. */",
        "#undef HAVE_FEATURES_H",
        "",
        "/* Define to 1 if you have the `geteuid' function. */",
        "#undef HAVE_GETEUID",
        "",
        "/* Define to 1 if you have the <glob.h> header file. */",
        "#undef HAVE_GLOB_H",
        "",
        "/* Define to 1 if you have the <inttypes.h> header file. */",
        "#undef HAVE_INTTYPES_H",
        "",
        "/* Define to 1 if you have the <libunwind.h> header file. */",
        "#undef HAVE_LIBUNWIND_H",
        "",
        "/* Define if this is Linux that has SIGEV_THREAD_ID */",
        "#undef HAVE_LINUX_SIGEV_THREAD_ID",
        "",
        "/* Define to 1 if you have the <malloc.h> header file. */",
        "#undef HAVE_MALLOC_H",
        "",
        "/* Define to 1 if you have a working `mmap' system call. */",
        "#undef HAVE_MMAP",
        "",
        "/* Define to 1 if you have the <poll.h> header file. */",
        "#undef HAVE_POLL_H",
        "",
        "/* define if libc has program_invocation_name */",
        "#undef HAVE_PROGRAM_INVOCATION_NAME",
        "",
        "/* Define if you have POSIX threads libraries and header files. */",
        "#undef HAVE_PTHREAD",
        "",
        "/* Have PTHREAD_PRIO_INHERIT. */",
        "#undef HAVE_PTHREAD_PRIO_INHERIT",
        "",
        "/* Define to 1 if you have the `sbrk' function. */",
        "#undef HAVE_SBRK",
        "",
        "/* Define to 1 if you have the <sched.h> header file. */",
        "#undef HAVE_SCHED_H",
        "",
        "/* Define to 1 if you have the <stdint.h> header file. */",
        "#undef HAVE_STDINT_H",
        "",
        "/* Define to 1 if you have the <stdio.h> header file. */",
        "#undef HAVE_STDIO_H",
        "",
        "/* Define to 1 if you have the <stdlib.h> header file. */",
        "#undef HAVE_STDLIB_H",
        "",
        "/* Define to 1 if you have the <strings.h> header file. */",
        "#undef HAVE_STRINGS_H",
        "",
        "/* Define to 1 if you have the <string.h> header file. */",
        "#undef HAVE_STRING_H",
        "",
        "/* Define to 1 if the system has the type `struct mallinfo'. */",
        "#undef HAVE_STRUCT_MALLINFO",
        "",
        "/* Define to 1 if the system has the type `struct mallinfo2'. */",
        "#undef HAVE_STRUCT_MALLINFO2",
        "",
        "/* Define to 1 if you have the <sys/cdefs.h> header file. */",
        "#undef HAVE_SYS_CDEFS_H",
        "",
        "/* Define to 1 if you have the <sys/socket.h> header file. */",
        "#undef HAVE_SYS_SOCKET_H",
        "",
        "/* Define to 1 if you have the <sys/stat.h> header file. */",
        "#undef HAVE_SYS_STAT_H",
        "",
        "/* Define to 1 if you have the <sys/syscall.h> header file. */",
        "#undef HAVE_SYS_SYSCALL_H",
        "",
        "/* Define to 1 if you have the <sys/types.h> header file. */",
        "#undef HAVE_SYS_TYPES_H",
        "",
        "/* Define to 1 if you have the <sys/ucontext.h> header file. */",
        "#undef HAVE_SYS_UCONTEXT_H",
        "",
        "/* Define to 1 if you have the <sys/wait.h> header file. */",
        "#undef HAVE_SYS_WAIT_H",
        "",
        "/* Define to 1 if you have the <ucontext.h> header file. */",
        "#undef HAVE_UCONTEXT_H",
        "",
        "/* Define to 1 if you have the <unistd.h> header file. */",
        "#undef HAVE_UNISTD_H",
        "",
        "/* Whether <unwind.h> contains _Unwind_Backtrace */",
        "#undef HAVE_UNWIND_BACKTRACE",
        "",
        "/* Define to 1 if you have the <unwind.h> header file. */",
        "#undef HAVE_UNWIND_H",
        "",
        "/* define if your compiler has __attribute__ */",
        "#undef HAVE___ATTRIBUTE__",
        "",
        "/* define if your compiler supports alignment of functions */",
        "#undef HAVE___ATTRIBUTE__ALIGNED_FN",
        "",
        "/* Define to 1 if compiler supports __environ */",
        "#undef HAVE___ENVIRON",
        "",
        "/* Define to the sub-directory where libtool stores uninstalled libraries. */",
        "#undef LT_OBJDIR",
        "",
        "/* Name of package */",
        "#undef PACKAGE",
        "",
        "/* Define to the address where bug reports for this package should be sent. */",
        "#undef PACKAGE_BUGREPORT",
        "",
        "/* Define to the full name of this package. */",
        "#undef PACKAGE_NAME",
        "",
        "/* Define to the full name and version of this package. */",
        "#undef PACKAGE_STRING",
        "",
        "/* Define to the one symbol short name of this package. */",
        "#undef PACKAGE_TARNAME",
        "",
        "/* Define to the home page for this package. */",
        "#undef PACKAGE_URL",
        "",
        "/* Define to the version of this package. */",
        "#undef PACKAGE_VERSION",
        "",
        "/* dllexport or attribute visibility */",
        "#undef PERFTOOLS_DLL_DECL",
        "",
        "/* if libgcc stacktrace method should be default */",
        "#undef PREFER_LIBGCC_UNWINDER",
        "",
        "/* Define to necessary symbol if this constant uses a non-standard name on",
        "   your system. */",
        "#undef PTHREAD_CREATE_JOINABLE",
        "",
        "/* Define to 1 if all of the C90 standard headers exist (not just the ones",
        "   required in a freestanding environment). This macro is provided for",
        "   backward compatibility; new code need not use it. */",
        "#undef STDC_HEADERS",
        "",
        "/* Define 8 bytes of allocation alignment for tcmalloc */",
        "#undef TCMALLOC_ALIGN_8BYTES",
        "",
        "/* Define internal page size for tcmalloc as number of left bitshift */",
        "#undef TCMALLOC_PAGE_SIZE_SHIFT",
        "",
        "/* libunwind.h was found and is working */",
        "#undef USE_LIBUNWIND",
        "",
        "/* Version number of package */",
        "#undef VERSION",
        "",
        "/* C99 says: define this to get the PRI... macros from stdint.h */",
        "#ifndef __STDC_FORMAT_MACROS",
        "# define __STDC_FORMAT_MACROS 1",
        "#endif",
        "",
        "",
        "#ifdef WIN32",
        "// TODO(csilvers): include windows/port.h in every relevant source file instead?",
        "#include \"windows/port.h\"",
        "#endif",
        "",
        "#endif  /* #ifndef GPERFTOOLS_CONFIG_H_ */",
        "",
        "EOF",
    ]),
)

template_rule(
    name = "config_h",
    src = ":config_h_in",
    out = "src/config.h",
    substitutions =
        select({
            ":linux_x86_64": {
                "#undef HAVE_SYS_CDEFS_H": "#define HAVE_SYS_CDEFS_H 1",
                "#undef HAVE_DECL_BACKTRACE\n": "/* #undef HAVE_DECL_BACKTRACE */\n",
                "#undef HAVE_DECL_BACKTRACE_SYMBOLS": "#define HAVE_DECL_BACKTRACE_SYMBOLS 1",
                "#undef HAVE___ENVIRON": "#define HAVE___ENVIRON 1",
                "#undef HAVE_DECL_PVALLOC": "#define HAVE_DECL_PVALLOC 1",
                "#undef HAVE_EXECINFO_H": "#define HAVE_EXECINFO_H 1",
                "#undef HAVE_STRUCT_MALLINFO\n": "#define HAVE_STRUCT_MALLINFO 1\n",
                "#undef HAVE_STRUCT_MALLINFO2": "#define HAVE_STRUCT_MALLINFO2 1",
            },
            ":linux_aarch64": {
                "#undef HAVE_DECL_PVALLOC": "#define HAVE_DECL_PVALLOC 0",
            },
        }) |
        {
            "#ifndef GPERFTOOLS_CONFIG_H_": "#ifndef GPERFTOOLS_CONFIG_H_",
            "#define GPERFTOOLS_CONFIG_H_": "#define GPERFTOOLS_CONFIG_H_",
            "#undef ENABLE_AGGRESSIVE_DECOMMIT_BY_DEFAULT": "/* #undef ENABLE_AGGRESSIVE_DECOMMIT_BY_DEFAULT */",
            "#undef ENABLE_DYNAMIC_SIZED_DELETE": "/* #undef ENABLE_DYNAMIC_SIZED_DELETE */",
            "#undef ENABLE_LARGE_ALLOC_REPORT": "/* #undef ENABLE_LARGE_ALLOC_REPORT */",
            "#undef ENABLE_SIZED_DELETE": "/* #undef ENABLE_SIZED_DELETE */",
            "#undef HAVE_ASM_PTRACE_H": "#define HAVE_ASM_PTRACE_H 1",
            "#undef HAVE_CXX17": "#define HAVE_CXX17 1",
            "#undef HAVE_CYGWIN_SIGNAL_H": "/* #undef HAVE_CYGWIN_SIGNAL_H */",
            "#undef HAVE_DECL_CFREE": "#define HAVE_DECL_CFREE 0",
            "#undef HAVE_DECL_MEMALIGN": "#define HAVE_DECL_MEMALIGN 1",
            "#undef HAVE_DECL_NANOSLEEP": "/* #undef HAVE_DECL_NANOSLEEP */",
            "#undef HAVE_DECL_POSIX_MEMALIGN": "#define HAVE_DECL_POSIX_MEMALIGN 1",
            "#undef HAVE_DECL_SLEEP": "/* #undef HAVE_DECL_SLEEP */",
            "#undef HAVE_DECL_VALLOC": "#define HAVE_DECL_VALLOC 1",
            "#undef HAVE_DLFCN_H": "#define HAVE_DLFCN_H 1",
            "#undef HAVE_FCNTL_H": "#define HAVE_FCNTL_H 1",
            "#undef HAVE_FEATURES_H": "#define HAVE_FEATURES_H 1",
            "#undef HAVE_GETEUID": "#define HAVE_GETEUID 1",
            "#undef HAVE_GLOB_H": "#define HAVE_GLOB_H 1",
            "#undef HAVE_INTTYPES_H": "#define HAVE_INTTYPES_H 1",
            "#undef HAVE_LIBUNWIND_H": "/* #undef HAVE_LIBUNWIND_H */",
            "#undef HAVE_LINUX_SIGEV_THREAD_ID": "#define HAVE_LINUX_SIGEV_THREAD_ID 1",
            "#undef HAVE_MALLOC_H": "#define HAVE_MALLOC_H 1",
            "#undef HAVE_MMAP": "#define HAVE_MMAP 1",
            "#undef HAVE_POLL_H": "#define HAVE_POLL_H 1",
            "#undef HAVE_PROGRAM_INVOCATION_NAME": "#define HAVE_PROGRAM_INVOCATION_NAME 1",
            "#undef HAVE_PTHREAD\n": "#define HAVE_PTHREAD 1\n",
            "#undef HAVE_PTHREAD_PRIO_INHERIT": "#define HAVE_PTHREAD_PRIO_INHERIT 1",
            "#undef HAVE_SBRK": "#define HAVE_SBRK 1",
            "#undef HAVE_SCHED_H": "#define HAVE_SCHED_H 1",
            "#undef HAVE_STDINT_H": "#define HAVE_STDINT_H 1",
            "#undef HAVE_STDIO_H": "#define HAVE_STDIO_H 1",
            "#undef HAVE_STDLIB_H": "#define HAVE_STDLIB_H 1",
            "#undef HAVE_STRINGS_H": "#define HAVE_STRINGS_H 1",
            "#undef HAVE_STRING_H": "#define HAVE_STRING_H 1",
            "#undef HAVE_SYS_SOCKET_H": "#define HAVE_SYS_SOCKET_H 1",
            "#undef HAVE_SYS_STAT_H": "#define HAVE_SYS_STAT_H 1",
            "#undef HAVE_SYS_SYSCALL_H": "#define HAVE_SYS_SYSCALL_H 1",
            "#undef HAVE_SYS_TYPES_H": "#define HAVE_SYS_TYPES_H 1",
            "#undef HAVE_SYS_UCONTEXT_H": "#define HAVE_SYS_UCONTEXT_H 1",
            "#undef HAVE_SYS_WAIT_H": "#define HAVE_SYS_WAIT_H 1",
            "#undef HAVE_UCONTEXT_H": "#define HAVE_UCONTEXT_H 1",
            "#undef HAVE_UNISTD_H": "#define HAVE_UNISTD_H 1",
            "#undef HAVE_UNWIND_BACKTRACE": "#define HAVE_UNWIND_BACKTRACE 1",
            "#undef HAVE_UNWIND_H": "#define HAVE_UNWIND_H 1",
            "#undef HAVE___ATTRIBUTE__\n": "#define HAVE___ATTRIBUTE__ 1\n",
            "#undef HAVE___ATTRIBUTE__ALIGNED_FN": "#define HAVE___ATTRIBUTE__ALIGNED_FN 1",
            "#undef LT_OBJDIR": "#define LT_OBJDIR \".libs/\"",
            "#undef PACKAGE": "#define PACKAGE \"gperftools\"",
            "#undef PACKAGE_BUGREPORT": "#define PACKAGE_BUGREPORT \"gperftools@googlegroups.com\"",
            "#undef PACKAGE_NAME": "#define PACKAGE_NAME \"gperftools\"",
            "#undef PACKAGE_STRING": "#define PACKAGE_STRING \"gperftools 2.15\"",
            "#undef PACKAGE_TARNAME": "#define PACKAGE_TARNAME \"gperftools\"",
            "#undef PACKAGE_URL": "#define PACKAGE_URL \"\"",
            "#undef PACKAGE_VERSION": "#define PACKAGE_VERSION \"2.15\"",
            "#undef PERFTOOLS_DLL_DECL": "#define PERFTOOLS_DLL_DECL /**/",
            "#undef PREFER_LIBGCC_UNWINDER": "/* #undef PREFER_LIBGCC_UNWINDER */",
            "#undef PTHREAD_CREATE_JOINABLE": "/* #undef PTHREAD_CREATE_JOINABLE */",
            "#undef STDC_HEADERS": "#define STDC_HEADERS 1",
            "#undef TCMALLOC_ALIGN_8BYTES": "/* #undef TCMALLOC_ALIGN_8BYTES */",
            "#undef TCMALLOC_PAGE_SIZE_SHIFT": "/* #undef TCMALLOC_PAGE_SIZE_SHIFT */",
            "#undef USE_LIBUNWIND": "/* #undef USE_LIBUNWIND */",
            "#undef VERSION": "#define VERSION \"2.15\"",
            "#ifndef __STDC_FORMAT_MACROS": "#ifndef __STDC_FORMAT_MACROS",
            "# define __STDC_FORMAT_MACROS 1": "# define __STDC_FORMAT_MACROS 1",
            "#ifdef WIN32": "#ifdef WIN32",
            "#include \"windows/port.h\"": "#include \"windows/port.h\"",
            "#endif\n": "#endif\n",
            "#endif  /* #ifndef GPERFTOOLS_CONFIG_H_ */": "#endif  /* #ifndef GPERFTOOLS_CONFIG_H_ */",
        },
)

genrule(
    name = "tcmalloc_h",
    outs = ["src/gperftools/tcmalloc.h"],
    cmd = "\n".join([
        "cat <<'EOF' >$@",
        "/* -*- Mode: C; c-basic-offset: 2; indent-tabs-mode: nil -*- */",
        "/* Copyright (c) 2003, Google Inc.",
        " * All rights reserved.",
        " *",
        " * Redistribution and use in source and binary forms, with or without",
        " * modification, are permitted provided that the following conditions are",
        " * met:",
        " *",
        " *     * Redistributions of source code must retain the above copyright",
        " * notice, this list of conditions and the following disclaimer.",
        " *     * Redistributions in binary form must reproduce the above",
        " * copyright notice, this list of conditions and the following disclaimer",
        " * in the documentation and/or other materials provided with the",
        " * distribution.",
        " *     * Neither the name of Google Inc. nor the names of its",
        " * contributors may be used to endorse or promote products derived from",
        " * this software without specific prior written permission.",
        " *",
        " * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS",
        " * \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT",
        " * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR",
        " * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT",
        " * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,",
        " * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT",
        " * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,",
        " * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY",
        " * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT",
        " * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE",
        " * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.",
        " *",
        " * ---",
        " * Author: Sanjay Ghemawat <opensource@google.com>",
        " *         .h file by Craig Silverstein <opensource@google.com>",
        " */",
        "",
        "#ifndef TCMALLOC_TCMALLOC_H_",
        "#define TCMALLOC_TCMALLOC_H_",
        "",
        "#include <stddef.h>                     /* for size_t */",
        "#ifdef __cplusplus",
        "#include <new>                          /* for std::nothrow_t, std::align_val_t */",
        "#endif",
        "",
        "/* Define the version number so folks can check against it */",
        "#define TC_VERSION_MAJOR  2",
        "#define TC_VERSION_MINOR  15",
        "#define TC_VERSION_PATCH  \"\"",
        "#define TC_VERSION_STRING \"gperftools 2.15\"",
        "",
        "#ifndef _WIN32",
        "/* For struct mallinfo, if it's defined. */",
        "#if 1 || 1 ",
        "# include <malloc.h>",
        "#endif",
        "#endif",
        "",
        "#ifndef PERFTOOLS_NOTHROW",
        "",
        "#ifdef __cplusplus",
        "#define PERFTOOLS_NOTHROW noexcept",
        "#else",
        "# ifdef __GNUC__",
        "#  define PERFTOOLS_NOTHROW __attribute__((__nothrow__))",
        "# else",
        "#  define PERFTOOLS_NOTHROW",
        "# endif",
        "#endif",
        "",
        "#endif",
        "",
        "#ifndef PERFTOOLS_DLL_DECL",
        "# ifdef _WIN32",
        "#   define PERFTOOLS_DLL_DECL  __declspec(dllimport)",
        "# else",
        "#   define PERFTOOLS_DLL_DECL",
        "# endif",
        "#endif",
        "",
        "#ifdef __cplusplus",
        "extern \"C\" {",
        "#endif",
        "  /*",
        "   * Returns a human-readable version string.  If major, minor,",
        "   * and/or patch are not NULL, they are set to the major version,",
        "   * minor version, and patch-code (a string, usually \"\").",
        "   */",
        "  PERFTOOLS_DLL_DECL const char* tc_version(int* major, int* minor,",
        "                                            const char** patch) PERFTOOLS_NOTHROW;",
        "",
        "  PERFTOOLS_DLL_DECL void* tc_malloc(size_t size) PERFTOOLS_NOTHROW;",
        "  PERFTOOLS_DLL_DECL void* tc_malloc_skip_new_handler(size_t size) PERFTOOLS_NOTHROW;",
        "  PERFTOOLS_DLL_DECL void tc_free(void* ptr) PERFTOOLS_NOTHROW;",
        "  PERFTOOLS_DLL_DECL void tc_free_sized(void *ptr, size_t size) PERFTOOLS_NOTHROW;",
        "  PERFTOOLS_DLL_DECL void* tc_realloc(void* ptr, size_t size) PERFTOOLS_NOTHROW;",
        "  PERFTOOLS_DLL_DECL void* tc_calloc(size_t nmemb, size_t size) PERFTOOLS_NOTHROW;",
        "  PERFTOOLS_DLL_DECL void tc_cfree(void* ptr) PERFTOOLS_NOTHROW;",
        "",
        "  PERFTOOLS_DLL_DECL void* tc_memalign(size_t __alignment,",
        "                                       size_t __size) PERFTOOLS_NOTHROW;",
        "  PERFTOOLS_DLL_DECL int tc_posix_memalign(void** ptr,",
        "                                           size_t align, size_t size) PERFTOOLS_NOTHROW;",
        "  PERFTOOLS_DLL_DECL void* tc_valloc(size_t __size) PERFTOOLS_NOTHROW;",
        "  PERFTOOLS_DLL_DECL void* tc_pvalloc(size_t __size) PERFTOOLS_NOTHROW;",
        "",
        "  PERFTOOLS_DLL_DECL void tc_malloc_stats(void) PERFTOOLS_NOTHROW;",
        "  PERFTOOLS_DLL_DECL int tc_mallopt(int cmd, int value) PERFTOOLS_NOTHROW;",
        "",
        "#ifndef _WIN32",
        "#if 1",
        "  PERFTOOLS_DLL_DECL struct mallinfo tc_mallinfo(void) PERFTOOLS_NOTHROW;",
        "#endif",
        "#if 1",
        "  PERFTOOLS_DLL_DECL struct mallinfo2 tc_mallinfo2(void) PERFTOOLS_NOTHROW;",
        "#endif",
        "#endif",
        "",
        "  /*",
        "   * This is an alias for MallocExtension::instance()->GetAllocatedSize().",
        "   * It is equivalent to",
        "   *    OS X: malloc_size()",
        "   *    glibc: malloc_usable_size()",
        "   *    Windows: _msize()",
        "   */",
        "  PERFTOOLS_DLL_DECL size_t tc_malloc_size(void* ptr) PERFTOOLS_NOTHROW;",
        "",
        "#ifdef __cplusplus",
        "  PERFTOOLS_DLL_DECL int tc_set_new_mode(int flag) PERFTOOLS_NOTHROW;",
        "  PERFTOOLS_DLL_DECL void* tc_new(size_t size);",
        "  PERFTOOLS_DLL_DECL void* tc_new_nothrow(size_t size,",
        "                                          const std::nothrow_t&) PERFTOOLS_NOTHROW;",
        "  PERFTOOLS_DLL_DECL void tc_delete(void* p) PERFTOOLS_NOTHROW;",
        "  PERFTOOLS_DLL_DECL void tc_delete_sized(void* p, size_t size) PERFTOOLS_NOTHROW;",
        "  PERFTOOLS_DLL_DECL void tc_delete_nothrow(void* p,",
        "                                            const std::nothrow_t&) PERFTOOLS_NOTHROW;",
        "  PERFTOOLS_DLL_DECL void* tc_newarray(size_t size);",
        "  PERFTOOLS_DLL_DECL void* tc_newarray_nothrow(size_t size,",
        "                                               const std::nothrow_t&) PERFTOOLS_NOTHROW;",
        "  PERFTOOLS_DLL_DECL void tc_deletearray(void* p) PERFTOOLS_NOTHROW;",
        "  PERFTOOLS_DLL_DECL void tc_deletearray_sized(void* p, size_t size) PERFTOOLS_NOTHROW;",
        "  PERFTOOLS_DLL_DECL void tc_deletearray_nothrow(void* p,",
        "                                                 const std::nothrow_t&) PERFTOOLS_NOTHROW;",
        "",
        "#if defined(__cpp_aligned_new) || \\",
        "    (defined(__cplusplus) && __cplusplus >= 201703L) || \\",
        "    (defined(_MSVC_LANG) && _MSVC_LANG >= 201703L)",
        "  PERFTOOLS_DLL_DECL void* tc_new_aligned(size_t size, std::align_val_t al);",
        "  PERFTOOLS_DLL_DECL void* tc_new_aligned_nothrow(size_t size, std::align_val_t al,",
        "                                          const std::nothrow_t&) PERFTOOLS_NOTHROW;",
        "  PERFTOOLS_DLL_DECL void tc_delete_aligned(void* p, std::align_val_t al) PERFTOOLS_NOTHROW;",
        "  PERFTOOLS_DLL_DECL void tc_delete_sized_aligned(void* p, size_t size, std::align_val_t al) PERFTOOLS_NOTHROW;",
        "  PERFTOOLS_DLL_DECL void tc_delete_aligned_nothrow(void* p, std::align_val_t al,",
        "                                            const std::nothrow_t&) PERFTOOLS_NOTHROW;",
        "  PERFTOOLS_DLL_DECL void* tc_newarray_aligned(size_t size, std::align_val_t al);",
        "  PERFTOOLS_DLL_DECL void* tc_newarray_aligned_nothrow(size_t size, std::align_val_t al,",
        "                                               const std::nothrow_t&) PERFTOOLS_NOTHROW;",
        "  PERFTOOLS_DLL_DECL void tc_deletearray_aligned(void* p, std::align_val_t al) PERFTOOLS_NOTHROW;",
        "  PERFTOOLS_DLL_DECL void tc_deletearray_sized_aligned(void* p, size_t size, std::align_val_t al) PERFTOOLS_NOTHROW;",
        "  PERFTOOLS_DLL_DECL void tc_deletearray_aligned_nothrow(void* p, std::align_val_t al,",
        "                                                 const std::nothrow_t&) PERFTOOLS_NOTHROW;",
        "#endif",
        "",
        "}",
        "#endif",
        "",
        "/* We're only un-defining for public */",
        "#if !defined(GPERFTOOLS_CONFIG_H_)",
        "",
        "#undef PERFTOOLS_NOTHROW",
        "",
        "#endif /* GPERFTOOLS_CONFIG_H_ */",
        "",
        "#endif  /* #ifndef TCMALLOC_TCMALLOC_H_ */",
        "EOF",
    ]),
)

cc_library(
    name = "tcmalloc_and_profiler",
    srcs = [
        "src/base/dynamic_annotations.cc",
        "src/base/elf_mem_image.cc",
        "src/base/generic_writer.cc",
        "src/base/logging.cc",
        "src/base/proc_maps_iterator.cc",
        "src/base/spinlock.cc",
        "src/base/spinlock_internal.cc",
        "src/base/sysinfo.cc",
        "src/base/vdso_support.cc",
        "src/profile-handler.cc",
        "src/profiledata.cc",
        "src/profiler.cc",
        "src/stacktrace.cc",
    ] + [
        "src/base/low_level_alloc.cc",
        "src/central_freelist.cc",
        "src/common.cc",
        "src/emergency_malloc.cc",
        "src/heap-checker.cc",
        "src/heap-profile-table.cc",
        "src/heap-profiler.cc",
        "src/internal_logging.cc",
        "src/malloc_backtrace.cc",
        "src/malloc_extension.cc",
        "src/malloc_hook.cc",
        "src/memfs_malloc.cc",
        "src/memory_region_map.cc",
        "src/mmap_hook.cc",
        "src/page_heap.cc",
        "src/safe_strerror.cc",
        "src/sampler.cc",
        "src/span.cc",
        "src/stack_trace_table.cc",
        "src/static_vars.cc",
        "src/symbolize.cc",
        "src/system-alloc.cc",
        "src/tcmalloc.cc",
        "src/thread_cache.cc",
        "src/thread_cache_ptr.cc",
        ":config_h",
        ":tcmalloc_h",
    ],
    hdrs = glob([
        "src/base/**/*.h",
        "src/gperftools/**/*.h",
        "src/*.h",
    ]) + [
        ":config_h",
    ],
    copts = COPTS,
    local_defines = [
        "ENABLE_EMERGENCY_MALLOC",
        "HAVE_CONFIG_H",
        "NDEBUG",
        "FORCED_FRAME_POINTERS",
    ],
    deps = [
        "@libunwind//:unwind-all",
    ],
)

cc_library(
    name = "tcmalloc_debug_and_profiler",
    srcs = [
        "src/base/dynamic_annotations.cc",
        "src/base/elf_mem_image.cc",
        "src/base/generic_writer.cc",
        "src/base/logging.cc",
        "src/base/proc_maps_iterator.cc",
        "src/base/spinlock.cc",
        "src/base/spinlock_internal.cc",
        "src/base/sysinfo.cc",
        "src/base/vdso_support.cc",
        "src/profiledata.cc",
        "src/profiler.cc",
        "src/stacktrace.cc",
    ] + [
        "src/base/linuxthreads.cc",
        "src/base/low_level_alloc.cc",
        "src/central_freelist.cc",
        "src/common.cc",
        "src/debugallocation.cc",
        "src/emergency_malloc.cc",
        "src/heap-checker.cc",
        "src/heap-profile-table.cc",
        "src/heap-profiler.cc",
        "src/internal_logging.cc",
        "src/malloc_backtrace.cc",
        "src/malloc_extension.cc",
        "src/malloc_hook.cc",
        "src/memfs_malloc.cc",
        "src/memory_region_map.cc",
        "src/mmap_hook.cc",
        "src/page_heap.cc",
        "src/safe_strerror.cc",
        "src/sampler.cc",
        "src/span.cc",
        "src/stack_trace_table.cc",
        "src/static_vars.cc",
        "src/symbolize.cc",
        "src/system-alloc.cc",
        "src/thread_cache.cc",
        "src/thread_cache_ptr.cc",
        ":config_h",
        ":tcmalloc_h",
    ],
    hdrs = glob([
        "src/base/**/*.h",
        "src/gperftools/**/*.h",
        "src/*.h",
    ]) + [
        "src/tcmalloc.cc",
        ":config_h",
    ],
    copts = COPTS,
    local_defines = [
        "HAVE_CONFIG_H",
        "ENABLE_EMERGENCY_MALLOC",
        "NDEBUG",
        "FORCED_FRAME_POINTERS",
    ],
    deps = [
        "@libunwind//:unwind-all",
    ],
)

cc_library(
    name = "run_benchmark",
    srcs = [
        "benchmark/run_benchmark.cc",
        ":config_h",
        ":tcmalloc_h",
    ],
    hdrs = glob([
        "src/base/**/*.h",
        "benchmark/**/*.h",
        "src/gperftools/**/*.h",
        "src/*.h",
    ]),
    copts = COPTS,
    local_defines = [
        "ENABLE_EMERGENCY_MALLOC",
        "HAVE_CONFIG_H",
        "NDEBUG",
        "FORCED_FRAME_POINTERS",
    ],
    deps = [
        ":tcmalloc_and_profiler",
    ],
)

cc_binary(
    name = "malloc_bench",
    srcs = [
        "benchmark/malloc_bench.cc",
        ":config_h",
        ":tcmalloc_h",
    ],
    copts = COPTS,
    local_defines = [
        "ENABLE_EMERGENCY_MALLOC",
        "HAVE_CONFIG_H",
        "NDEBUG",
        "FORCED_FRAME_POINTERS",
    ],
    deps = [
        ":run_benchmark",
        ":tcmalloc_and_profiler",
    ],
)

cc_binary(
    name = "binary_trees",
    srcs = [
        "benchmark/binary_trees.cc",
        ":config_h",
        ":tcmalloc_h",
    ],
    copts = COPTS,
    local_defines = [
        "ENABLE_EMERGENCY_MALLOC",
        "HAVE_CONFIG_H",
        "NDEBUG",
        "FORCED_FRAME_POINTERS",
    ],
    deps = [
        ":tcmalloc_and_profiler",
    ],
)
