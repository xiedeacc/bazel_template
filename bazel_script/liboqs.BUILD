package(default_visibility = ["//visibility:public"])

COPTS = [
    "-I.",
    "-I$(BINDIR)/external/liboqs/src",
    "-march=native",
    "-mbmi2",
    "-mbmi",
    "-maes",
    "-msse4.2",
    "-mavx2",
    "-mavx512f",
    "-mavx512dq",
    "-mavx512cd",
    "-mavx512bw",
    "-mavx512vl",
    "-mpclmul",
    "-mvpclmulqdq",
    "-mpopcnt",
    "-fPIC",
    "-O3",
    "-std=gnu11",
    "-fomit-frame-pointer",
]

filegroup(
    name = "header_files",
    srcs = [
        "src/common/aes/aes.h",
        "src/common/common.h",
        "src/common/rand/rand.h",
        "src/common/sha2/sha2.h",
        "src/common/sha3/sha3.h",
        "src/common/sha3/sha3x4.h",
        "src/kem/bike/kem_bike.h",
        "src/kem/classic_mceliece/kem_classic_mceliece.h",
        "src/kem/frodokem/kem_frodokem.h",
        "src/kem/hqc/kem_hqc.h",
        "src/kem/kem.h",
        "src/kem/kyber/kem_kyber.h",
        "src/kem/ntruprime/kem_ntruprime.h",
        "src/oqs.h",
        "src/oqsconfig.h",
        "src/sig/dilithium/sig_dilithium.h",
        "src/sig/falcon/sig_falcon.h",
        "src/sig/sig.h",
        "src/sig/sphincs/sig_sphincs.h",
    ],
)

genrule(
    name = "cp_headers",
    srcs = [
        ":header_files",
    ],
    outs = [
        "src/oqs/aes.h",
        "src/oqs/common.h",
        "src/oqs/rand.h",
        "src/oqs/sha2.h",
        "src/oqs/sha3.h",
        "src/oqs/sha3x4.h",
        "src/oqs/kem_bike.h",
        "src/oqs/kem_classic_mceliece.h",
        "src/oqs/kem_frodokem.h",
        "src/oqs/kem_hqc.h",
        "src/oqs/kem.h",
        "src/oqs/kem_kyber.h",
        "src/oqs/kem_ntruprime.h",
        "src/oqs/oqs.h",
        "src/oqs/oqsconfig.h",
        "src/oqs/sig_dilithium.h",
        "src/oqs/sig_falcon.h",
        "src/oqs/sig.h",
        "src/oqs/sig_sphincs.h",
    ],
    cmd = "cp $(SRCS) $(BINDIR)/external/liboqs/src/oqs",
)

cc_library(
    name = "headers",
    hdrs = [
        "src/common/aes/aes.h",
        "src/common/aes/aes_local.h",
        "src/common/ossl_helpers.h",
        "src/oqs/aes.h",
        "src/oqs/common.h",
        "src/oqs/kem.h",
        "src/oqs/kem_bike.h",
        "src/oqs/kem_classic_mceliece.h",
        "src/oqs/kem_frodokem.h",
        "src/oqs/kem_hqc.h",
        "src/oqs/kem_kyber.h",
        "src/oqs/kem_ntruprime.h",
        "src/oqs/oqs.h",
        "src/oqs/oqsconfig.h",
        "src/oqs/rand.h",
        "src/oqs/sha2.h",
        "src/oqs/sha3.h",
        "src/oqs/sha3x4.h",
        "src/oqs/sig.h",
        "src/oqs/sig_dilithium.h",
        "src/oqs/sig_falcon.h",
        "src/oqs/sig_sphincs.h",
    ],
)

