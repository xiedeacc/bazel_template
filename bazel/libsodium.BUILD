load("@bazel_template//bazel:common.bzl", "extract_symbols", "template_rule")

package(default_visibility = ["//visibility:public"])

template_rule(
    name = "version_h",
    src = "src/libsodium/include/sodium/version.h.in",
    out = "src/libsodium/include/sodium/version.h",
    substitutions = {
        "@VERSION@": "1.0.20",
        "@SODIUM_LIBRARY_VERSION_MAJOR@": "26",
        "@SODIUM_LIBRARY_VERSION_MINOR@": "2",
        "@SODIUM_LIBRARY_MINIMAL_DEF@": "",
    },
)

COMMON_COTPS = [
    "-g",
    "-pthread",
    "-fno-strict-aliasing",
    "-fno-strict-overflow",
    "-fstack-protector",
    "-ftls-model=local-dynamic",
    "-Wno-deprecated-declarations",
    "-Wno-unknown-pragmas",
    "-Iexternal/libsodium/src/libsodium/include/sodium",
    "-Iexternal/libsodium/src/libsodium/crypto_aead/aegis128l",
    "-Iexternal/libsodium/src/libsodium/crypto_aead/aegis256",
    "-Iexternal/libsodium/src/libsodium/crypto_core/ed25519/ref10/fe_25_5",
    "-Iexternal/libsodium/src/libsodium/crypto_core/ed25519/ref10/fe_51",
    "-Iexternal/libsodium/src/libsodium/crypto_generichash/blake2b/ref",
    "-Iexternal/libsodium/src/libsodium/crypto_onetimeauth/poly1305",
    "-Iexternal/libsodium/src/libsodium/crypto_onetimeauth/poly1305/donna",
    "-Iexternal/libsodium/src/libsodium/crypto_onetimeauth/poly1305/sse2",
    "-Iexternal/libsodium/src/libsodium/crypto_pwhash/argon2",
    "-Iexternal/libsodium/src/libsodium/crypto_pwhash/scryptsalsa208sha256",
    "-Iexternal/libsodium/src/libsodium/crypto_pwhash/scryptsalsa208sha256/nosse",
    "-Iexternal/libsodium/src/libsodium/crypto_scalarmult/curve25519",
    "-Iexternal/libsodium/src/libsodium/crypto_scalarmult/curve25519/ref10",
    "-Iexternal/libsodium/src/libsodium/crypto_scalarmult/curve25519/sandy2x",
    "-Iexternal/libsodium/src/libsodium/crypto_shorthash/siphash24/ref",
    "-Iexternal/libsodium/src/libsodium/crypto_sign/ed25519/ref10",
    "-Iexternal/libsodium/src/libsodium/crypto_stream/chacha20",
    "-Iexternal/libsodium/src/libsodium/crypto_stream/chacha20/dolbeau",
    "-Iexternal/libsodium/src/libsodium/crypto_stream/chacha20/ref",
    "-Iexternal/libsodium/src/libsodium/crypto_stream/salsa20",
    "-Iexternal/libsodium/src/libsodium/crypto_stream/salsa20/ref",
    "-Iexternal/libsodium/src/libsodium/crypto_stream/salsa20/xmm6",
    "-Iexternal/libsodium/src/libsodium/crypto_stream/salsa20/xmm6int",
]

