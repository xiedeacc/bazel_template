load("@bazel_skylib//lib:selects.bzl", "selects")
load("@bazel_skylib//rules:copy_file.bzl", "copy_file")
load("@bazel_template//bazel:common.bzl", "GLOBAL_COPTS", "GLOBAL_LINKOPTS", "template_rule")

package(default_visibility = ["//visibility:public"])

COPTS = GLOBAL_COPTS + select({
    "@bazel_template//bazel:not_cross_compiling_on_windows": [
        "/std:c11",
        "/Iexternal/xz/src/common",
        "/Iexternal/xz/src/liblzma/api",  #https://gcc.gnu.org/onlinedocs/gcc/Directory-Options.html
        "/Iexternal/xz/src/liblzma/check",
        "/Iexternal/xz/src/liblzma/common",
        "/Iexternal/xz/src/liblzma/delta",
        "/Iexternal/xz/src/liblzma/lz",
        "/Iexternal/xz/src/liblzma/lzma",
        "/Iexternal/xz/src/liblzma/rangecoder",
        "/Iexternal/xz/src/liblzma/simple",
    ],
    "//conditions:default": [
        "-Iexternal/xz/src/common",
        "-iquote external/xz/src/liblzma/api",  #https://gcc.gnu.org/onlinedocs/gcc/Directory-Options.html
        "-Iexternal/xz/src/liblzma/check",
        "-Iexternal/xz/src/liblzma/common",
        "-Iexternal/xz/src/liblzma/delta",
        "-Iexternal/xz/src/liblzma/lz",
        "-Iexternal/xz/src/liblzma/lzma",
        "-Iexternal/xz/src/liblzma/rangecoder",
        "-Iexternal/xz/src/liblzma/simple",
        "-std=c11",
        "-g",
    ],
})