cc_library(
    name = "aes",
    srcs = [
        "src/common/aes/aes.c",
        "src/common/aes/aes128_ni.c",
        "src/common/aes/aes256_ni.c",
        "src/common/aes/aes_c.c",
    ],
    hdrs = [
        "src/common/x86_64_helpers.h",
    ],
    copts = COPTS + [
        "-I.",
    ],
    includes = [
        "$(BINDIR)/external/liboqs/src",
    ],
    local_defines = [
        "ADD_SYMBOL_SUFFIX",
        "OQS_HAVE_ALIGNED_ALLOC",
        "OQS_HAVE_EXPLICIT_BZERO",
        "OQS_HAVE_MEMALIGN",
        "DISABLE_VPCLMUL",
        "X86_64",
        "OQS_HAVE_POSIX_MEMALIGN",
    ],
    deps = [
        ":headers",
    ],
)

cc_library(
    name = "sha2",
    srcs = [
        "src/common/ossl_helpers.c",
        "src/common/sha2/sha2_ossl.c",
    ],
    hdrs = [
        "src/common/ossl_helpers.h",
        "src/common/sha2/sha2.h",
        "src/common/x86_64_helpers.h",
    ],
    copts = COPTS + [
        "-I.",
    ],
    includes = [
        "$(BINDIR)/external/liboqs/src",
    ],
    local_defines = [
        "ADD_SYMBOL_SUFFIX",
        "OQS_HAVE_ALIGNED_ALLOC",
        "OQS_HAVE_EXPLICIT_BZERO",
        "OQS_HAVE_MEMALIGN",
        "DISABLE_VPCLMUL",
        "X86_64",
        "OQS_HAVE_POSIX_MEMALIGN",
    ],
    deps = [
        ":headers",
        "@openssl//:ssl",
    ],
)

cc_library(
    name = "sha3plain",
    srcs = [
    ] + glob(
        [
            "src/common/sha3/xkcp_low/KeccakP-1600/plain-64bits/*.c",
            "src/common/sha3/xkcp_low/KeccakP-1600/plain-64bits/*.h",
            "src/common/sha3/xkcp_low/KeccakP-1600times4/serial/*.inc",
            "src/common/sha3/xkcp_low/KeccakP-1600times4/serial/*.h",
            "src/common/sha3/xkcp_low/KeccakP-1600times4/serial/*.c",
        ],
        exclude = [
            "src/common/sha3/xkcp_low/KeccakP-1600times4/serial/KeccakP-1600-times4-SnP.h",
            "src/common/sha3/xkcp_low/KeccakP-1600/plain-64bits/KeccakP-1600-SnP.h",
        ],
    ),
    hdrs = [
    ] + glob(
        [
            "src/common/sha3/xkcp_low/KeccakP-1600times4/serial/KeccakP-1600-times4-SnP.h",
            "src/common/sha3/xkcp_low/KeccakP-1600/plain-64bits/KeccakP-1600-SnP.h",
            "src/common/sha3/xkcp_low/KeccakP-1600/plain-64bits/*.macros",
        ],
        exclude = [
        ],
    ),
    copts = COPTS + [
        "-I.",
    ],
    includes = [
        "$(BINDIR)/external/liboqs/src",
        "src/common/sha3/xkcp_low/KeccakP-1600/plain-64bits",
        "src/common/sha3/xkcp_low/KeccakP-1600times4/serial",
    ],
    local_defines = [
        "ADD_SYMBOL_SUFFIX",
        "OQS_HAVE_ALIGNED_ALLOC",
        "OQS_HAVE_EXPLICIT_BZERO",
        "OQS_HAVE_MEMALIGN",
        "DISABLE_VPCLMUL",
        "X86_64",
        "OQS_HAVE_POSIX_MEMALIGN",
    ],
    deps = [
        ":headers",
    ],
)

