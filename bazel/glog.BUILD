load("@bazel_template//bazel:common.bzl", "dict_union", "template_rule")

common_copts = [
    "-DGLOG_BAZEL_BUILD",
    "-DGOOGLE_NAMESPACE=google",
    "-DHAVE_CXX11_NULLPTR_T",
    "-DHAVE_STDINT_H",
    "-DHAVE_STRING_H",
    "-DHAVE_UNWIND_H",
    "-I$(GENDIR)/external/com_github_glog_glog/glog_internal",
    "-DHAVE_LIB_GFLAGS",
]

wasm_copts = [
    "-Wno-sign-compare",
    "-Wno-unused-function",
    "-Wno-unused-local-typedefs",
    "-Wno-unused-variable",
    "-DHAVE_PTHREAD",
    "-DHAVE_SYS_UTSNAME_H",
    "-DHAVE_SYS_TIME_H",
    "-DHAVE_SIGACTION",
    "-DHAVE_PREAD",
    "-DHAVE___ATTRIBUTE__",
]

linux_or_darwin_copts = wasm_copts + [
    "-DHAVE_SYS_SYSCALL_H",
    "-DHAVE_UNISTD_H",
]

freebsd_only_copts = [
    "-D_GNU_SOURCE",
]

darwin_only_copts = [
    "-DHAVE_DLADDR",
]

windows_only_copts = [
    "-DHAVE_SNPRINTF",
    #"-DGLOG_NO_SYMBOLIZE_DETECTION",
    "-I" + "external/com_github_glog_glog/src/windows",
]

windows_only_srcs = [
    "src/glog/log_severity.h",
    "src/windows/dirent.h",
    "src/windows/port.cc",
    "src/windows/port.h",
]

cc_library(
    name = "glog",
    srcs = [
        "src/base/commandlineflags.h",
        "src/base/googleinit.h",
        "src/base/mutex.h",
        "src/demangle.cc",
        "src/demangle.h",
        "src/logging.cc",
        "src/raw_logging.cc",
        "src/signalhandler.cc",
        "src/stacktrace.h",
        "src/stacktrace_generic-inl.h",
        "src/stacktrace_libunwind-inl.h",
        "src/stacktrace_powerpc-inl.h",
        "src/stacktrace_windows-inl.h",
        "src/stacktrace_x86-inl.h",
        "src/stacktrace_x86_64-inl.h",
        "src/symbolize.cc",
        "src/symbolize.h",
        "src/utilities.cc",
        "src/utilities.h",
        "src/vlog_is_on.cc",
        ":config_h",
    ] + select({
        "@platforms//os:windows": windows_only_srcs,
        "//conditions:default": [],
    }),
    hdrs = [
        "src/glog/log_severity.h",
        ":logging_h",
        ":raw_logging_h",
        ":stl_logging_h",
        ":vlog_is_on_h",
    ],
    copts = select({
        "@platforms//os:windows": common_copts + windows_only_copts,
        "@platforms//os:osx": common_copts + linux_or_darwin_copts + darwin_only_copts,
        "@platforms//os:freebsd": common_copts + linux_or_darwin_copts + freebsd_only_copts,
        "//conditions:default": common_copts + linux_or_darwin_copts,
    }),
    defines = select({
        "@platforms//os:windows": [
            "GOOGLE_GLOG_DLL_DECL=__declspec(dllexport)",
            "GLOG_NO_ABBREVIATED_SEVERITIES",
            "GLOG_NO_SYMBOLIZE_DETECTION",
            "GFLAGS_DLL_DECLARE_FLAG=GOOGLE_GLOG_DLL_DECL",
            "GFLAGS_IS_A_DLL=0",
            "GLOG_BAZEL_BUILD",
        ],
        "//conditions:default": [],
    }),
    linkopts = select({
        "@platforms//os:windows": ["-ldbghelp"],
        "//conditions:default": [],
    }),
    strip_include_prefix = "src",
    visibility = ["//visibility:public"],
    deps = ["@com_github_gflags_gflags//:gflags"] + select({
        "@platforms//os:windows": [":strip_include_prefix_hack"],
        "//conditions:default": [],
    }),
)

