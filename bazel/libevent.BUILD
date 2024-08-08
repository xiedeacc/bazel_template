load("@bazel_template//bazel:common.bzl", "extract_symbols", "template_rule")

package(default_visibility = ["//visibility:public"])

COPTS = [
    "-Wall",
    "-W",
    "-Waddress",
    "-Wbad-function-cast",
    "-Wdeclaration-after-statement",
    "-Wfloat-equal",
    "-Winit-self",
    "-Wmissing-declarations",
    "-Wmissing-field-initializers",
    "-Wmissing-prototypes",
    "-Wnested-externs",
    "-Wnormalized=id",
    "-Woverride-init",
    "-Wpointer-arith",
    "-Wredundant-decls",
    "-Wstrict-prototypes",
    "-Wundef",
    "-Wwrite-strings",
    "-Wno-unused-parameter",
    "-Wno-unused-function",
    "-Wno-pragmas",
    "-O3",
    #"-I$(GENDIR)/include",
]

template_rule(
    name = "event-config_h",
    src = "event-config.h.cmake",
    out = "include/event2/event-config.h",
    substitutions =
        select({
            "@bazel_template//bazel:linux_aarch64": {
                "#cmakedefine EVENT__HAVE_EPOLL_PWAIT2 1": "/* #undef EVENT__HAVE_EPOLL_PWAIT2 */",
            },
            "//conditions:default": {
                "#cmakedefine EVENT__HAVE_EPOLL_PWAIT2 1": "#define EVENT__HAVE_EPOLL_PWAIT2 1",
            },
        }) |
        select({
            "@bazel_template//bazel:clang": {
                "#cmakedefine EVENT__HAVE_ARC4RANDOM 1": "/* #undef EVENT__HAVE_ARC4RANDOM */",
                "#cmakedefine EVENT__HAVE_ARC4RANDOM_BUF 1": "/* #undef EVENT__HAVE_ARC4RANDOM_BUF */",
                "#cmakedefine EVENT__HAVE_ARC4RANDOM_ADDRANDOM 1": "/* #undef EVENT__HAVE_ARC4RANDOM_ADDRANDOM */",
            },
            "@bazel_template//bazel:gcc": {
                "#cmakedefine EVENT__HAVE_ARC4RANDOM 1": "#define EVENT__HAVE_ARC4RANDOM 1",
                "#cmakedefine EVENT__HAVE_ARC4RANDOM_BUF 1": "#define EVENT__HAVE_ARC4RANDOM_BUF 1",
                "#cmakedefine EVENT__HAVE_ARC4RANDOM_ADDRANDOM 1": "#define EVENT__HAVE_ARC4RANDOM_ADDRANDOM 1",
            },
            "//conditions:default": {
                "#cmakedefine EVENT__HAVE_ARC4RANDOM 1": "/* #undef EVENT__HAVE_ARC4RANDOM */",
                "#cmakedefine EVENT__HAVE_ARC4RANDOM_BUF 1": "/* #undef EVENT__HAVE_ARC4RANDOM_BUF */",
                "#cmakedefine EVENT__HAVE_ARC4RANDOM_ADDRANDOM 1": "/* #undef EVENT__HAVE_ARC4RANDOM_ADDRANDOM */",
            },
        }) |
        {
            "#ifndef EVENT2_EVENT_CONFIG_H_INCLUDED_": "#ifndef EVENT2_EVENT_CONFIG_H_INCLUDED_",
            "#define EVENT2_EVENT_CONFIG_H_INCLUDED_": "#define EVENT2_EVENT_CONFIG_H_INCLUDED_",
            "#define EVENT__NUMERIC_VERSION @EVENT_NUMERIC_VERSION@": "#define EVENT__NUMERIC_VERSION 0x02020100",
            "#define EVENT__PACKAGE_VERSION \"@EVENT_PACKAGE_VERSION@\"": "#define EVENT__PACKAGE_VERSION \"2.2.1\"",
            "#define EVENT__VERSION_MAJOR @EVENT_VERSION_MAJOR@": "#define EVENT__VERSION_MAJOR 2",
            "#define EVENT__VERSION_MINOR @EVENT_VERSION_MINOR@": "#define EVENT__VERSION_MINOR 2",
            "#define EVENT__VERSION_PATCH @EVENT_VERSION_PATCH@": "#define EVENT__VERSION_PATCH 1",
            "#define EVENT__VERSION \"@EVENT_VERSION@\"": "#define EVENT__VERSION \"2.2.1-alpha-dev\"",
            "#define EVENT__PACKAGE \"libevent\"": "#define EVENT__PACKAGE \"libevent\"",
            "#define EVENT__PACKAGE_BUGREPORT \"\"": "#define EVENT__PACKAGE_BUGREPORT \"\"",
            "#define EVENT__PACKAGE_NAME \"\"": "#define EVENT__PACKAGE_NAME \"\"",
            "#define EVENT__PACKAGE_STRING \"\"": "#define EVENT__PACKAGE_STRING \"\"",
            "#define EVENT__PACKAGE_TARNAME \"\"": "#define EVENT__PACKAGE_TARNAME \"\"",
            "#cmakedefine EVENT__DISABLE_DEBUG_MODE 1": "/* #undef EVENT__DISABLE_DEBUG_MODE */",
            "#cmakedefine EVENT__DISABLE_MM_REPLACEMENT 1": "/* #undef EVENT__DISABLE_MM_REPLACEMENT */",
            "#cmakedefine EVENT__DISABLE_THREAD_SUPPORT 1": "/* #undef EVENT__DISABLE_THREAD_SUPPORT */",
            "#cmakedefine EVENT__HAVE_ACCEPT4 1": "#define EVENT__HAVE_ACCEPT4 1",
            "#cmakedefine EVENT__HAVE_ARPA_INET_H 1": "#define EVENT__HAVE_ARPA_INET_H 1",
            "#cmakedefine EVENT__HAVE_CLOCK_GETTIME 1": "#define EVENT__HAVE_CLOCK_GETTIME 1",
            "#define EVENT__HAVE_DECL_CTL_KERN @EVENT__HAVE_DECL_CTL_KERN@": "#define EVENT__HAVE_DECL_CTL_KERN 0",
            "#define EVENT__HAVE_DECL_KERN_ARND @EVENT__HAVE_DECL_KERN_ARND@": "#define EVENT__HAVE_DECL_KERN_ARND 0",
            "#cmakedefine EVENT__HAVE_GETRANDOM 1": "#define EVENT__HAVE_GETRANDOM 1",
            "#cmakedefine EVENT__HAVE_DEVPOLL 1": "/* #undef EVENT__HAVE_DEVPOLL */",
            "#cmakedefine EVENT__HAVE_NETDB_H 1": "#define EVENT__HAVE_NETDB_H 1",
            "#cmakedefine EVENT__HAVE_FD_MASK 1": "#define EVENT__HAVE_FD_MASK 1",
            "#cmakedefine EVENT__HAVE_EPOLL 1": "#define EVENT__HAVE_EPOLL 1",
            "#cmakedefine EVENT__HAVE_EPOLL_CREATE1 1": "#define EVENT__HAVE_EPOLL_CREATE1 1",
            "#cmakedefine EVENT__HAVE_WEPOLL 1": "/* #undef EVENT__HAVE_WEPOLL */",
            "#cmakedefine EVENT__HAVE_EVENTFD 1": "#define EVENT__HAVE_EVENTFD 1",
            "#cmakedefine EVENT__HAVE_EVENT_PORTS 1": "/* #undef EVENT__HAVE_EVENT_PORTS */",
            "#cmakedefine EVENT__HAVE_FCNTL 1": "#define EVENT__HAVE_FCNTL 1",
            "#cmakedefine EVENT__HAVE_FCNTL_H 1": "#define EVENT__HAVE_FCNTL_H 1",
            "#cmakedefine EVENT__HAVE_GETADDRINFO 1": "#define EVENT__HAVE_GETADDRINFO 1",
            "#cmakedefine EVENT__HAVE_GETEGID 1": "#define EVENT__HAVE_GETEGID 1",
            "#cmakedefine EVENT__HAVE_GETEUID 1": "#define EVENT__HAVE_GETEUID 1",
            "#cmakedefine EVENT__HAVE_GETHOSTBYNAME_R 1": "#define EVENT__HAVE_GETHOSTBYNAME_R 1",
            "#cmakedefine EVENT__HAVE_GETHOSTBYNAME_R_3_ARG 1": "/* #undef EVENT__HAVE_GETHOSTBYNAME_R_3_ARG */",
            "#cmakedefine EVENT__HAVE_GETHOSTBYNAME_R_5_ARG 1": "/* #undef EVENT__HAVE_GETHOSTBYNAME_R_5_ARG */",
            "#cmakedefine EVENT__HAVE_GETHOSTBYNAME_R_6_ARG 1": "#define EVENT__HAVE_GETHOSTBYNAME_R_6_ARG 1",
            "#cmakedefine EVENT__HAVE_GETIFADDRS 1": "#define EVENT__HAVE_GETIFADDRS 1",
            "#cmakedefine EVENT__HAVE_GETNAMEINFO 1": "#define EVENT__HAVE_GETNAMEINFO 1",
            "#cmakedefine EVENT__HAVE_GETPROTOBYNUMBER 1": "#define EVENT__HAVE_GETPROTOBYNUMBER 1",
            "#cmakedefine EVENT__HAVE_GETSERVBYNAME 1": "#define EVENT__HAVE_GETSERVBYNAME 1",
            "#cmakedefine EVENT__HAVE_GETTIMEOFDAY 1": "#define EVENT__HAVE_GETTIMEOFDAY 1",
            "#cmakedefine EVENT__HAVE_IFADDRS_H 1": "#define EVENT__HAVE_IFADDRS_H 1",
            "#cmakedefine EVENT__HAVE_INET_NTOP 1": "#define EVENT__HAVE_INET_NTOP 1",
            "#cmakedefine EVENT__HAVE_INET_PTON 1": "#define EVENT__HAVE_INET_PTON 1",
            "#cmakedefine EVENT__HAVE_INTTYPES_H 1": "#define EVENT__HAVE_INTTYPES_H 1",
            "#cmakedefine EVENT__HAVE_ISSETUGID 1": "/* #undef EVENT__HAVE_ISSETUGID */",
            "#cmakedefine EVENT__HAVE_KQUEUE 1": "/* #undef EVENT__HAVE_KQUEUE */",
            "#cmakedefine EVENT__HAVE_LIBZ 1": "/* #undef EVENT__HAVE_LIBZ */",
            "#cmakedefine EVENT__HAVE_MACH_ABSOLUTE_TIME 1": "/* #undef EVENT__HAVE_MACH_ABSOLUTE_TIME */",
            "#cmakedefine EVENT__HAVE_MACH_MACH_TIME_H 1": "/* #undef EVENT__HAVE_MACH_MACH_TIME_H */",
            "#cmakedefine EVENT__HAVE_MACH_MACH_H 1": "/* #undef EVENT__HAVE_MACH_MACH_H */",
            "#cmakedefine EVENT__HAVE_MMAP 1": "#define EVENT__HAVE_MMAP 1",
            "#cmakedefine EVENT__HAVE_MMAP64 1": "#define EVENT__HAVE_MMAP64 1",
            "#cmakedefine EVENT__HAVE_NANOSLEEP 1": "#define EVENT__HAVE_NANOSLEEP 1",
            "#cmakedefine EVENT__HAVE_USLEEP 1": "#define EVENT__HAVE_USLEEP 1",
            "#cmakedefine EVENT__HAVE_NETINET_IN6_H 1": "/* #undef EVENT__HAVE_NETINET_IN6_H */",
            "#cmakedefine EVENT__HAVE_NETINET_IN_H 1": "#define EVENT__HAVE_NETINET_IN_H 1",
            "#cmakedefine EVENT__HAVE_NETINET_TCP_H 1": "#define EVENT__HAVE_NETINET_TCP_H 1",
            "#cmakedefine EVENT__HAVE_SYS_UN_H 1": "#define EVENT__HAVE_SYS_UN_H 1",
            "#cmakedefine EVENT__HAVE_AFUNIX_H 1": "/* #undef EVENT__HAVE_AFUNIX_H */",
            "#cmakedefine EVENT__HAVE_OPENSSL 1": "#define EVENT__HAVE_OPENSSL 1",
            "#cmakedefine EVENT__HAVE_MBEDTLS 1": "#define EVENT__HAVE_MBEDTLS 1",
            "#cmakedefine EVENT__HAVE_PIPE 1": "#define EVENT__HAVE_PIPE 1",
            "#cmakedefine EVENT__HAVE_PIPE2 1": "#define EVENT__HAVE_PIPE2 1",
            "#cmakedefine EVENT__HAVE_POLL 1": "#define EVENT__HAVE_POLL 1",
            "#cmakedefine EVENT__HAVE_PTHREADS 1": "#define EVENT__HAVE_PTHREADS 1",
            "#cmakedefine EVENT__HAVE_PTHREAD_MUTEXATTR_SETPROTOCOL 1": "#define EVENT__HAVE_PTHREAD_MUTEXATTR_SETPROTOCOL 1",
            "#cmakedefine EVENT__HAVE_PUTENV 1": "#define EVENT__HAVE_PUTENV 1",
            "#cmakedefine EVENT__HAVE_SA_FAMILY_T 1": "#define EVENT__HAVE_SA_FAMILY_T 1",
            "#cmakedefine EVENT__HAVE_SELECT 1": "#define EVENT__HAVE_SELECT 1",
            "#cmakedefine EVENT__HAVE_SETENV 1": "#define EVENT__HAVE_SETENV 1",
            "#cmakedefine EVENT__HAVE_SETFD 1": "#define EVENT__HAVE_SETFD 1",
            "#cmakedefine EVENT__HAVE_SETRLIMIT 1": "#define EVENT__HAVE_SETRLIMIT 1",
            "#cmakedefine EVENT__HAVE_SENDFILE 1": "#define EVENT__HAVE_SENDFILE 1",
            "#cmakedefine EVENT__HAVE_PREAD 1": "#define EVENT__HAVE_PREAD 1",
            "#cmakedefine EVENT__HAVE_SIGACTION 1": "#define EVENT__HAVE_SIGACTION 1",
            "#cmakedefine EVENT__HAVE_SOCKETPAIR 1": "#define EVENT__HAVE_SOCKETPAIR 1",
            "#cmakedefine EVENT__HAVE_STRSIGNAL 1": "#define EVENT__HAVE_STRSIGNAL 1",
            "#cmakedefine EVENT__HAVE_STDARG_H 1": "#define EVENT__HAVE_STDARG_H 1",
            "#cmakedefine EVENT__HAVE_STDDEF_H 1": "#define EVENT__HAVE_STDDEF_H 1",
            "#cmakedefine EVENT__HAVE_STDINT_H 1": "#define EVENT__HAVE_STDINT_H 1",
            "#cmakedefine EVENT__HAVE_STDLIB_H 1": "#define EVENT__HAVE_STDLIB_H 1",
            "#cmakedefine EVENT__HAVE_STRLCPY 1": "/* #undef EVENT__HAVE_STRLCPY */",
            "#cmakedefine EVENT__HAVE_STRSEP 1": "#define EVENT__HAVE_STRSEP 1",
            "#cmakedefine EVENT__HAVE_STRTOK_R 1": "#define EVENT__HAVE_STRTOK_R 1",
            "#cmakedefine EVENT__HAVE_STRTOLL 1": "#define EVENT__HAVE_STRTOLL 1",
            "#cmakedefine EVENT__HAVE__GMTIME64_S 1": "/* #undef EVENT__HAVE__GMTIME64_S */",
            "#cmakedefine EVENT__HAVE__GMTIME64 1": "/* #undef EVENT__HAVE__GMTIME64 */",
            "#cmakedefine EVENT__HAVE_STRUCT_ADDRINFO 1": "#define EVENT__HAVE_STRUCT_ADDRINFO 1",
            "#cmakedefine EVENT__HAVE_STRUCT_IN6_ADDR 1": "#define EVENT__HAVE_STRUCT_IN6_ADDR 1",
            "#cmakedefine EVENT__HAVE_STRUCT_SOCKADDR_IN6 1": "#define EVENT__HAVE_STRUCT_SOCKADDR_IN6 1",
            "#cmakedefine EVENT__HAVE_STRUCT_SOCKADDR_IN6_SIN6_LEN 1": "/* #undef EVENT__HAVE_STRUCT_SOCKADDR_IN6_SIN6_LEN */",
            "#cmakedefine EVENT__HAVE_STRUCT_SOCKADDR_IN_SIN_LEN 1": "/* #undef EVENT__HAVE_STRUCT_SOCKADDR_IN_SIN_LEN */",
            "#cmakedefine EVENT__HAVE_STRUCT_SOCKADDR_UN 1": "#define EVENT__HAVE_STRUCT_SOCKADDR_UN 1",
            "#cmakedefine EVENT__HAVE_STRUCT_SOCKADDR_STORAGE 1": "#define EVENT__HAVE_STRUCT_SOCKADDR_STORAGE 1",
            "#cmakedefine EVENT__HAVE_STRUCT_SOCKADDR_STORAGE_SS_FAMILY 1": "#define EVENT__HAVE_STRUCT_SOCKADDR_STORAGE_SS_FAMILY 1",
            "#cmakedefine EVENT__HAVE_STRUCT_SOCKADDR_STORAGE___SS_FAMILY 1": "/* #undef EVENT__HAVE_STRUCT_SOCKADDR_STORAGE___SS_FAMILY */",
            "#cmakedefine EVENT__HAVE_STRUCT_LINGER 1": "#define EVENT__HAVE_STRUCT_LINGER 1",
            "#cmakedefine EVENT__HAVE_SYSCTL 1": "/* #undef EVENT__HAVE_SYSCTL */",
            "#cmakedefine EVENT__HAVE_SYS_EVENTFD_H 1": "#define EVENT__HAVE_SYS_EVENTFD_H 1",
            "#cmakedefine EVENT__HAVE_SYS_IOCTL_H 1": "#define EVENT__HAVE_SYS_IOCTL_H 1",
            "#cmakedefine EVENT__HAVE_SYS_MMAN_H 1": "#define EVENT__HAVE_SYS_MMAN_H 1",
            "#cmakedefine EVENT__HAVE_SYS_PARAM_H 1": "#define EVENT__HAVE_SYS_PARAM_H 1",
            "#cmakedefine EVENT__HAVE_SYS_RESOURCE_H 1": "#define EVENT__HAVE_SYS_RESOURCE_H 1",
            "#cmakedefine EVENT__HAVE_SYS_SELECT_H 1": "#define EVENT__HAVE_SYS_SELECT_H 1",
            "#cmakedefine EVENT__HAVE_SYS_SENDFILE_H 1": "#define EVENT__HAVE_SYS_SENDFILE_H 1",
            "#cmakedefine EVENT__HAVE_SYS_SOCKET_H 1": "#define EVENT__HAVE_SYS_SOCKET_H 1",
            "#cmakedefine EVENT__HAVE_SYS_STAT_H 1": "#define EVENT__HAVE_SYS_STAT_H 1",
            "#cmakedefine EVENT__HAVE_SYS_RANDOM_H 1": "#define EVENT__HAVE_SYS_RANDOM_H 1",
            "#cmakedefine EVENT__HAVE_SYS_SYSCTL_H 1": "/* #undef EVENT__HAVE_SYS_SYSCTL_H */",
            "#cmakedefine EVENT__HAVE_SYS_TIMERFD_H 1": "#define EVENT__HAVE_SYS_TIMERFD_H 1",
            "#cmakedefine EVENT__HAVE_SYS_SIGNALFD_H 1": "#define EVENT__HAVE_SYS_SIGNALFD_H 1",
            "#cmakedefine EVENT__HAVE_SYS_TIME_H 1": "#define EVENT__HAVE_SYS_TIME_H 1",
            "#cmakedefine EVENT__HAVE_SYS_TYPES_H 1": "#define EVENT__HAVE_SYS_TYPES_H 1",
            "#cmakedefine EVENT__HAVE_SYS_UIO_H 1": "#define EVENT__HAVE_SYS_UIO_H 1",
            "#cmakedefine EVENT__HAVE_SYS_WAIT_H 1": "#define EVENT__HAVE_SYS_WAIT_H 1",
            "#cmakedefine EVENT__HAVE_ERRNO_H 1": "#define EVENT__HAVE_ERRNO_H 1",
            "#cmakedefine EVENT__HAVE_TIMERADD 1": "#define EVENT__HAVE_TIMERADD 1",
            "#cmakedefine EVENT__HAVE_TIMERCLEAR 1": "#define EVENT__HAVE_TIMERCLEAR 1",
            "#cmakedefine EVENT__HAVE_TIMERFD_CREATE 1": "#define EVENT__HAVE_TIMERFD_CREATE 1",
            "#cmakedefine EVENT__HAVE_TIMERISSET 1": "#define EVENT__HAVE_TIMERISSET 1",
            "#cmakedefine EVENT__HAVE_UINT8_T 1": "#define EVENT__HAVE_UINT8_T 1",
            "#cmakedefine EVENT__HAVE_UINT16_T 1": "#define EVENT__HAVE_UINT16_T 1",
            "#cmakedefine EVENT__HAVE_UINT32_T 1": "#define EVENT__HAVE_UINT32_T 1",
            "#cmakedefine EVENT__HAVE_UINT64_T 1": "#define EVENT__HAVE_UINT64_T 1",
            "#cmakedefine EVENT__HAVE_UINTPTR_T 1": "#define EVENT__HAVE_UINTPTR_T 1",
            "#cmakedefine EVENT__HAVE_UMASK 1": "#define EVENT__HAVE_UMASK 1",
            "#cmakedefine EVENT__HAVE_UNISTD_H 1": "#define EVENT__HAVE_UNISTD_H 1",
            "#cmakedefine EVENT__HAVE_UNSETENV 1": "#define EVENT__HAVE_UNSETENV 1",
            "#cmakedefine EVENT__HAVE_WORKING_KQUEUE 1": "/* #undef EVENT__HAVE_WORKING_KQUEUE */",
            "#define EVENT__SIZEOF_PTHREAD_T @EVENT__SIZEOF_PTHREAD_T@": "#define EVENT__SIZEOF_PTHREAD_T 8",
            "#define EVENT__SIZEOF_INT @EVENT__SIZEOF_INT@": "#define EVENT__SIZEOF_INT 4",
            "#define EVENT__SIZEOF_LONG @EVENT__SIZEOF_LONG@": "#define EVENT__SIZEOF_LONG 8",
            "#define EVENT__SIZEOF_LONG_LONG @EVENT__SIZEOF_LONG_LONG@": "#define EVENT__SIZEOF_LONG_LONG 8",
            "#define EVENT__SIZEOF_OFF_T @EVENT__SIZEOF_OFF_T@": "#define EVENT__SIZEOF_OFF_T 8",
            "#define EVENT__SIZEOF_SHORT @EVENT__SIZEOF_SHORT@": "#define EVENT__SIZEOF_SHORT 2",
            "#define EVENT__SIZEOF_SIZE_T @EVENT__SIZEOF_SIZE_T@": "#define EVENT__SIZEOF_SIZE_T 8",
            "#define EVENT__SIZEOF_SOCKLEN_T @EVENT__SIZEOF_SOCKLEN_T@": "#define EVENT__SIZEOF_SOCKLEN_T 4",
            "#define EVENT__SIZEOF_VOID_P @EVENT__SIZEOF_VOID_P@": "#define EVENT__SIZEOF_VOID_P 8",
            "#define EVENT__SIZEOF_TIME_T @EVENT__SIZEOF_TIME_T@": "#define EVENT__SIZEOF_TIME_T 8",
            "#ifndef __cplusplus": "#ifndef __cplusplus",
            "#define EVENT__inline @EVENT__inline@": "#define EVENT__inline inline",
            "#endif": "#endif",
            "#cmakedefine EVENT__HAVE___func__ 1": "#define EVENT__HAVE___func__ 1",
            "#cmakedefine EVENT__HAVE___FUNCTION__ 1": "#define EVENT__HAVE___FUNCTION__ 1",
            "#define EVENT__socklen_t @EVENT__socklen_t@": "#define EVENT__socklen_t socklen_t",
            "#define EVENT__ssize_t @EVENT__ssize_t@": "#define EVENT__ssize_t ssize_t",
            "#endif /* \\EVENT2_EVENT_CONFIG_H_INCLUDED_ */": "#endif /* \\EVENT2_EVENT_CONFIG_H_INCLUDED_ */",
        },
)