cc_library(
    name = "sha3avx2",
    srcs = [
    ] + glob(
        [
            "src/common/sha3/xkcp_low/KeccakP-1600/avx2/*.S",
            "src/common/sha3/xkcp_low/KeccakP-1600/avx2/*.h",
            "src/common/sha3/xkcp_low/KeccakP-1600times4/avx2/*.inc",
            "src/common/sha3/xkcp_low/KeccakP-1600times4/avx2/*.c",
            "src/common/sha3/xkcp_low/KeccakP-1600times4/avx2/*.h",
        ],
        exclude = [
            "src/common/sha3/xkcp_low/KeccakP-1600times4/avx2/KeccakP-1600-times4-SnP.h",
            "src/common/sha3/xkcp_low/KeccakP-1600/avx2/KeccakP-1600-SnP.h",
        ],
    ),
    hdrs = [
    ] + glob(
        [
            "src/common/sha3/xkcp_low/KeccakP-1600times4/avx2/*.macros",
            "src/common/sha3/xkcp_low/KeccakP-1600times4/avx2/KeccakP-1600-times4-SnP.h",
            "src/common/sha3/xkcp_low/KeccakP-1600/avx2/KeccakP-1600-SnP.h",
        ],
        exclude = [
        ],
    ),
    copts = COPTS + [
        "-I.",
    ],
    includes = [
        "$(BINDIR)/external/liboqs/src",
        "src/common/sha3/xkcp_low/KeccakP-1600/avx2",
        "src/common/sha3/xkcp_low/KeccakP-1600times4/avx2",
    ],
    local_defines = [
        "ADD_SYMBOL_SUFFIX",
        "OQS_HAVE_ALIGNED_ALLOC",
        "OQS_HAVE_EXPLICIT_BZERO",
        "OQS_HAVE_MEMALIGN",
        "DISABLE_VPCLMUL",
        "X86_64",
        "OQS_HAVE_POSIX_MEMALIGN",
    ],
    deps = [
        ":headers",
    ],
)

cc_library(
    name = "sha3",
    srcs = [
        "src/common/sha3/xkcp_sha3.c",
        "src/common/sha3/xkcp_sha3x4.c",
    ],
    hdrs = [
        "src/common/sha3/sha3.h",
        "src/common/sha3/sha3x4.h",
        "src/common/sha3/xkcp_dispatch.h",
    ],
    copts = COPTS + [
        "-I.",
    ],
    includes = [
        "$(BINDIR)/external/liboqs/src",
    ],
    local_defines = [
        "ADD_SYMBOL_SUFFIX",
        "OQS_HAVE_ALIGNED_ALLOC",
        "OQS_HAVE_EXPLICIT_BZERO",
        "OQS_HAVE_MEMALIGN",
        "DISABLE_VPCLMUL",
        "X86_64",
        "OQS_HAVE_POSIX_MEMALIGN",
    ],
    deps = [
        ":sha3avx2",
        ":sha3plain",
    ],
)

cc_library(
    name = "common",
    srcs = [
        "src/common/common.c",
        "src/common/pqclean_shims/fips202.c",
        "src/common/pqclean_shims/fips202x4.c",
        "src/common/pqclean_shims/nistseedexpander.c",
        "src/common/rand/rand.c",
        "src/common/rand/rand_nist.c",
    ],
    hdrs = [
    ] + glob([
        "src/common/pqclean_shims/*.h",
        "src/common/rand/*.h",
    ]),
    copts = COPTS + [
        "-I.",
    ],
    includes = [
        "$(BINDIR)/external/liboqs/src",
    ],
    local_defines = [
        "ADD_SYMBOL_SUFFIX",
        "OQS_HAVE_ALIGNED_ALLOC",
        "OQS_HAVE_EXPLICIT_BZERO",
        "OQS_HAVE_MEMALIGN",
        "DISABLE_VPCLMUL",
        "X86_64",
        "OQS_HAVE_POSIX_MEMALIGN",
    ],
    deps = [
        ":aes",
        ":sha2",
        ":sha3",
    ],
)

cc_library(
    name = "classic_mceliece",
    srcs = glob(
        [
            "src/kem/classic_mceliece/**/*.c",
            "src/kem/classic_mceliece/**/*.S",
            "src/kem/classic_mceliece/**/*.h",
        ],
        exclude = [
        ],
    ),
    hdrs = [
    ] + glob(
        [
            "src/kem/classic_mceliece/**/*.data",
        ],
    ),
    copts = COPTS + [
        "-I.",
    ],
    includes = [
        "$(BINDIR)/external/liboqs/src",
        "src/common/pqclean_shims",
    ],
    local_defines = [
    ],
    deps = [
        ":common",
    ],
)