COMMON_DEFINES = [
    "PACKAGE_NAME=\\\"libsodium\\\"",
    "PACKAGE_TARNAME=\\\"libsodium\\\"",
    "PACKAGE_VERSION=\\\"1.0.20\\\"",
    "PACKAGE_STRING=\\\"libsodium\\ 1.0.20\\\"",
    "PACKAGE_BUGREPORT=\\\"https://github.com/jedisct1/libsodium/issues\\\"",
    "PACKAGE_URL=\\\"https://libsodium.org\\\"",
    "PACKAGE=\\\"libsodium\\\"",
    "VERSION=\\\"1.0.20\\\"",
    "CONFIGURED=1",
    "HAVE_ALLOCA=1",
    "HAVE_ALLOCA_H=1",
    "HAVE_ATOMIC_OPS=1",
    "HAVE_C11_MEMORY_FENCES=1",
    "HAVE_CLOCK_GETTIME=1",
    "HAVE_C_VARARRAYS=1",
    "HAVE_DLFCN_H=1",
    "HAVE_EXPLICIT_BZERO=1",
    "HAVE_GCC_MEMORY_FENCES=1",
    "HAVE_GETAUXVAL=1",
    "HAVE_GETENTROPY=1",
    "HAVE_GETPID=1",
    "HAVE_GETRANDOM=1",
    "HAVE_INLINE_ASM=1",
    "HAVE_INTTYPES_H=1",
    "HAVE_MADVISE=1",
    "HAVE_MLOCK=1",
    "HAVE_MMAP=1",
    "HAVE_MPROTECT=1",
    "HAVE_NANOSLEEP=1",
    "HAVE_POSIX_MEMALIGN=1",
    "HAVE_PTHREAD=1",
    "HAVE_PTHREAD_PRIO_INHERIT=1",
    "HAVE_RAISE=1",
    "HAVE_STDINT_H=1",
    "HAVE_STDIO_H=1",
    "HAVE_STDLIB_H=1",
    "HAVE_STRINGS_H=1",
    "HAVE_STRING_H=1",
    "HAVE_SYSCONF=1",
    "HAVE_SYS_MMAN_H=1",
    "HAVE_SYS_PARAM_H=1",
    "HAVE_SYS_RANDOM_H=1",
    "HAVE_SYS_STAT_H=1",
    "HAVE_SYS_TYPES_H=1",
    "HAVE_TI_MODE=1",
    "HAVE_UNISTD_H=1",
    "HAVE_WCHAR_H=1",
    "HAVE_WEAK_SYMBOLS=1",
    "NATIVE_LITTLE_ENDIAN=1",
    "STDC_HEADERS=1",
    "TLS=_Thread_local",
    "_ALL_SOURCE=1",
    "_DARWIN_C_SOURCE=1",
    "_GNU_SOURCE=1",
    "_HPUX_ALT_XOPEN_SOCKET_API=1",
    "_NETBSD_SOURCE=1",
    "_OPENBSD_SOURCE=1",
    "_POSIX_PTHREAD_SEMANTICS=1",
    "_TANDEM_SOURCE=1",
    "__EXTENSIONS__=1",
    "__STDC_WANT_IEC_60559_ATTRIBS_EXT__=1",
    "__STDC_WANT_IEC_60559_BFP_EXT__=1",
    "__STDC_WANT_IEC_60559_DFP_EXT__=1",
    "__STDC_WANT_IEC_60559_EXT__=1",
    "__STDC_WANT_IEC_60559_FUNCS_EXT__=1",
    "__STDC_WANT_IEC_60559_TYPES_EXT__=1",
    "__STDC_WANT_LIB_EXT2__=1",
    "__STDC_WANT_MATH_SPEC_FUNCS__=1",
]

X86_64_DEFINES = [
    "HAVE_AMD64_ASM=1",
    "HAVE_AMD64_ASM_V=1",
    "HAVE_AVX2INTRIN_H=1",
    "HAVE_CATCHABLE_ABRT=1",
    "HAVE_CPUID=1",
    "HAVE_EMMINTRIN_H=1",
    "HAVE_MMINTRIN_H=1",
    "HAVE_PMMINTRIN_H=1",
    "HAVE_RDRAND=1",
    "HAVE_TI_MODE_V=1",
    "HAVE_WMMINTRIN_H=1",
    "HAVE_TMMINTRIN_H=1",
    "HAVE_SMMINTRIN_H=1",
    "HAVE_CPUID_V=1",
    "HAVE_CATCHABLE_SEGV=1",
    "HAVE_CET_H=1",
    "HAVE_AVX512FINTRIN_H=1",
    "HAVE_AVXINTRIN_H=1",
    "HAVE_AVX_ASM=1",
    "HAVE_AVX_ASM_V=1",
]

AARCH64_DEFINES = [
    "HAVE_ARMCRYPTO=1",
]

LINUX_DEFINES = [
    "HAVE_SYS_AUXV_H=1",
]

OSX_DEFINES = [
    "HAVE_COMMONCRYPTO_COMMONRANDOM_H=1",
    "HAVE_ARC4RANDOM=1",
    "HAVE_ARC4RANDOM_BUF=1",
    "HAVE_MEMSET_S=1",
]