cc_library(
    name = "strip_include_prefix_hack",
    hdrs = [
        "src/glog/log_severity.h",
        ":logging_h",
        ":raw_logging_h",
        ":stl_logging_h",
        ":vlog_is_on_h",
    ],
)

template_rule(
    name = "config_h",
    src = ":config_h_in",
    out = "glog_internal/config.h",
    substitutions = select({
        "@platforms//os:windows": {},
        "@platforms//os:linux": {},
        "@platforms//os:osx": {},
        "//conditions:default": {},
    }),
)

common_config = {
    "@ac_cv_cxx11_nullptr_t@": "1",
    "@ac_cv_cxx_using_operator@": "1",
    "@ac_cv_have_inttypes_h@": "0",
    "@ac_cv_have_u_int16_t@": "0",
    "@ac_cv_have_glog_export@": "0",
    "@ac_google_start_namespace@": "namespace google {",
    "@ac_google_end_namespace@": "}",
    "@ac_google_namespace@": "google",
}

posix_config = dict_union(
    common_config,
    {
        "@ac_cv_have_unistd_h@": "1",
        "@ac_cv_have_stdint_h@": "1",
        "@ac_cv_have_systypes_h@": "1",
        "@ac_cv_have_uint16_t@": "1",
        "@ac_cv_have___uint16@": "0",
        "@ac_cv_have___builtin_expect@": "1",
        "@ac_cv_have_libgflags@": "1",
        "@ac_cv___attribute___noinline@": "__attribute__((noinline))",
        "@ac_cv___attribute___noreturn@": "__attribute__((noreturn))",
        "@ac_cv___attribute___printf_4_5@": "__attribute__((__format__(__printf__, 4, 5)))",
    },
)

windows_config = dict_union(
    common_config,
    {
        "@ac_cv_have_unistd_h@": "1",
        "@ac_cv_have_stdint_h@": "1",
        "@ac_cv_have_systypes_h@": "1",
        "@ac_cv_have_uint16_t@": "1",
        "@ac_cv_have___uint16@": "1",
        "@ac_cv_have___builtin_expect@": "1",
        "@ac_cv_have_libgflags@": "1",
        "@ac_cv___attribute___noinline@": "__attribute__((noinline))",
        "@ac_cv___attribute___noreturn@": "__attribute__((noreturn))",
        "@ac_cv___attribute___printf_4_5@": "__attribute__((__format__(__printf__, 4, 5)))",
    },
)

template_rule(
    name = "vlog_is_on_h",
    src = "src/glog/vlog_is_on.h.in",
    out = "src/glog/vlog_is_on.h",
    substitutions = select({
        "@platforms//os:windows": windows_config,
        "//conditions:default": posix_config,
    }),
)

template_rule(
    name = "stl_logging_h",
    src = "src/glog/stl_logging.h.in",
    out = "src/glog/stl_logging.h",
    substitutions = select({
        "@platforms//os:windows": windows_config,
        "//conditions:default": posix_config,
    }),
)

template_rule(
    name = "raw_logging_h",
    src = "src/glog/raw_logging.h.in",
    out = "src/glog/raw_logging.h",
    substitutions = select({
        "@platforms//os:windows": windows_config,
        "//conditions:default": posix_config,
    }),
)

template_rule(
    name = "logging_h",
    src = "src/glog/logging.h.in",
    out = "src/glog/logging.h",
    substitutions = select({
        "@platforms//os:windows": windows_config,
        "//conditions:default": posix_config,
    }),
)