cc_library(
    name = "bikel1",
    srcs = glob(
        [
            "src/kem/bike/**/*.c",
            "src/kem/bike/**/*.h",
        ],
        exclude = [
        ],
    ),
    hdrs = [
    ] + glob(
        [
        ],
    ),
    copts = [
        "-include src/kem/bike/functions_renaming.h",
    ] + COPTS,
    includes = [
        "$(BINDIR)/external/liboqs/src",
        "src/common/pqclean_shims",
    ],
    local_defines = [
        "DISABLE_VPCLMUL",
        "FUNC_PREFIX=OQS_KEM_bike_l1",
        "LEVEL=1",
        "X86_64",
    ],
    deps = [
        ":common",
    ],
)

cc_library(
    name = "bikel3",
    srcs = glob(
        [
            "src/kem/bike/**/*.c",
            "src/kem/bike/**/*.h",
        ],
        exclude = [
        ],
    ),
    hdrs = [
    ] + glob(
        [
        ],
    ),
    copts = [
        "-include src/kem/bike/functions_renaming.h",
    ] + COPTS,
    includes = [
        "$(BINDIR)/external/liboqs/src",
        "src/common/pqclean_shims",
    ],
    local_defines = [
        "DISABLE_VPCLMUL",
        "FUNC_PREFIX=OQS_KEM_bike_l3",
        "LEVEL=3",
        "X86_64",
    ],
    deps = [
        ":common",
    ],
)

cc_library(
    name = "bikel5",
    srcs = glob(
        [
            "src/kem/bike/**/*.c",
            "src/kem/bike/**/*.h",
        ],
        exclude = [
        ],
    ),
    hdrs = [
    ] + glob(
        [
        ],
    ),
    copts = [
        "-include src/kem/bike/functions_renaming.h",
    ] + COPTS,
    includes = [
        "$(BINDIR)/external/liboqs/src",
        "src/common/pqclean_shims",
    ],
    local_defines = [
        "DISABLE_VPCLMUL",
        "FUNC_PREFIX=OQS_KEM_bike_l5",
        "LEVEL=5",
        "X86_64",
    ],
    deps = [
        ":common",
    ],
)

cc_library(
    name = "bike",
    deps = [
        "bikel1",
        "bikel3",
        "bikel5",
    ],
)

cc_library(
    name = "frodokem",
    srcs = glob(
        [
            "src/kem/frodokem/external/*.c",
            "src/kem/frodokem/external/*.h",
            "src/kem/frodokem/*.c",
            "src/kem/frodokem/*.h",
        ],
        exclude = [
            "src/kem/frodokem/external/kem.c",
            "src/kem/frodokem/external/util.c",
            "src/kem/frodokem/external/noise.c",
            "src/kem/frodokem/external/frodo_macrify_*.c",
        ],
    ),
    hdrs = [
        "src/kem/frodokem/external/kem.c",
        "src/kem/frodokem/external/util.c",
        "src/kem/frodokem/external/noise.c",
    ] + glob(
        [
            "src/kem/frodokem/external/frodo_macrify_*.c",
        ],
    ),
    copts = COPTS,
    includes = [
        "$(BINDIR)/external/liboqs/src",
        "src/kem/frodokem/external",
    ],
    local_defines = [
    ],
    deps = [
        ":common",
    ],
)

cc_library(
    name = "hqc128clean",
    srcs = glob(
        [
            "src/kem/hqc/pqclean_hqc-rmrs-128_clean/*.c",
            "src/kem/hqc/pqclean_hqc-rmrs-128_clean/*.h",
        ],
        exclude = [
        ],
    ),
    hdrs = [
    ] + glob(
        [
        ],
    ),
    copts = COPTS,
    includes = [
        "src/common/pqclean_shims",
        "src/kem/hqc/pqclean_hqc-rmrs-128_clean",
    ],
    local_defines = [],
    deps = [
        ":common",
    ],
)