cc_library(
    name = "utils",
    srcs = [
        "src/libsodium/sodium/utils.c",
        "src/libsodium/sodium/version.c",
    ],
    hdrs = [":version_h"] + glob([
        "src/libsodium/include/sodium/*.h",
        "src/libsodium/include/sodium/private/*.h",
    ]),
    copts = select({
        "@platforms//cpu:aarch64": [],
        "//conditions:default": [],
    }) + COMMON_COTPS + ["-I$(GENDIR)/external/libsodium/src/libsodium/include/sodium"],
    local_defines = COMMON_DEFINES + select({
        "@platforms//cpu:aarch64": AARCH64_DEFINES,
        "//conditions:default": X86_64_DEFINES,
    }) + select({
        "@platforms//os:osx": OSX_DEFINES,
        "@platforms//os:linux": LINUX_DEFINES,
        "//conditions:default": [],
    }),
)

cc_library(
    name = "common",
    srcs = [
        "src/libsodium/crypto_aead/aegis128l/aead_aegis128l.c",
        "src/libsodium/crypto_aead/aegis128l/aegis128l_armcrypto.c",
        "src/libsodium/crypto_aead/aegis128l/aegis128l_soft.c",
        "src/libsodium/crypto_aead/aegis256/aead_aegis256.c",
        "src/libsodium/crypto_aead/aegis256/aegis256_armcrypto.c",
        "src/libsodium/crypto_aead/aegis256/aegis256_soft.c",
        "src/libsodium/crypto_aead/aes256gcm/aead_aes256gcm.c",
        "src/libsodium/crypto_aead/aes256gcm/armcrypto/aead_aes256gcm_armcrypto.c",
        "src/libsodium/crypto_aead/chacha20poly1305/aead_chacha20poly1305.c",
        "src/libsodium/crypto_aead/xchacha20poly1305/aead_xchacha20poly1305.c",
        "src/libsodium/crypto_auth/crypto_auth.c",
        "src/libsodium/crypto_auth/hmacsha256/auth_hmacsha256.c",
        "src/libsodium/crypto_auth/hmacsha512/auth_hmacsha512.c",
        "src/libsodium/crypto_auth/hmacsha512256/auth_hmacsha512256.c",
        "src/libsodium/crypto_box/crypto_box.c",
        "src/libsodium/crypto_box/crypto_box_easy.c",
        "src/libsodium/crypto_box/crypto_box_seal.c",
        "src/libsodium/crypto_box/curve25519xchacha20poly1305/box_curve25519xchacha20poly1305.c",
        "src/libsodium/crypto_box/curve25519xchacha20poly1305/box_seal_curve25519xchacha20poly1305.c",
        "src/libsodium/crypto_box/curve25519xsalsa20poly1305/box_curve25519xsalsa20poly1305.c",
        "src/libsodium/crypto_core/ed25519/core_ed25519.c",
        "src/libsodium/crypto_core/ed25519/core_ristretto255.c",
        "src/libsodium/crypto_core/ed25519/ref10/ed25519_ref10.c",
        "src/libsodium/crypto_core/hchacha20/core_hchacha20.c",
        "src/libsodium/crypto_core/hsalsa20/core_hsalsa20.c",
        "src/libsodium/crypto_core/hsalsa20/ref2/core_hsalsa20_ref2.c",
        "src/libsodium/crypto_core/salsa/ref/core_salsa_ref.c",
        "src/libsodium/crypto_core/softaes/softaes.c",
        "src/libsodium/crypto_generichash/blake2b/generichash_blake2.c",
        "src/libsodium/crypto_generichash/blake2b/ref/blake2b-compress-ref.c",
        "src/libsodium/crypto_generichash/blake2b/ref/blake2b-ref.c",
        "src/libsodium/crypto_generichash/blake2b/ref/generichash_blake2b.c",
        "src/libsodium/crypto_generichash/crypto_generichash.c",
        "src/libsodium/crypto_hash/crypto_hash.c",
        "src/libsodium/crypto_hash/sha256/cp/hash_sha256_cp.c",
        "src/libsodium/crypto_hash/sha256/hash_sha256.c",
        "src/libsodium/crypto_hash/sha512/cp/hash_sha512_cp.c",
        "src/libsodium/crypto_hash/sha512/hash_sha512.c",
        "src/libsodium/crypto_kdf/blake2b/kdf_blake2b.c",
        "src/libsodium/crypto_kdf/crypto_kdf.c",
        "src/libsodium/crypto_kdf/hkdf/kdf_hkdf_sha256.c",
        "src/libsodium/crypto_kdf/hkdf/kdf_hkdf_sha512.c",
        "src/libsodium/crypto_kx/crypto_kx.c",
        "src/libsodium/crypto_onetimeauth/crypto_onetimeauth.c",
        "src/libsodium/crypto_onetimeauth/poly1305/donna/poly1305_donna.c",
        "src/libsodium/crypto_onetimeauth/poly1305/onetimeauth_poly1305.c",
        "src/libsodium/crypto_pwhash/argon2/argon2.c",
        "src/libsodium/crypto_pwhash/argon2/argon2-core.c",
        "src/libsodium/crypto_pwhash/argon2/argon2-encoding.c",
        "src/libsodium/crypto_pwhash/argon2/argon2-fill-block-ref.c",
        "src/libsodium/crypto_pwhash/argon2/blake2b-long.c",
        "src/libsodium/crypto_pwhash/argon2/pwhash_argon2i.c",
        "src/libsodium/crypto_pwhash/argon2/pwhash_argon2id.c",
        "src/libsodium/crypto_pwhash/crypto_pwhash.c",
        "src/libsodium/crypto_pwhash/scryptsalsa208sha256/crypto_scrypt-common.c",
        "src/libsodium/crypto_pwhash/scryptsalsa208sha256/nosse/pwhash_scryptsalsa208sha256_nosse.c",
        "src/libsodium/crypto_pwhash/scryptsalsa208sha256/pbkdf2-sha256.c",
        "src/libsodium/crypto_pwhash/scryptsalsa208sha256/pwhash_scryptsalsa208sha256.c",
        "src/libsodium/crypto_pwhash/scryptsalsa208sha256/scrypt_platform.c",
        "src/libsodium/crypto_scalarmult/crypto_scalarmult.c",
        "src/libsodium/crypto_scalarmult/curve25519/ref10/x25519_ref10.c",
        "src/libsodium/crypto_scalarmult/curve25519/sandy2x/curve25519_sandy2x.c",
        "src/libsodium/crypto_scalarmult/curve25519/sandy2x/fe51_invert.c",
        "src/libsodium/crypto_scalarmult/curve25519/sandy2x/fe_frombytes_sandy2x.c",
        "src/libsodium/crypto_scalarmult/curve25519/scalarmult_curve25519.c",
        "src/libsodium/crypto_scalarmult/ed25519/ref10/scalarmult_ed25519_ref10.c",
        "src/libsodium/crypto_scalarmult/ristretto255/ref10/scalarmult_ristretto255_ref10.c",
        "src/libsodium/crypto_secretbox/crypto_secretbox.c",
        "src/libsodium/crypto_secretbox/crypto_secretbox_easy.c",
        "src/libsodium/crypto_secretbox/xchacha20poly1305/secretbox_xchacha20poly1305.c",
        "src/libsodium/crypto_secretbox/xsalsa20poly1305/secretbox_xsalsa20poly1305.c",
        "src/libsodium/crypto_secretstream/xchacha20poly1305/secretstream_xchacha20poly1305.c",
        "src/libsodium/crypto_shorthash/crypto_shorthash.c",
        "src/libsodium/crypto_shorthash/siphash24/ref/shorthash_siphash24_ref.c",
        "src/libsodium/crypto_shorthash/siphash24/ref/shorthash_siphashx24_ref.c",
        "src/libsodium/crypto_shorthash/siphash24/shorthash_siphash24.c",
        "src/libsodium/crypto_shorthash/siphash24/shorthash_siphashx24.c",
        "src/libsodium/crypto_sign/crypto_sign.c",
        "src/libsodium/crypto_sign/ed25519/ref10/keypair.c",
        "src/libsodium/crypto_sign/ed25519/ref10/obsolete.c",
        "src/libsodium/crypto_sign/ed25519/ref10/open.c",
        "src/libsodium/crypto_sign/ed25519/ref10/sign.c",
        "src/libsodium/crypto_sign/ed25519/sign_ed25519.c",
        "src/libsodium/crypto_stream/chacha20/ref/chacha20_ref.c",
        "src/libsodium/crypto_stream/chacha20/stream_chacha20.c",
        "src/libsodium/crypto_stream/crypto_stream.c",
        "src/libsodium/crypto_stream/salsa20/stream_salsa20.c",
        "src/libsodium/crypto_stream/salsa20/xmm6/salsa20_xmm6.c",
        "src/libsodium/crypto_stream/salsa2012/ref/stream_salsa2012_ref.c",
        "src/libsodium/crypto_stream/salsa2012/stream_salsa2012.c",
        "src/libsodium/crypto_stream/salsa208/ref/stream_salsa208_ref.c",
        "src/libsodium/crypto_stream/salsa208/stream_salsa208.c",
        "src/libsodium/crypto_stream/xchacha20/stream_xchacha20.c",
        "src/libsodium/crypto_stream/xsalsa20/stream_xsalsa20.c",
        "src/libsodium/crypto_verify/verify.c",
        "src/libsodium/randombytes/randombytes.c",
        "src/libsodium/randombytes/sysrandom/randombytes_sysrandom.c",
        #"src/libsodium/sodium/codecs.c",
        #"src/libsodium/sodium/core.c",
        #"src/libsodium/sodium/runtime.c",
        #"src/libsodium/sodium/utils.c",
        #"src/libsodium/sodium/version.c",
    ] + [
        "src/libsodium/crypto_scalarmult/curve25519/sandy2x/sandy2x.S",
        "src/libsodium/crypto_stream/salsa20/xmm6/salsa20_xmm6-asm.S",
    ],
    hdrs = glob([
        "src/libsodium/**/*.h",
        "src/libsodium/crypto_scalarmult/curve25519/sandy2x/*.S",
    ]),
    copts = select({
        "@platforms//cpu:aarch64": [],
        "//conditions:default": [],
    }) + COMMON_COTPS + ["-Iexternal/libsodium/src/libsodium/include/sodium"],
    local_defines = COMMON_DEFINES + select({
        "@platforms//cpu:aarch64": AARCH64_DEFINES,
        "//conditions:default": X86_64_DEFINES,
    }) + select({
        "@platforms//os:osx": OSX_DEFINES + ["ASM_HIDE_SYMBOL=.private_extern"],
        "@platforms//os:linux": LINUX_DEFINES + ["ASM_HIDE_SYMBOL=.hidden"],
        "//conditions:default": ["ASM_HIDE_SYMBOL=.hidden"],
    }),
    deps = [":utils"],
)