genrule(
    name = "config_h_in",
    outs = ["config.h.in"],
    cmd = "\n".join([
        "cat <<'EOF' >$@",
        "#ifndef GLOG_CONFIG_H",
        "#define GLOG_CONFIG_H",
        "",
        "/* #undef DISABLE_RTTI */",
        "#define GOOGLE_NAMESPACE google",
        "/* #undef HAVE_DLADDR */",
        "#define HAVE_SNPRINTF",
        "/* #undef HAVE_DLFCN_H */",
        "/* #undef HAVE_EXECINFO_H */",
        "/* #undef HAVE_FCNTL */",
        "/* #undef HAVE_GLOB_H */",
        "#define HAVE_INTTYPES_H 1",
        "/* #undef HAVE_LIBPTHREAD */",
        "/* #undef HAVE_LIBUNWIND_H */",
        "#define HAVE_LIB_GFLAGS",
        "/* #undef HAVE_LIB_GMOCK */",
        "/* #undef HAVE_LIB_GTEST */",
        "/* #undef HAVE_LIB_UNWIND */",
        "#define HAVE_MEMORY_H",
        "/* #undef NO_THREADS */",
        "#define HAVE_NAMESPACES",
        "/* #undef HAVE_PREAD */",
        "#define HAVE_PTHREAD",
        "/* #undef HAVE_PWD_H */",
        "/* #undef HAVE_PWRITE */",
        "#define HAVE_RWLOCK",
        "/* #undef HAVE_SIGACTION */",
        "/* #undef HAVE_SIGALTSTACK */",
        "#define HAVE_STDINT_H 1",
        "#define HAVE_STRINGS_H",
        "/* #undef HAVE_SYSCALL_H */",
        "/* #undef HAVE_SYSLOG_H */",
        "#define HAVE_SYS_STAT_H",
        "/* #undef HAVE_SYS_SYSCALL_H */",
        "/* Define to 1 if you have the <sys/time.h> header file. */",
        "#define HAVE_SYS_TIME_H",
        "#define HAVE_SYS_TYPES_H 1",
        "/* #undef HAVE_SYS_UCONTEXT_H */",
        "/* #undef HAVE_SYS_UTSNAME_H */",
        "/* #undef HAVE_SYS_WAIT_H */",
        "/* #undef HAVE_UCONTEXT_H */",
        "#define HAVE_UNISTD_H 1",
        "/* #undef HAVE_UNWIND_H */",
        "#define HAVE_USING_OPERATOR",
        "#define HAVE___ATTRIBUTE__",
        "#define HAVE___BUILTIN_EXPECT 1",
        "#define HAVE___SYNC_VAL_COMPARE_AND_SWAP",
        "/* #undef HAVE_SYMBOLIZE */",
        "/* #undef HAVE_LOCALTIME_R */",
        "/* #undef HAVE_GMTIME_R */",
        "/* #undef LT_OBJDIR */",
        "/* #undef PACKAGE */",
        "/* #undef PACKAGE_BUGREPORT */",
        "/* #undef PACKAGE_NAME */",
        "/* #undef PACKAGE_STRING */",
        "/* #undef PACKAGE_TARNAME */",
        "/* #undef PACKAGE_URL */",
        "/* #undef PACKAGE_VERSION */",
        "/* #undef PC_FROM_UCONTEXT */",
        "/* #undef PRINT_UNSYMBOLIZED_STACK_TRACES */",
        "/* #undef PTHREAD_CREATE_JOINABLE */",
        "#define SIZEOF_VOID_P 8",
        "/* #undef STDC_HEADERS */",
        "#define STL_NAMESPACE std",
        "#define GLOG_THREAD_LOCAL_STORAGE __thread",
        "#define HAVE_ALIGNED_STORAGE 1",
        "#define HAVE_CXX11_ATOMIC 1",
        "#define HAVE_CXX11_NULLPTR_T 1",
        "/* #undef VERSION */",
        "#ifdef GLOG_BAZEL_BUILD",
        "#define _START_GOOGLE_NAMESPACE_ namespace GOOGLE_NAMESPACE {",
        "#define _END_GOOGLE_NAMESPACE_ }",
        "#else",
        "#define _END_GOOGLE_NAMESPACE_ }",
        "#define _START_GOOGLE_NAMESPACE_ namespace google {",
        "#endif",
        "",
        "#endif  // GLOG_CONFIG_H",
        "EOF",
    ]),
)