LOCAL_DEFINES = [
    "HAVE_CHECK_CRC32",
    "HAVE_CHECK_CRC64",
    "HAVE_CHECK_SHA256",
    "HAVE_DECODERS",
    "HAVE_DECODER_LZMA1",
    "HAVE_DECODER_LZMA2",
    "HAVE_ENCODERS",
    "HAVE_ENCODER_LZMA1",
    "HAVE_ENCODER_LZMA2",
    "HAVE_LZIP_DECODER",
    "HAVE_MF_BT2",
    "HAVE_MF_BT3",
    "HAVE_MF_BT4",
    "HAVE_MF_HC3",
    "HAVE_MF_HC4",
    "HAVE_STDLIB_H",
    "HAVE_STDIO_H",
    "HAVE_STDBOOL_H",
    "HAVE__BOOL",
    "HAVE_STDINT_H",
    "HAVE_INTTYPES_H",
    "HAVE_SYS_STAT_H",
    "HAVE_SYS_TYPES_H",
    "HAVE__MM_MOVEMASK_EPI8",
    "TUKLIB_FAST_UNALIGNED_ACCESS",
    "HAVE___BUILTIN_ASSUME_ALIGNED",
    "NDEBUG",
    "PACKAGE_BUGREPORT=\\\"xz@tukaani.org\\\"",
    "PACKAGE_NAME=\\\"XZUtils\\\"",
    "PACKAGE_URL=\\\"https://tukaani.org/xz/\\\"",
    "TUKLIB_PHYSMEM_SYSCONF",
    "TUKLIB_SYMBOL_PREFIX=lzma_",
] + select({
    "@bazel_template//bazel:osx_x86_64": [
        "HAVE___BUILTIN_BSWAPXX",
        "HAVE_FUNC_ATTRIBUTE_CONSTRUCTOR",
        "HAVE_STRINGS_H",
        "HAVE_STDIO_H",
        "HAVE_CLOCK_GETTIME",
        "HAVE_CLOCK_MONOTONIC",
        "HAVE_CPUID_H",
        "HAVE_WCHAR_H",
        "SIZEOF_SIZE_T=8",
        "ENABLE_NLS=1",
        "HAVE_SYS_CDEFS_H",
        "HAVE_SYS_PARAM_H",
        "HAVE_UNISTD_H",
        "HAVE_CFLOCALECOPYPREFERREDLANGUAGES=1",
        "HAVE_CFPREFERENCESCOPYAPPVALUE=1",
        "HAVE_DLFCN_H=1",
        "HAVE_DECODER_X86",
        "HAVE_ENCODER_X86",
        "HAVE_FUTIMENS=1",
        "HAVE_GETOPT_H=1",
        "HAVE_GETOPT_LONG=1",
        "HAVE_GETTEXT=1",
        "HAVE_ICONV=1",
        "HAVE_IMMINTRIN_H",
        "HAVE_MBRTOWC=1",
        "HAVE_OPTRESET=1",
        "HAVE_PTHREAD_PRIO_INHERIT",
        "HAVE_STRUCT_STAT_ST_ATIMESPEC_TV_NSEC",
        "HAVE_UINTPTR_T",
        "HAVE_USABLE_CLMUL",
        "HAVE_VISIBILITY=1",
        "HAVE_WCWIDTH=1",
        "STDC_HEADERS=1",
        "MYTHREAD_POSIX",
        "TUKLIB_CPUCORES_SYSCTL=1",
        "_DARWIN_C_SOURCE",
        "__STDC_WANT_IEC_60559_ATTRIBS_EXT__=1",
        "__STDC_WANT_IEC_60559_BFP_EXT__=1",
        "__STDC_WANT_IEC_60559_DFP_EXT__=1",
        "__STDC_WANT_IEC_60559_EXT__=1",
        "__STDC_WANT_IEC_60559_FUNCS_EXT__=1",
        "__STDC_WANT_IEC_60559_TYPES_EXT__=1",
        "__STDC_WANT_LIB_EXT2__=1",
        "__STDC_WANT_MATH_SPEC_FUNCS__=1",
    ],
    "@bazel_template//bazel:osx_aarch64": [
        "HAVE___BUILTIN_BSWAPXX",
        "HAVE_FUNC_ATTRIBUTE_CONSTRUCTOR",
        "HAVE_STRINGS_H",
        "HAVE_WCHAR_H",
        "HAVE_CLOCK_GETTIME",
        "HAVE_CLOCK_MONOTONIC",
        "HAVE_CPUID_H",
        "SIZEOF_SIZE_T=8",
        "ENABLE_NLS=1",
        "HAVE_ENCODER_ARM64",
        "HAVE_DECODER_ARM64",
        "MYTHREAD_POSIX",
        "HAVE_SYS_CDEFS_H",
        "HAVE_SYS_PARAM_H",
        "HAVE_UNISTD_H",
    ],
    "@bazel_template//bazel:linux_x86_64": [
        "HAVE___BUILTIN_BSWAPXX",
        "HAVE_FUNC_ATTRIBUTE_CONSTRUCTOR",
        "HAVE_STRINGS_H",
        "HAVE_CLOCK_GETTIME",
        "HAVE_CLOCK_MONOTONIC",
        "HAVE_CPUID_H",
        "HAVE_SYS_CDEFS_H",
        "HAVE_SYS_PARAM_H",
        "HAVE_UNISTD_H",
        "HAVE_WCHAR_H",
        "SIZEOF_SIZE_T=8",
        "ENABLE_NLS=1",
        "HAVE_VISIBILITY=0",
        "TUKLIB_CPUCORES_SCHED_GETAFFINITY",
        "HAVE_PTHREAD_CONDATTR_SETCLOCK",
        "HAVE_DECODER_X86",
        "HAVE_ENCODER_X86",
        "HAVE_USABLE_CLMUL",
        "HAVE_IMMINTRIN_H",
        "_GNU_SOURCE",
        "MYTHREAD_POSIX",
    ],
    "@bazel_template//bazel:linux_aarch64": [
        "HAVE___BUILTIN_BSWAPXX",
        "HAVE_FUNC_ATTRIBUTE_CONSTRUCTOR",
        "HAVE_STRINGS_H",
        "HAVE_CLOCK_GETTIME",
        "HAVE_CLOCK_MONOTONIC",
        "HAVE_CPUID_H",
        "HAVE_SYS_CDEFS_H",
        "HAVE_SYS_PARAM_H",
        "HAVE_UNISTD_H",
        "HAVE_WCHAR_H",
        "SIZEOF_SIZE_T=8",
        "ENABLE_NLS=1",
        "HAVE_PTHREAD_CONDATTR_SETCLOCK",
        "HAVE_ENCODER_ARM64",
        "HAVE_DECODER_ARM64",
        "_GNU_SOURCE",
        "MYTHREAD_POSIX",
    ],
    "@bazel_template//bazel:windows_x86_64_gcc": [
        "HAVE___BUILTIN_BSWAPXX",
        "HAVE_FUNC_ATTRIBUTE_CONSTRUCTOR",
        "HAVE_STRINGS_H",
        "HAVE_CLOCK_GETTIME",
        "HAVE_CLOCK_MONOTONIC",
        "HAVE_CPUID_H",
        "HAVE_SYS_CDEFS_H",
        "HAVE_SYS_PARAM_H",
        "HAVE_UNISTD_H",
        "HAVE_WCHAR_H",
        "SIZEOF_SIZE_T=8",
        "ENABLE_NLS=1",
        "MYTHREAD_POSIX",
        "HAVE_ENCODER_X86",
        "HAVE_DECODER_X86",
    ],
    "@bazel_template//bazel:windows_x86_64_msvc": [
        "HAVE_IMMINTRIN_H",
        "HAVE_USABLE_CLMUL",
        "HAVE_VISIBILITY=1",
        "MYTHREAD_VISTA",
        "HAVE_ENCODER_X86",
        "HAVE_DECODER_X86",
    ],
    "//conditions:default": [
    ],
})