cc_library(
    name = "rdrand",
    srcs = [
        "src/libsodium/randombytes/internal/randombytes_internal_random.c",
    ],
    hdrs = glob([
        "src/libsodium/**/*.h",
    ]),
    copts = select({
        "@platforms//cpu:aarch64": [],
        "//conditions:default": [
            "-mrdrnd",
        ],
    }) + COMMON_COTPS,
    linkopts = select({
        "@platforms//os:windows": [],
        "@platforms//os:osx": [],
        "//conditions:default": [
            "-Wl,-z,relro",
            "-Wl,-z,now",
            "-Wl,-z,noexecstack",
        ],
    }),
    local_defines = COMMON_DEFINES + select({
        "@platforms//cpu:aarch64": AARCH64_DEFINES,
        "//conditions:default": X86_64_DEFINES,
    }) + select({
        "@platforms//os:osx": OSX_DEFINES,
        "@platforms//os:linux": LINUX_DEFINES,
        "//conditions:default": [],
    }),
    deps = [":common"],
)

cc_library(
    name = "sse2",
    srcs = [
        "src/libsodium/crypto_onetimeauth/poly1305/sse2/poly1305_sse2.c",
        "src/libsodium/crypto_pwhash/scryptsalsa208sha256/sse/pwhash_scryptsalsa208sha256_sse.c",
    ],
    hdrs = glob([
        "src/libsodium/**/*.h",
    ]),
    copts = select({
        "@platforms//cpu:aarch64": [],
        "//conditions:default": [
            "-msse2",
        ],
    }) + COMMON_COTPS,
    linkopts = select({
        "@platforms//os:windows": [],
        "@platforms//os:osx": [],
        "//conditions:default": [
            "-Wl,-z,relro",
            "-Wl,-z,now",
            "-Wl,-z,noexecstack",
        ],
    }),
    local_defines = COMMON_DEFINES + select({
        "@platforms//cpu:aarch64": AARCH64_DEFINES,
        "//conditions:default": X86_64_DEFINES,
    }) + select({
        "@platforms//os:osx": OSX_DEFINES,
        "@platforms//os:linux": LINUX_DEFINES,
        "//conditions:default": [],
    }),
)