cc_library(
    name = "hqc128avx2",
    srcs = glob(
        [
            "src/kem/hqc/pqclean_hqc-rmrs-128_avx2/*.c",
            "src/kem/hqc/pqclean_hqc-rmrs-128_avx2/*.h",
        ],
        exclude = [
        ],
    ),
    hdrs = [
    ] + glob(
        [
        ],
    ),
    copts = COPTS,
    includes = [
        "src/common/pqclean_shims",
        "src/kem/hqc/pqclean_hqc-rmrs-128_avx2",
    ],
    local_defines = [],
    deps = [
        ":common",
    ],
)

cc_library(
    name = "hqc192clean",
    srcs = glob(
        [
            "src/kem/hqc/pqclean_hqc-rmrs-192_clean/*.c",
            "src/kem/hqc/pqclean_hqc-rmrs-192_clean/*.h",
        ],
        exclude = [
        ],
    ),
    hdrs = [
    ] + glob(
        [
        ],
    ),
    copts = COPTS,
    includes = [
        "src/common/pqclean_shims",
        "src/kem/hqc/pqclean_hqc-rmrs-192_clean",
    ],
    local_defines = [],
    deps = [
        ":common",
    ],
)

cc_library(
    name = "hqc192avx2",
    srcs = glob(
        [
            "src/kem/hqc/pqclean_hqc-rmrs-192_avx2/*.c",
            "src/kem/hqc/pqclean_hqc-rmrs-192_avx2/*.h",
        ],
        exclude = [
        ],
    ),
    hdrs = [
    ] + glob(
        [
        ],
    ),
    copts = COPTS,
    includes = [
        "src/common/pqclean_shims",
        "src/kem/hqc/pqclean_hqc-rmrs-192_avx2",
    ],
    local_defines = [],
    deps = [
        ":common",
    ],
)

cc_library(
    name = "hqc256clean",
    srcs = glob(
        [
            "src/kem/hqc/pqclean_hqc-rmrs-256_clean/*.c",
            "src/kem/hqc/pqclean_hqc-rmrs-256_clean/*.h",
        ],
        exclude = [
        ],
    ),
    hdrs = [
    ] + glob(
        [
        ],
    ),
    copts = COPTS,
    includes = [
        "src/common/pqclean_shims",
        "src/kem/hqc/pqclean_hqc-rmrs-256_clean",
    ],
    local_defines = [],
    deps = [
        ":common",
    ],
)

cc_library(
    name = "hqc256avx2",
    srcs = glob(
        [
            "src/kem/hqc/pqclean_hqc-rmrs-256_avx2/*.c",
            "src/kem/hqc/pqclean_hqc-rmrs-256_avx2/*.h",
        ],
        exclude = [
        ],
    ),
    hdrs = [
    ] + glob(
        [
        ],
    ),
    copts = COPTS,
    includes = [
        "src/common/pqclean_shims",
        "src/kem/hqc/pqclean_hqc-rmrs-256_avx2",
    ],
    local_defines = [],
    deps = [
        ":common",
    ],
)

cc_library(
    name = "hqc",
    srcs = [
        "src/kem/hqc/kem_hqc_128.c",
        "src/kem/hqc/kem_hqc_192.c",
        "src/kem/hqc/kem_hqc_256.c",
    ],
    hdrs = [
        "src/kem/hqc/kem_hqc.h",
    ],
    copts = COPTS,
    deps = [
        ":hqc128avx2",
        ":hqc128clean",
        ":hqc192avx2",
        ":hqc192clean",
        ":hqc256avx2",
        ":hqc256clean",
    ],
)