cc_library(
    name = "lzma",
    srcs = select({
        "@platforms//cpu:aarch64": ["src/liblzma/simple/arm64.c"],
        "@platforms//cpu:riscv64": ["src/liblzma/simple/riscv.c"],
        "//conditions:default": ["src/liblzma/simple/x86.c"],
    }) + [
        "src/common/tuklib_cpucores.c",
        "src/common/tuklib_physmem.c",
        "src/liblzma/check/check.c",
        "src/liblzma/check/crc32_fast.c",
        #"src/liblzma/check/crc_clmul_consts_gen.c",
        "src/liblzma/check/crc64_fast.c",
        "src/liblzma/check/sha256.c",
        "src/liblzma/common/alone_decoder.c",
        "src/liblzma/common/alone_encoder.c",
        "src/liblzma/common/auto_decoder.c",
        "src/liblzma/common/block_buffer_decoder.c",
        "src/liblzma/common/block_buffer_encoder.c",
        "src/liblzma/common/block_decoder.c",
        "src/liblzma/common/block_encoder.c",
        "src/liblzma/common/block_header_decoder.c",
        "src/liblzma/common/block_header_encoder.c",
        "src/liblzma/common/block_util.c",
        "src/liblzma/common/common.c",
        "src/liblzma/common/easy_buffer_encoder.c",
        "src/liblzma/common/easy_decoder_memusage.c",
        "src/liblzma/common/easy_encoder.c",
        "src/liblzma/common/easy_encoder_memusage.c",
        "src/liblzma/common/easy_preset.c",
        "src/liblzma/common/file_info.c",
        "src/liblzma/common/filter_buffer_decoder.c",
        "src/liblzma/common/filter_buffer_encoder.c",
        "src/liblzma/common/filter_common.c",
        "src/liblzma/common/filter_decoder.c",
        "src/liblzma/common/filter_encoder.c",
        "src/liblzma/common/filter_flags_decoder.c",
        "src/liblzma/common/filter_flags_encoder.c",
        "src/liblzma/common/hardware_cputhreads.c",
        "src/liblzma/common/hardware_physmem.c",
        "src/liblzma/common/index.c",
        "src/liblzma/common/index_decoder.c",
        "src/liblzma/common/index_encoder.c",
        "src/liblzma/common/index_hash.c",
        "src/liblzma/common/lzip_decoder.c",
        "src/liblzma/common/microlzma_decoder.c",
        "src/liblzma/common/microlzma_encoder.c",
        "src/liblzma/common/outqueue.c",
        "src/liblzma/common/stream_buffer_decoder.c",
        "src/liblzma/common/stream_buffer_encoder.c",
        "src/liblzma/common/stream_decoder.c",
        "src/liblzma/common/stream_decoder_mt.c",
        "src/liblzma/common/stream_encoder.c",
        "src/liblzma/common/stream_encoder_mt.c",
        "src/liblzma/common/stream_flags_common.c",
        "src/liblzma/common/stream_flags_decoder.c",
        "src/liblzma/common/stream_flags_encoder.c",
        "src/liblzma/common/string_conversion.c",
        "src/liblzma/common/vli_decoder.c",
        "src/liblzma/common/vli_encoder.c",
        "src/liblzma/common/vli_size.c",
        "src/liblzma/delta/delta_common.c",
        "src/liblzma/delta/delta_decoder.c",
        "src/liblzma/delta/delta_encoder.c",
        "src/liblzma/lz/lz_decoder.c",
        "src/liblzma/lz/lz_encoder.c",
        "src/liblzma/lz/lz_encoder_mf.c",
        "src/liblzma/lzma/fastpos_table.c",
        "src/liblzma/lzma/lzma2_decoder.c",
        "src/liblzma/lzma/lzma2_encoder.c",
        "src/liblzma/lzma/lzma_decoder.c",
        "src/liblzma/lzma/lzma_encoder.c",
        "src/liblzma/lzma/lzma_encoder_optimum_fast.c",
        "src/liblzma/lzma/lzma_encoder_optimum_normal.c",
        "src/liblzma/lzma/lzma_encoder_presets.c",
        "src/liblzma/rangecoder/price_table.c",
        "src/liblzma/simple/simple_coder.c",
        "src/liblzma/simple/simple_decoder.c",
        "src/liblzma/simple/simple_encoder.c",
    ],
    hdrs = select({
        "@platforms//cpu:x86_64": [
            "src/liblzma/check/crc_x86_clmul.h",
        ],
        "//conditions:default": [],
    }) + [
        "src/common/mythread.h",
        "src/common/sysdefs.h",
        "src/common/tuklib_common.h",
        "src/common/tuklib_config.h",
        "src/common/tuklib_cpucores.h",
        "src/common/tuklib_exit.h",
        "src/common/tuklib_gettext.h",
        "src/common/tuklib_integer.h",
        "src/common/tuklib_mbstr.h",
        "src/common/tuklib_open_stdxxx.h",
        "src/common/tuklib_physmem.h",
        "src/common/tuklib_progname.h",
        "src/liblzma/api/lzma.h",
        "src/liblzma/api/lzma/base.h",
        "src/liblzma/api/lzma/bcj.h",
        "src/liblzma/api/lzma/block.h",
        "src/liblzma/api/lzma/check.h",
        "src/liblzma/api/lzma/container.h",
        "src/liblzma/api/lzma/delta.h",
        "src/liblzma/api/lzma/filter.h",
        "src/liblzma/api/lzma/hardware.h",
        "src/liblzma/api/lzma/index.h",
        "src/liblzma/api/lzma/index_hash.h",
        "src/liblzma/api/lzma/lzma12.h",
        "src/liblzma/api/lzma/stream_flags.h",
        "src/liblzma/api/lzma/version.h",
        "src/liblzma/api/lzma/vli.h",
        "src/liblzma/check/check.h",
        "src/liblzma/check/crc32_arm64.h",
        "src/liblzma/check/crc32_table_be.h",
        "src/liblzma/check/crc32_table_le.h",
        "src/liblzma/check/crc64_table_be.h",
        "src/liblzma/check/crc64_table_le.h",
        "src/liblzma/check/crc_common.h",
        "src/liblzma/common/alone_decoder.h",
        "src/liblzma/common/block_buffer_encoder.h",
        "src/liblzma/common/block_decoder.h",
        "src/liblzma/common/block_encoder.h",
        "src/liblzma/common/common.h",
        "src/liblzma/common/easy_preset.h",
        "src/liblzma/common/filter_common.h",
        "src/liblzma/common/filter_decoder.h",
        "src/liblzma/common/filter_encoder.h",
        "src/liblzma/common/index.h",
        "src/liblzma/common/index_decoder.h",
        "src/liblzma/common/index_encoder.h",
        "src/liblzma/common/lzip_decoder.h",
        "src/liblzma/common/memcmplen.h",
        "src/liblzma/common/outqueue.h",
        "src/liblzma/common/stream_decoder.h",
        "src/liblzma/common/stream_flags_common.h",
        "src/liblzma/delta/delta_common.h",
        "src/liblzma/delta/delta_decoder.h",
        "src/liblzma/delta/delta_encoder.h",
        "src/liblzma/delta/delta_private.h",
        "src/liblzma/lz/lz_decoder.h",
        "src/liblzma/lz/lz_encoder.h",
        "src/liblzma/lz/lz_encoder_hash.h",
        "src/liblzma/lz/lz_encoder_hash_table.h",
        "src/liblzma/lzma/fastpos.h",
        "src/liblzma/lzma/lzma2_decoder.h",
        "src/liblzma/lzma/lzma2_encoder.h",
        "src/liblzma/lzma/lzma_common.h",
        "src/liblzma/lzma/lzma_decoder.h",
        "src/liblzma/lzma/lzma_encoder.h",
        "src/liblzma/lzma/lzma_encoder_private.h",
        "src/liblzma/rangecoder/price.h",
        "src/liblzma/rangecoder/range_common.h",
        "src/liblzma/rangecoder/range_decoder.h",
        "src/liblzma/rangecoder/range_encoder.h",
        "src/liblzma/simple/simple_coder.h",
        "src/liblzma/simple/simple_decoder.h",
        "src/liblzma/simple/simple_encoder.h",
        "src/liblzma/simple/simple_private.h",
    ],
    copts = COPTS,
    includes = ["src/liblzma/api"],
    linkopts = select({
        "@platforms//os:android": [],
        "@platforms//os:windows": [],
        "//conditions:default": ["-pthread"],
    }),
    # linkstatic = select({
    #     "@platforms//os:windows": True,
    #     "//conditions:default": False,
    # }),
    local_defines = LOCAL_DEFINES,
    #alwayslink = True,
)