cc_library(
    name = "ssse3",
    srcs = [
        "src/libsodium/crypto_generichash/blake2b/ref/blake2b-compress-ssse3.c",
        "src/libsodium/crypto_pwhash/argon2/argon2-fill-block-ssse3.c",
        "src/libsodium/crypto_stream/chacha20/dolbeau/chacha20_dolbeau-ssse3.c",
    ],
    hdrs = glob([
        "src/libsodium/**/*.h",
    ]),
    copts = select({
        "@platforms//cpu:aarch64": [],
        "//conditions:default": [
            "-msse2",
            "-mssse3",
        ],
    }) + COMMON_COTPS,
    linkopts = select({
        "@platforms//os:windows": [],
        "@platforms//os:osx": [],
        "//conditions:default": [
            "-Wl,-z,relro",
            "-Wl,-z,now",
            "-Wl,-z,noexecstack",
        ],
    }),
    local_defines = COMMON_DEFINES + select({
        "@platforms//cpu:aarch64": AARCH64_DEFINES,
        "//conditions:default": X86_64_DEFINES,
    }) + select({
        "@platforms//os:osx": OSX_DEFINES,
        "@platforms//os:linux": LINUX_DEFINES,
        "//conditions:default": [],
    }),
    deps = [":common"],
)

cc_library(
    name = "armcrypto",
    srcs = [
        "src/libsodium/crypto_aead/aegis128l/aegis128l_armcrypto.c",
        "src/libsodium/crypto_aead/aegis256/aegis256_armcrypto.c",
        "src/libsodium/crypto_aead/aes256gcm/armcrypto/aead_aes256gcm_armcrypto.c",
    ],
    hdrs = glob([
        "src/libsodium/**/*.h",
    ]),
    copts = select({
        "@platforms//cpu:aarch64": [],
        "//conditions:default": [
            "-msse2",
            "-mssse3",
        ],
    }) + COMMON_COTPS,
    linkopts = select({
        "@platforms//os:windows": [],
        "@platforms//os:osx": [],
        "//conditions:default": [
            "-Wl,-z,relro",
            "-Wl,-z,now",
            "-Wl,-z,noexecstack",
        ],
    }),
    local_defines = COMMON_DEFINES + select({
        "@platforms//cpu:aarch64": AARCH64_DEFINES,
        "//conditions:default": X86_64_DEFINES,
    }) + select({
        "@platforms//os:osx": OSX_DEFINES,
        "@platforms//os:linux": LINUX_DEFINES,
        "//conditions:default": [],
    }),
    deps = [":common"],
)