cc_library(
    name = "kyber512",
    srcs = glob(
        [
            "src/kem/kyber/pqcrystals-kyber_kyber512_*/*.c",
            "src/kem/kyber/pqcrystals-kyber_kyber512_*/*.S",
            "src/kem/kyber/pqcrystals-kyber_kyber512_*/*.h",
        ],
        exclude = [
            "src/kem/kyber/*_aarch64",
        ],
    ),
    hdrs = [
    ] + glob(
        [
        ],
    ),
    copts = COPTS,
    includes = [
        "$(BINDIR)/external/liboqs/src",
        "src/common/pqclean_shims",
    ],
    local_defines = [
        "KYBER_K=2",
    ],
    deps = [
        ":common",
    ],
)

cc_library(
    name = "kyber768",
    srcs = glob(
        [
            "src/kem/kyber/pqcrystals-kyber_kyber768_*/*.c",
            "src/kem/kyber/pqcrystals-kyber_kyber768_*/*.S",
            "src/kem/kyber/pqcrystals-kyber_kyber768_*/*.h",
        ],
        exclude = [
            "src/kem/kyber/*_aarch64",
        ],
    ),
    hdrs = [
    ] + glob(
        [
        ],
    ),
    copts = COPTS,
    includes = [
        "$(BINDIR)/external/liboqs/src",
        "src/common/pqclean_shims",
    ],
    local_defines = [
        "KYBER_K=3",
    ],
    deps = [
        ":common",
    ],
)

cc_library(
    name = "kyber1024",
    srcs = glob(
        [
            "src/kem/kyber/pqcrystals-kyber_kyber1024_*/*.c",
            "src/kem/kyber/pqcrystals-kyber_kyber1024_*/*.S",
            "src/kem/kyber/pqcrystals-kyber_kyber1024_*/*.h",
        ],
        exclude = [
            "src/kem/kyber/*_aarch64",
        ],
    ),
    hdrs = [
    ] + glob(
        [
        ],
    ),
    copts = COPTS,
    includes = [
        "$(BINDIR)/external/liboqs/src",
        "src/common/pqclean_shims",
    ],
    local_defines = [
        "KYBER_K=4",
    ],
    deps = [
        ":common",
    ],
)

cc_library(
    name = "kyber",
    srcs = [
        "src/kem/kyber/kem_kyber_1024.c",
        "src/kem/kyber/kem_kyber_512.c",
        "src/kem/kyber/kem_kyber_768.c",
    ],
    hdrs = [
        "src/kem/kyber/kem_kyber.h",
    ],
    copts = COPTS,
    deps = [
        ":kyber1024",
        ":kyber512",
        ":kyber768",
    ],
)

cc_library(
    name = "ntruprime",
    srcs = glob(
        [
            "src/kem/ntruprime/**/*.c",
            "src/kem/ntruprime/**/*.h",
        ],
        exclude = [
        ],
    ),
    hdrs = [
    ] + glob(
        [
        ],
    ),
    copts = COPTS,
    includes = [
        "$(BINDIR)/external/liboqs/src",
        "src/common/pqclean_shims",
    ],
    local_defines = [
    ],
    deps = [
        ":common",
    ],
)

cc_library(
    name = "kem",
    srcs = [
        "src/kem/kem.c",
    ],
    hdrs = [
        "src/kem/kem.h",
    ],
    copts = COPTS,
    includes = [
        "$(BINDIR)/external/liboqs/src",
        "src/common/pqclean_shims",
    ],
    local_defines = [
        "OQS_HAVE_ALIGNED_ALLOC",
        "OQS_HAVE_EXPLICIT_BZERO",
        "OQS_HAVE_MEMALIGN",
        "DISABLE_VPCLMUL",
        "X86_64",
        "OQS_HAVE_POSIX_MEMALIGN",
    ],
    deps = [
        ":bike",
        ":classic_mceliece",
        ":frodokem",
        ":headers",
        ":hqc",
        ":kyber",
        ":ntruprime",
    ],
)