genrule(
    name = "evconfig-private_h",
    outs = ["include/evconfig-private.h"],
    cmd = "\n".join([
        "cat <<'EOF' >$@",
        "",
        "#ifndef EVCONFIG_PRIVATE_H_INCLUDED_",
        "#define EVCONFIG_PRIVATE_H_INCLUDED_",
        "",
        "/* Enable extensions on AIX 3, Interix.  */",
        "/* #undef _ALL_SOURCE */",
        "",
        "/* Enable GNU extensions on systems that have them.  */",
        "#define _GNU_SOURCE 1",
        "",
        "/* Enable threading extensions on Solaris.  */",
        "/* #undef _POSIX_PTHREAD_SEMANTICS */",
        "",
        "/* Enable extensions on HP NonStop.  */",
        "/* #undef _TANDEM_SOURCE */",
        "",
        "/* Enable general extensions on Solaris.  */",
        "/* #undef __EXTENSIONS__ */",
        "",
        "/* Number of bits in a file offset, on hosts where this is settable. */",
        "/* #undef _FILE_OFFSET_BITS */",
        "/* Define for large files, on AIX-style hosts. */",
        "/* #undef _LARGE_FILES */",
        "",
        "/* Define to 1 if on MINIX. */",
        "/* #undef _MINIX */",
        "",
        "/* Define to 2 if the system does not provide POSIX.1 features except with",
        "   this defined. */",
        "/* #undef _POSIX_1_SOURCE */",
        "",
        "/* Define to 1 if you need to in order for `stat' and other things to work. */",
        "/* #undef _POSIX_SOURCE */",
        "",
        "/* Enable POSIX.2 extensions on QNX for getopt */",
        "#ifdef __QNX__",
        "/* #undef __EXT_POSIX2 */",
        "#endif",
        "",
        "#endif",
        "EOF",
    ]),
)