cc_library(
    name = "aesni",
    srcs = [
        "src/libsodium/crypto_aead/aegis128l/aegis128l_aesni.c",
        "src/libsodium/crypto_aead/aegis256/aegis256_aesni.c",
        "src/libsodium/crypto_aead/aes256gcm/aesni/aead_aes256gcm_aesni.c",
    ],
    hdrs = glob([
        "src/libsodium/**/*.h",
    ]),
    copts = select({
        "@platforms//cpu:aarch64": [],
        "//conditions:default": [
            "-msse2",
            "-mssse3",
            "-mavx",
            "-maes",
            "-mpclmul",
        ],
    }) + COMMON_COTPS,
    linkopts = select({
        "@platforms//os:windows": [],
        "@platforms//os:osx": [],
        "//conditions:default": [
            "-Wl,-z,relro",
            "-Wl,-z,now",
            "-Wl,-z,noexecstack",
        ],
    }),
    local_defines = COMMON_DEFINES + select({
        "@platforms//cpu:aarch64": AARCH64_DEFINES,
        "//conditions:default": X86_64_DEFINES,
    }) + select({
        "@platforms//os:osx": OSX_DEFINES,
        "@platforms//os:linux": LINUX_DEFINES,
        "//conditions:default": [],
    }),
    deps = [":common"],
)

cc_library(
    name = "sse41",
    srcs = [
        "src/libsodium/crypto_generichash/blake2b/ref/blake2b-compress-sse41.c",
    ],
    hdrs = glob([
        "src/libsodium/**/*.h",
    ]),
    copts = select({
        "@platforms//cpu:aarch64": [],
        "//conditions:default": [
            "-msse2",
            "-mssse3",
            "-msse4.1",
        ],
    }) + COMMON_COTPS,
    linkopts = select({
        "@platforms//os:windows": [],
        "@platforms//os:osx": [],
        "//conditions:default": [
            "-Wl,-z,relro",
            "-Wl,-z,now",
            "-Wl,-z,noexecstack",
        ],
    }),
    local_defines = COMMON_DEFINES + select({
        "@platforms//cpu:aarch64": AARCH64_DEFINES,
        "//conditions:default": X86_64_DEFINES,
    }) + select({
        "@platforms//os:osx": OSX_DEFINES,
        "@platforms//os:linux": LINUX_DEFINES,
        "//conditions:default": [],
    }),
    deps = [":common"],
)