cc_library(
    name = "pqcrystals-dilithium_dilithium2",
    srcs = [
    ] + glob(
        [
            "src/sig/dilithium/pqcrystals-dilithium_dilithium2*/*.c",
            "src/sig/dilithium/pqcrystals-dilithium_dilithium2*/*.S",
        ],
        exclude = [
            "src/sig/**/*aarch64/*.c",
            "src/sig/**/*aarch64/*.S",
        ],
    ),
    hdrs = [
    ] + glob(
        [
            "src/sig/dilithium/pqcrystals-dilithium_dilithium2*/*.h",
        ],
        exclude = [
            "src/sig/**/*aarch64/*.h",
        ],
    ),
    copts = COPTS,
    includes = [
        "$(BINDIR)/external/liboqs/src",
        "src/common/pqclean_shims",
        "src/sig/dilithium/pqcrystals-dilithium_dilithium2_avx2",
    ],
    local_defines = [
        "DILITHIUM_MODE=2",
        "DILITHIUM_RANDOMIZED_SIGNING",
        "OQS_HAVE_ALIGNED_ALLOC",
        "OQS_HAVE_EXPLICIT_BZERO",
        "OQS_HAVE_MEMALIGN",
        "DISABLE_VPCLMUL",
        "OQS_HAVE_POSIX_MEMALIGN",
    ],
    deps = [
        ":common",
        ":headers",
    ],
)

cc_library(
    name = "pqcrystals-dilithium_dilithium3",
    srcs = [
    ] + glob(
        [
            "src/sig/dilithium/pqcrystals-dilithium_dilithium3*/*.c",
            "src/sig/dilithium/pqcrystals-dilithium_dilithium3*/*.S",
        ],
        exclude = [
            "src/sig/**/*aarch64/*.c",
            "src/sig/**/*aarch64/*.S",
        ],
    ),
    hdrs = [
    ] + glob(
        [
            "src/sig/dilithium/pqcrystals-dilithium_dilithium3*/*.h",
        ],
        exclude = [
            "src/sig/**/*aarch64/*.h",
        ],
    ),
    copts = COPTS,
    includes = [
        "$(BINDIR)/external/liboqs/src",
        "src/common/pqclean_shims",
    ],
    local_defines = [
        "DILITHIUM_MODE=3",
        "DILITHIUM_RANDOMIZED_SIGNING",
        "OQS_HAVE_ALIGNED_ALLOC",
        "OQS_HAVE_EXPLICIT_BZERO",
        "OQS_HAVE_MEMALIGN",
        "DISABLE_VPCLMUL",
        "OQS_HAVE_POSIX_MEMALIGN",
    ],
    deps = [
        ":common",
        ":headers",
    ],
)

cc_library(
    name = "pqcrystals-dilithium_dilithium5",
    srcs = [
    ] + glob(
        [
            "src/sig/dilithium/pqcrystals-dilithium_dilithium5*/*.c",
            "src/sig/dilithium/pqcrystals-dilithium_dilithium5*/*.S",
        ],
        exclude = [
            "src/sig/**/*aarch64/*.c",
            "src/sig/**/*aarch64/*.S",
        ],
    ),
    hdrs = [
    ] + glob(
        [
            "src/sig/dilithium/pqcrystals-dilithium_dilithium5*/*.h",
        ],
        exclude = [
            "src/sig/**/*aarch64/*.h",
        ],
    ),
    copts = COPTS,
    includes = [
        "$(BINDIR)/external/liboqs/src",
        "src/common/pqclean_shims",
    ],
    local_defines = [
        "DILITHIUM_MODE=5",
        "DILITHIUM_RANDOMIZED_SIGNING",
        "OQS_HAVE_ALIGNED_ALLOC",
        "OQS_HAVE_EXPLICIT_BZERO",
        "OQS_HAVE_MEMALIGN",
        "DISABLE_VPCLMUL",
        "OQS_HAVE_POSIX_MEMALIGN",
    ],
    deps = [
        ":common",
    ],
)