cc_library(
    name = "event",
    srcs = [
        "buffer.c",
        "bufferevent.c",
        "bufferevent_filter.c",
        "bufferevent_pair.c",
        "bufferevent_ratelim.c",
        "bufferevent_sock.c",
        "epoll.c",
        "evdns.c",
        "event.c",
        "event_tagging.c",
        "evmap.c",
        "evrpc.c",
        "evthread.c",
        "evutil.c",
        "evutil_rand.c",
        "evutil_time.c",
        "http.c",
        "listener.c",
        "log.c",
        "poll.c",
        "select.c",
        "sha1.c",
        "signal.c",
        "signalfd.c",
        "strlcpy.c",
        "watch.c",
        "ws.c",
    ],
    hdrs = [
        "arc4random.c",
        ":evconfig-private_h",
        ":event-config_h",
    ] + glob(
        [
            "include/**/*.h",
            "*.h",
        ],
    ),
    copts = COPTS,
    includes = ["include"],
    local_defines = [
        "HAVE_CONFIG_H",
        "NDEBUG",
        "LITTLE_ENDIAN",
    ],
)

cc_library(
    name = "event_core",
    srcs = [
        "buffer.c",
        "bufferevent.c",
        "bufferevent_filter.c",
        "bufferevent_pair.c",
        "bufferevent_ratelim.c",
        "bufferevent_sock.c",
        "epoll.c",
        "event.c",
        "evmap.c",
        "evthread.c",
        "evutil.c",
        "evutil_rand.c",
        "evutil_time.c",
        "listener.c",
        "log.c",
        "poll.c",
        "select.c",
        "signal.c",
        "signalfd.c",
        "strlcpy.c",
        "watch.c",
    ],
    hdrs = [
        "arc4random.c",
        ":evconfig-private_h",
        ":event-config_h",
    ] + glob(
        [
            "include/**/*.h",
            "*.h",
        ],
    ),
    copts = COPTS,
    includes = ["include"],
    local_defines = [
        "HAVE_CONFIG_H",
        "NDEBUG",
    ],
)