cc_library(
    name = "avx2",
    srcs = [
        "src/libsodium/crypto_generichash/blake2b/ref/blake2b-compress-avx2.c",
        "src/libsodium/crypto_pwhash/argon2/argon2-fill-block-avx2.c",
        "src/libsodium/crypto_stream/chacha20/dolbeau/chacha20_dolbeau-avx2.c",
        "src/libsodium/crypto_stream/salsa20/xmm6int/salsa20_xmm6int-avx2.c",
    ],
    hdrs = glob([
        "src/libsodium/**/*.h",
    ]),
    copts = select({
        "@platforms//cpu:aarch64": [],
        "//conditions:default": [
            "-msse2",
            "-mssse3",
            "-msse4.1",
            "-mavx",
            "-mavx2",
        ],
    }) + COMMON_COTPS,
    linkopts = select({
        "@platforms//os:windows": [],
        "@platforms//os:osx": [],
        "//conditions:default": [
            "-Wl,-z,relro",
            "-Wl,-z,now",
            "-Wl,-z,noexecstack",
        ],
    }),
    local_defines = COMMON_DEFINES + select({
        "@platforms//cpu:aarch64": AARCH64_DEFINES,
        "//conditions:default": X86_64_DEFINES,
    }) + select({
        "@platforms//os:osx": OSX_DEFINES,
        "@platforms//os:linux": LINUX_DEFINES,
        "//conditions:default": [],
    }),
    deps = [":common"],
)

cc_library(
    name = "avx512f",
    srcs = [
        "src/libsodium/crypto_pwhash/argon2/argon2-fill-block-avx512f.c",
    ],
    hdrs = glob([
        "src/libsodium/**/*.h",
    ]),
    copts = select({
        "@platforms//cpu:aarch64": [],
        "//conditions:default": [
            "-msse2",
            "-mssse3",
            "-msse4.1",
            "-mavx",
            "-mavx2",
            "-mavx512f",
        ],
    }) + COMMON_COTPS,
    linkopts = select({
        "@platforms//os:windows": [],
        "@platforms//os:osx": [],
        "//conditions:default": [
            "-Wl,-z,relro",
            "-Wl,-z,now",
            "-Wl,-z,noexecstack",
        ],
    }),
    local_defines = COMMON_DEFINES + select({
        "@platforms//cpu:aarch64": AARCH64_DEFINES,
        "//conditions:default": X86_64_DEFINES,
    }) + select({
        "@platforms//os:osx": OSX_DEFINES,
        "@platforms//os:linux": LINUX_DEFINES,
        "//conditions:default": [],
    }),
    deps = [":common"],
)

cc_library(
    name = "sodium",
    srcs = [
        "src/libsodium/sodium/codecs.c",
        "src/libsodium/sodium/core.c",
        "src/libsodium/sodium/runtime.c",
    ],
    hdrs = glob([
        "src/libsodium/include/sodium/*.h",
        "src/libsodium/include/sodium/private/*.h",
    ]),
    copts = select({
        "@platforms//cpu:aarch64": [],
        "//conditions:default": [
            "-msse2",
            "-mssse3",
        ],
    }) + COMMON_COTPS,
    linkopts = select({
        "@platforms//os:windows": [],
        "@platforms//os:osx": [],
        "//conditions:default": [
            "-Wl,-z,relro",
            "-Wl,-z,now",
            "-Wl,-z,noexecstack",
        ],
    }),
    local_defines = COMMON_DEFINES + select({
        "@platforms//cpu:aarch64": AARCH64_DEFINES,
        "//conditions:default": X86_64_DEFINES,
    }) + select({
        "@platforms//os:osx": OSX_DEFINES,
        "@platforms//os:linux": LINUX_DEFINES,
        "//conditions:default": [],
    }),
    deps = [
        ":aesni",
        ":armcrypto",
        ":avx2",
        ":avx512f",
        ":common",
        ":rdrand",
        ":sse2",
        ":sse41",
        ":ssse3",
    ],
)