cc_library(
    name = "dilithium",
    srcs = [
    ] + glob(
        [
            "src/sig/dilithium/*.c",
        ],
        exclude = [
        ],
    ),
    hdrs = [
        "src/sig/dilithium/sig_dilithium.h",
    ] + glob(
        [
        ],
        exclude = [
        ],
    ),
    copts = COPTS,
    includes = [
        "$(BINDIR)/external/liboqs/src",
        "src/common/pqclean_shims",
    ],
    local_defines = [
        "DILITHIUM_RANDOMIZED_SIGNING",
        "OQS_HAVE_ALIGNED_ALLOC",
        "OQS_HAVE_EXPLICIT_BZERO",
        "OQS_HAVE_MEMALIGN",
        "DISABLE_VPCLMUL",
        "OQS_HAVE_POSIX_MEMALIGN",
    ],
    deps = [
        ":common",
        ":pqcrystals-dilithium_dilithium2",
        ":pqcrystals-dilithium_dilithium3",
        ":pqcrystals-dilithium_dilithium5",
    ],
)

cc_library(
    name = "falcon",
    srcs = [
    ] + glob(
        [
            "src/sig/falcon/**/*.c",
            "src/sig/falcon/**/*.S",
        ],
        exclude = [
        ],
    ),
    hdrs = [
    ] + glob(
        [
            "src/sig/falcon/**/*.h",
        ],
        exclude = [
        ],
    ),
    copts = COPTS,
    includes = [
        "$(BINDIR)/external/liboqs/src",
        "src/common/pqclean_shims",
    ],
    local_defines = [
        "DILITHIUM_RANDOMIZED_SIGNING",
        "OQS_HAVE_ALIGNED_ALLOC",
        "OQS_HAVE_EXPLICIT_BZERO",
        "OQS_HAVE_MEMALIGN",
        "DISABLE_VPCLMUL",
        "OQS_HAVE_POSIX_MEMALIGN",
    ],
    deps = [
        ":common",
    ],
)

cc_library(
    name = "sphincs",
    srcs = [
    ] + glob(
        [
            "src/sig/sphincs/**/*.c",
            "src/sig/sphincs/**/*.S",
        ],
        exclude = [
            "src/sig/**/*aarch64/*.c",
            "src/sig/**/*aarch64/*.S",
        ],
    ),
    hdrs = [
    ] + glob(
        [
            "src/sig/sphincs/**/*.h",
        ],
        exclude = [
            "src/sig/**/*aarch64/*.h",
        ],
    ),
    copts = COPTS,
    includes = [
        "$(BINDIR)/external/liboqs/src",
        "src/common/pqclean_shims",
    ],
    local_defines = [
        "DILITHIUM_RANDOMIZED_SIGNING",
        "OQS_HAVE_ALIGNED_ALLOC",
        "OQS_HAVE_EXPLICIT_BZERO",
        "OQS_HAVE_MEMALIGN",
        "DISABLE_VPCLMUL",
        "OQS_HAVE_POSIX_MEMALIGN",
    ],
    deps = [
        ":common",
    ],
)

cc_library(
    name = "sig",
    srcs = [
        "src/sig/sig.c",
    ],
    hdrs = [
        "src/sig/sig.h",
    ],
    copts = COPTS,
    includes = [
        "$(BINDIR)/external/liboqs/src",
        "src/common/pqclean_shims",
    ],
    local_defines = [
        "DILITHIUM_RANDOMIZED_SIGNING",
        "OQS_HAVE_ALIGNED_ALLOC",
        "OQS_HAVE_EXPLICIT_BZERO",
        "OQS_HAVE_MEMALIGN",
        "DISABLE_VPCLMUL",
        "OQS_HAVE_POSIX_MEMALIGN",
    ],
    deps = [
        ":common",
        ":dilithium",
        ":falcon",
        ":pqcrystals-dilithium_dilithium2",
        ":pqcrystals-dilithium_dilithium3",
        ":pqcrystals-dilithium_dilithium5",
        ":sphincs",
    ],
)

cc_library(
    name = "oqs",
    srcs = [
    ],
    hdrs = [
    ],
    copts = COPTS,
    includes = [
        "$(BINDIR)/external/liboqs/src",
        "src/common/pqclean_shims",
    ],
    deps = [
        ":common",
        ":kem",
        ":sig",
    ],
)