cc_binary(
    name = "xz",
    srcs = [
        "src/common/tuklib_exit.c",
        "src/common/tuklib_mbstr_fw.c",
        "src/common/tuklib_mbstr_width.c",
        "src/common/tuklib_open_stdxxx.c",
        "src/common/tuklib_progname.c",
        "src/liblzma/api/lzma/version.h",
        "src/xz/args.c",
        "src/xz/args.h",
        "src/xz/coder.c",
        "src/xz/coder.h",
        "src/xz/file_io.c",
        "src/xz/file_io.h",
        "src/xz/hardware.c",
        "src/xz/hardware.h",
        "src/xz/list.c",
        "src/xz/list.h",
        "src/xz/main.c",
        "src/xz/main.h",
        "src/xz/message.c",
        "src/xz/message.h",
        "src/xz/mytime.c",
        "src/xz/mytime.h",
        "src/xz/options.c",
        "src/xz/options.h",
        "src/xz/private.h",
        "src/xz/sandbox.c",
        "src/xz/sandbox.h",
        "src/xz/signals.c",
        "src/xz/signals.h",
        "src/xz/suffix.c",
        "src/xz/suffix.h",
        "src/xz/util.c",
        "src/xz/util.h",
    ] + select({
        "@bazel_template//bazel:not_cross_compiling_on_windows": [
            "lib/getopt.c",
            "lib/getopt-cdefs.h",
            "lib/getopt-core.h",
            "lib/getopt-ext.h",
            "lib/getopt-pfx-core.h",
            "lib/getopt-pfx-ext.h",
            "lib/getopt1.c",
            "lib/getopt_int.h",
            ":getopt_h",
        ],
        "//conditions:default": [],
    }),
    copts = COPTS + select({
        "@bazel_template//bazel:not_cross_compiling_on_windows": [
            "/Iexternal/xz/lib",
        ],
        "//conditions:default": [],
    }),
    local_defines = LOCAL_DEFINES + [
        "ASSUME_RAM=128",
        "HAVE_MBRTOWC",
        "HAVE__FUTIME",
        "__GETOPT_PREFIX=rpl_",
        "WIN32",
        "_WIN32",
        "_MBCS",
        "_WINDOWS",
    ],
    deps = [":lzma"],
)

genrule(
    name = "getopt_h",
    outs = ["getopt.h"],
    cmd = "\n".join([
        "cat <<'EOF' >$@",
        "#ifndef _GETOPT_H",
        "",
        "#define _GETOPT_H 1",
        "",
        "#if defined __GETOPT_PREFIX",
        "# include <stdlib.h>",
        "# include <stdio.h>",
        "",
        "# ifndef _MSC_VER",
        "#  include <unistd.h>",
        "# endif",
        "#endif",
        "",
        "#ifndef _GL_ARG_NONNULL",
        "# if __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 3) || defined __clang__",
        "#  define _GL_ARG_NONNULL(params) __attribute__ ((__nonnull__ params))",
        "# else",
        "#  define _GL_ARG_NONNULL(params)",
        "# endif",
        "#endif",
        "",
        "#include <getopt-cdefs.h>",
        "#include <getopt-pfx-core.h>",
        "#include <getopt-pfx-ext.h>",
        "",
        "#endif",
        "EOF",
    ]),
)