cc_library(
    name = "event_extra",
    srcs = [
        "evdns.c",
        "event_tagging.c",
        "evrpc.c",
        "http.c",
        "sha1.c",
        "ws.c",
    ],
    hdrs = [
        "arc4random.c",
        ":evconfig-private_h",
        ":event-config_h",
    ] + glob(
        [
            "include/**/*.h",
            "*.h",
        ],
    ),
    copts = COPTS,
    includes = ["include"],
    local_defines = [
        "HAVE_CONFIG_H",
        "LITTLE_ENDIAN",
        "NDEBUG",
    ],
)

cc_library(
    name = "event_openssl",
    srcs = [
        "bufferevent_openssl.c",
        "bufferevent_ssl.c",
    ],
    hdrs = [
        "arc4random.c",
        ":evconfig-private_h",
        ":event-config_h",
    ] + glob(
        [
            "include/**/*.h",
            "*.h",
        ],
    ),
    copts = COPTS,
    includes = ["include"],
    local_defines = [
        "HAVE_CONFIG_H",
        "LITTLE_ENDIAN",
        "NDEBUG",
    ],
    deps = [
        "@openssl//:crypto",
        "@openssl//:ssl",
    ],
)

cc_library(
    name = "event_mbedtls",
    srcs = [
        "bufferevent_mbedtls.c",
        "bufferevent_ssl.c",
    ],
    hdrs = [
        "arc4random.c",
        ":evconfig-private_h",
        ":event-config_h",
    ] + glob(
        [
            "include/**/*.h",
            "*.h",
        ],
    ),
    copts = COPTS,
    includes = ["include"],
    local_defines = [
        "HAVE_CONFIG_H",
        "NDEBUG",
        "LITTLE_ENDIAN",
    ],
    deps = [
        "@mbedtls",
    ],
)

cc_library(
    name = "event_pthreads",
    srcs = [
        "evthread_pthread.c",
    ],
    hdrs = [
        "arc4random.c",
        ":evconfig-private_h",
        ":event-config_h",
    ] + glob(
        [
            "include/**/*.h",
            "*.h",
        ],
    ),
    copts = COPTS + ["-pthread"],
    includes = ["include"],
    local_defines = [
        "HAVE_CONFIG_H",
        "NDEBUG",
        "LITTLE_ENDIAN",
    ],
    deps = [
    ],
)
