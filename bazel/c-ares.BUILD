load("@bazel_skylib//lib:selects.bzl", "selects")
load("@bazel_template//bazel:common.bzl", "GLOBAL_COPTS", "template_rule")

package(default_visibility = ["//visibility:public"])

COPTS = GLOBAL_COPTS + selects.with_or({
    ("@bazel_template//bazel:gcc", "@bazel_template//bazel:clang"): [],
    "@bazel_template//bazel:msvc": ["/std:c11"],
    "//conditions:default": [],
}) + select({
    "@bazel_template//bazel:windows_x86_64_gcc": [
        "-std=c90",
        "-fPIC",
    ],
    "@bazel_template//bazel:windows_x86_64_msvc": [],
    "//conditions:default": ["-std=gnu90"],
})

alias(
    name = "ares",
    actual = "c-ares",
)

cc_library(
    name = "c-ares",
    srcs = [
        "src/lib/ares__addrinfo2hostent.c",
        "src/lib/ares__addrinfo_localhost.c",
        "src/lib/ares__buf.c",
        "src/lib/ares__close_sockets.c",
        "src/lib/ares__hosts_file.c",
        "src/lib/ares__htable.c",
        "src/lib/ares__htable_asvp.c",
        "src/lib/ares__htable_strvp.c",
        "src/lib/ares__htable_szvp.c",
        "src/lib/ares__htable_vpvp.c",
        "src/lib/ares__iface_ips.c",
        "src/lib/ares__llist.c",
        "src/lib/ares__parse_into_addrinfo.c",
        "src/lib/ares__slist.c",
        "src/lib/ares__socket.c",
        "src/lib/ares__sortaddrinfo.c",
        "src/lib/ares__threads.c",
        "src/lib/ares__timeval.c",
        "src/lib/ares_android.c",
        "src/lib/ares_cancel.c",
        "src/lib/ares_create_query.c",
        "src/lib/ares_data.c",
        "src/lib/ares_destroy.c",
        "src/lib/ares_dns_mapping.c",
        "src/lib/ares_dns_name.c",
        "src/lib/ares_dns_parse.c",
        "src/lib/ares_dns_record.c",
        "src/lib/ares_dns_write.c",
        "src/lib/ares_event_configchg.c",
        "src/lib/ares_event_epoll.c",
        "src/lib/ares_event_kqueue.c",
        "src/lib/ares_event_poll.c",
        "src/lib/ares_event_select.c",
        "src/lib/ares_event_thread.c",
        "src/lib/ares_event_wake_pipe.c",
        "src/lib/ares_event_win32.c",
        "src/lib/ares_expand_name.c",
        "src/lib/ares_expand_string.c",
        "src/lib/ares_fds.c",
        "src/lib/ares_free_hostent.c",
        "src/lib/ares_free_string.c",
        "src/lib/ares_freeaddrinfo.c",
        "src/lib/ares_getaddrinfo.c",
        "src/lib/ares_getenv.c",
        "src/lib/ares_gethostbyaddr.c",
        "src/lib/ares_gethostbyname.c",
        "src/lib/ares_getnameinfo.c",
        "src/lib/ares_getsock.c",
        "src/lib/ares_init.c",
        "src/lib/ares_library_init.c",
        "src/lib/ares_math.c",
        "src/lib/ares_metrics.c",
        "src/lib/ares_options.c",
        "src/lib/ares_parse_a_reply.c",
        "src/lib/ares_parse_aaaa_reply.c",
        "src/lib/ares_parse_caa_reply.c",
        "src/lib/ares_parse_mx_reply.c",
        "src/lib/ares_parse_naptr_reply.c",
        "src/lib/ares_parse_ns_reply.c",
        "src/lib/ares_parse_ptr_reply.c",
        "src/lib/ares_parse_soa_reply.c",
        "src/lib/ares_parse_srv_reply.c",
        "src/lib/ares_parse_txt_reply.c",
        "src/lib/ares_parse_uri_reply.c",
        "src/lib/ares_platform.c",
        "src/lib/ares_process.c",
        "src/lib/ares_qcache.c",
        "src/lib/ares_query.c",
        "src/lib/ares_rand.c",
        "src/lib/ares_search.c",
        "src/lib/ares_send.c",
        "src/lib/ares_str.c",
        "src/lib/ares_strcasecmp.c",
        "src/lib/ares_strerror.c",
        "src/lib/ares_strsplit.c",
        "src/lib/ares_sysconfig.c",
        "src/lib/ares_sysconfig_files.c",
        "src/lib/ares_sysconfig_mac.c",
        "src/lib/ares_sysconfig_win.c",
        "src/lib/ares_timeout.c",
        "src/lib/ares_update_servers.c",
        "src/lib/ares_version.c",
        "src/lib/inet_net_pton.c",
        "src/lib/inet_ntop.c",
        "src/lib/windows_port.c",
    ],
    hdrs = [
        "src/lib/thirdparty/apple/dnsinfo.h",
        ":ares_build_h",
        ":ares_config_h",
    ] + glob([
        "include/*.h",
        "src/lib/*.h",
    ]),
    copts = COPTS + [
        "-Iexternal/c-ares/src/lib",
        "-I$(GENDIR)/external/c-ares/src/lib",
    ],
    defines = [
        "CARES_BUILDING_LIBRARY",
        "HAVE_CONFIG_H=1",
        #"CARES_STATICLIB",
        "c_ares_EXPORTS",
    ] + select({
        "@platforms//os:linux": [
            "_XOPEN_SOURCE=700",
            "_POSIX_C_SOURCE=200809L",
            "_GNU_SOURCE",
        ],
        "@platforms//os:osx": [
            "_XOPEN_SOURCE=700",
            "_POSIX_C_SOURCE=200809L",
            "_DARWIN_C_SOURCE",
        ],
        "@platforms//os:windows": [
            "_WIN32_WINNT=0x0601",
            "WIN32_LEAN_AND_MEAN",
            "_CRT_NONSTDC_NO_DEPRECATE",
            "CARES_NO_DEPRECATED",
            "_CRT_SECURE_NO_DEPRECATE",
        ],
        "//conditions:default": [],
    }),
    includes = [
        "include",
    ],
)

genrule(
    name = "ares_config_h_in",
    outs = ["src/lib/ares_config.h.in"],
    cmd = "\n".join([
        "cat <<'EOF' >$@",
        "/* Copyright (C) The c-ares project and its contributors",
        " * SPDX-License-Identifier: MIT",
        " */",
        "",
        "/* Generated from ares_config.h.cmake */",
        "",
        "/* Define if building universal (internal helper macro) */",
        "#undef AC_APPLE_UNIVERSAL_BUILD",
        "",
        "/* Defined for build with symbol hiding. */",
        "/* #undef CARES_SYMBOL_HIDING */",
        "",
        "/* Use resolver library to configure cares */",
        "/* #undef CARES_USE_LIBRESOLV */",
        "",
        "/* if a /etc/inet dir is being used */",
        "#undef ETC_INET",
        "",
        "/* Define to the type of arg 2 for gethostname. */",
        "#define GETHOSTNAME_TYPE_ARG2 size_t",
        "",
        "/* Define to the type qualifier of arg 1 for getnameinfo. */",
        "#define GETNAMEINFO_QUAL_ARG1 ",
        "",
        "/* Define to the type of arg 1 for getnameinfo. */",
        "#define GETNAMEINFO_TYPE_ARG1 struct sockaddr *",
        "",
        "/* Define to the type of arg 2 for getnameinfo. */",
        "#define GETNAMEINFO_TYPE_ARG2 socklen_t",
        "",
        "/* Define to the type of args 4 and 6 for getnameinfo. */",
        "#define GETNAMEINFO_TYPE_ARG46 socklen_t",
        "",
        "/* Define to the type of arg 7 for getnameinfo. */",
        "#define GETNAMEINFO_TYPE_ARG7 int",
        "",
        "/* Specifies the number of arguments to getservbyport_r */",
        "#define GETSERVBYPORT_R_ARGS 6",
        "",
        "/* Specifies the number of arguments to getservbyname_r */",
        "#define GETSERVBYNAME_R_ARGS 6",
        "",
        "/* Define to 1 if you have AF_INET6. */",
        "#define HAVE_AF_INET6 1",
        "",
        "/* Define to 1 if you have the <arpa/inet.h> header file. */",
        "#define HAVE_ARPA_INET_H 1",
        "",
        "/* Define to 1 if you have the <arpa/nameser_compat.h> header file. */",
        "#define HAVE_ARPA_NAMESER_COMPAT_H 1",
        "",
        "/* Define to 1 if you have the <arpa/nameser.h> header file. */",
        "#define HAVE_ARPA_NAMESER_H 1",
        "",
        "/* Define to 1 if you have the <assert.h> header file. */",
        "#define HAVE_ASSERT_H 1",
        "",
        "/* Define to 1 if you have the clock_gettime function and monotonic timer. */",
        "#define HAVE_CLOCK_GETTIME_MONOTONIC 1",
        "",
        "/* Define to 1 if you have the closesocket function. */",
        "/* #undef HAVE_CLOSESOCKET */",
        "",
        "/* Define to 1 if you have the CloseSocket camel case function. */",
        "/* #undef HAVE_CLOSESOCKET_CAMEL */",
        "",
        "/* Define to 1 if you have the connect function. */",
        "#define HAVE_CONNECT 1",
        "",
        "/* Define to 1 if you have the connectx function. */",
        "/* #undef HAVE_CONNECTX */",
        "",
        "/* define if the compiler supports basic C++11 syntax */",
        "/* #undef HAVE_CXX11 */",
        "",
        "/* Define to 1 if you have the <dlfcn.h> header file. */",
        "#define HAVE_DLFCN_H 1",
        "",
        "/* Define to 1 if you have the <errno.h> header file. */",
        "#define HAVE_ERRNO_H 1",
        "",
        "/* Define to 1 if you have the <poll.h> header file. */",
        "#define HAVE_POLL_H 1",
        "",
        "/* Define to 1 if you have the poll function. */",
        "#define HAVE_POLL 1",
        "",
        "/* Define to 1 if you have the pipe function. */",
        "#define HAVE_PIPE 1",
        "",
        "/* Define to 1 if you have the pipe2 function. */",
        "#define HAVE_PIPE2 1",
        "",
        "/* Define to 1 if you have the kqueue function. */",
        "/* #undef HAVE_KQUEUE */",
        "",
        "/* Define to 1 if you have the epoll{_create,ctl,wait} functions. */",
        "#define HAVE_EPOLL 1",
        "",
        "/* Define to 1 if you have the fcntl function. */",
        "#define HAVE_FCNTL 1",
        "",
        "/* Define to 1 if you have the <fcntl.h> header file. */",
        "#define HAVE_FCNTL_H 1",
        "",
        "/* Define to 1 if you have a working fcntl O_NONBLOCK function. */",
        "#define HAVE_FCNTL_O_NONBLOCK 1",
        "",
        "/* Define to 1 if you have the freeaddrinfo function. */",
        "#define HAVE_FREEADDRINFO 1",
        "",
        "/* Define to 1 if you have a working getaddrinfo function. */",
        "#define HAVE_GETADDRINFO 1",
        "",
        "/* Define to 1 if the getaddrinfo function is threadsafe. */",
        "/* #undef HAVE_GETADDRINFO_THREADSAFE */",
        "",
        "/* Define to 1 if you have the getenv function. */",
        "#define HAVE_GETENV 1",
        "",
        "/* Define to 1 if you have the gethostname function. */",
        "#define HAVE_GETHOSTNAME 1",
        "",
        "/* Define to 1 if you have the getnameinfo function. */",
        "#define HAVE_GETNAMEINFO 1",
        "",
        "/* Define to 1 if you have the getrandom function. */",
        "#define HAVE_GETRANDOM 1",
        "",
        "/* Define to 1 if you have the getservbyport_r function. */",
        "#define HAVE_GETSERVBYPORT_R 1",
        "",
        "/* Define to 1 if you have the getservbyname_r function. */",
        "#define HAVE_GETSERVBYNAME_R 1",
        "",
        "/* Define to 1 if you have the `gettimeofday' function. */",
        "#define HAVE_GETTIMEOFDAY 1",
        "",
        "/* Define to 1 if you have the `if_indextoname' function. */",
        "#define HAVE_IF_INDEXTONAME 1",
        "",
        "/* Define to 1 if you have the `if_nametoindex' function. */",
        "#define HAVE_IF_NAMETOINDEX 1",
        "",
        "/* Define to 1 if you have the `ConvertInterfaceIndexToLuid' function. */",
        "/* #undef HAVE_CONVERTINTERFACEINDEXTOLUID */",
        "",
        "/* Define to 1 if you have the `ConvertInterfaceLuidToNameA' function. */",
        "/* #undef HAVE_CONVERTINTERFACELUIDTONAMEA */",
        "",
        "/* Define to 1 if you have a IPv6 capable working inet_net_pton function. */",
        "/* #undef HAVE_INET_NET_PTON */",
        "",
        "/* Define to 1 if you have a IPv6 capable working inet_ntop function. */",
        "#define HAVE_INET_NTOP 1",
        "",
        "/* Define to 1 if you have a IPv6 capable working inet_pton function. */",
        "#define HAVE_INET_PTON 1",
        "",
        "/* Define to 1 if you have the <inttypes.h> header file. */",
        "#define HAVE_INTTYPES_H 1",
        "",
        "/* Define to 1 if you have the ioctl function. */",
        "#define HAVE_IOCTL 1",
        "",
        "/* Define to 1 if you have the ioctlsocket function. */",
        "/* #undef HAVE_IOCTLSOCKET */",
        "",
        "/* Define to 1 if you have the IoctlSocket camel case function. */",
        "/* #undef HAVE_IOCTLSOCKET_CAMEL */",
        "",
        "/* Define to 1 if you have a working IoctlSocket camel case FIONBIO function.",
        "   */",
        "/* #undef HAVE_IOCTLSOCKET_CAMEL_FIONBIO */",
        "",
        "/* Define to 1 if you have a working ioctlsocket FIONBIO function. */",
        "/* #undef HAVE_IOCTLSOCKET_FIONBIO */",
        "",
        "/* Define to 1 if you have a working ioctl FIONBIO function. */",
        "#define HAVE_IOCTL_FIONBIO 1",
        "",
        "/* Define to 1 if you have a working ioctl SIOCGIFADDR function. */",
        "#define HAVE_IOCTL_SIOCGIFADDR 1",
        "",
        "/* Define to 1 if you have the `resolve' library (-lresolve). */",
        "/* #undef HAVE_LIBRESOLV */",
        "",
        "/* Define to 1 if you have iphlpapi.h */",
        "/* #undef HAVE_IPHLPAPI_H */",
        "",
        "/* Define to 1 if you have netioapi.h */",
        "/* #undef HAVE_NETIOAPI_H */",
        "",
        "/* Define to 1 if you have the <limits.h> header file. */",
        "#define HAVE_LIMITS_H 1",
        "",
        "/* Define to 1 if the compiler supports the 'long long' data type. */",
        "#define HAVE_LONGLONG 1",
        "",
        "/* Define to 1 if you have the malloc.h header file. */",
        "#define HAVE_MALLOC_H 1",
        "",
        "/* Define to 1 if you have the memory.h header file. */",
        "#define HAVE_MEMORY_H 1",
        "",
        "/* Define to 1 if you have the AvailabilityMacros.h header file. */",
        "/* #undef HAVE_AVAILABILITYMACROS_H */",
        "",
        "/* Define to 1 if you have the MSG_NOSIGNAL flag. */",
        "#define HAVE_MSG_NOSIGNAL 1",
        "",
        "/* Define to 1 if you have the <netdb.h> header file. */",
        "#define HAVE_NETDB_H 1",
        "",
        "/* Define to 1 if you have the <netinet/in.h> header file. */",
        "#define HAVE_NETINET_IN_H 1",
        "",
        "/* Define to 1 if you have the <netinet6/in6.h> header file. */",
        "/* #undef HAVE_NETINET6_IN6_H */",
        "",
        "/* Define to 1 if you have the <netinet/tcp.h> header file. */",
        "#define HAVE_NETINET_TCP_H 1",
        "",
        "/* Define to 1 if you have the <net/if.h> header file. */",
        "#define HAVE_NET_IF_H 1",
        "",
        "/* Define to 1 if you have PF_INET6. */",
        "#define HAVE_PF_INET6 1",
        "",
        "/* Define to 1 if you have the recv function. */",
        "#define HAVE_RECV 1",
        "",
        "/* Define to 1 if you have the recvfrom function. */",
        "#define HAVE_RECVFROM 1",
        "",
        "/* Define to 1 if you have the send function. */",
        "#define HAVE_SEND 1",
        "",
        "/* Define to 1 if you have the setsockopt function. */",
        "#define HAVE_SETSOCKOPT 1",
        "",
        "/* Define to 1 if you have a working setsockopt SO_NONBLOCK function. */",
        "/* #undef HAVE_SETSOCKOPT_SO_NONBLOCK */",
        "",
        "/* Define to 1 if you have the <signal.h> header file. */",
        "#define HAVE_SIGNAL_H 1",
        "",
        "/* Define to 1 if your struct sockaddr_in6 has sin6_scope_id. */",
        "#define HAVE_STRUCT_SOCKADDR_IN6_SIN6_SCOPE_ID 1",
        "",
        "/* Define to 1 if you have the socket function. */",
        "#define HAVE_SOCKET 1",
        "",
        "/* Define to 1 if you have the <socket.h> header file. */",
        "/* #undef HAVE_SOCKET_H */",
        "",
        "/* Define to 1 if you have the <stdbool.h> header file. */",
        "#define HAVE_STDBOOL_H 1",
        "",
        "/* Define to 1 if you have the <stdint.h> header file. */",
        "#define HAVE_STDINT_H 1",
        "",
        "/* Define to 1 if you have the <stdlib.h> header file. */",
        "#define HAVE_STDLIB_H 1",
        "",
        "/* Define to 1 if you have the strcasecmp function. */",
        "#define HAVE_STRCASECMP 1",
        "",
        "/* Define to 1 if you have the strcmpi function. */",
        "/* #undef HAVE_STRCMPI */",
        "",
        "/* Define to 1 if you have the strdup function. */",
        "#define HAVE_STRDUP 1",
        "",
        "/* Define to 1 if you have the stricmp function. */",
        "/* #undef HAVE_STRICMP */",
        "",
        "/* Define to 1 if you have the <strings.h> header file. */",
        "#define HAVE_STRINGS_H 1",
        "",
        "/* Define to 1 if you have the <string.h> header file. */",
        "#define HAVE_STRING_H 1",
        "",
        "/* Define to 1 if you have the strncasecmp function. */",
        "#define HAVE_STRNCASECMP 1",
        "",
        "/* Define to 1 if you have the strncmpi function. */",
        "/* #undef HAVE_STRNCMPI */",
        "",
        "/* Define to 1 if you have the strnicmp function. */",
        "/* #undef HAVE_STRNICMP */",
        "",
        "/* Define to 1 if you have the <stropts.h> header file. */",
        "/* #undef HAVE_STROPTS_H */",
        "",
        "/* Define to 1 if you have struct addrinfo. */",
        "#define HAVE_STRUCT_ADDRINFO 1",
        "",
        "/* Define to 1 if you have struct in6_addr. */",
        "#define HAVE_STRUCT_IN6_ADDR 1",
        "",
        "/* Define to 1 if you have struct sockaddr_in6. */",
        "#define HAVE_STRUCT_SOCKADDR_IN6 1",
        "",
        "/* if struct sockaddr_storage is defined */",
        "#define HAVE_STRUCT_SOCKADDR_STORAGE 1",
        "",
        "/* Define to 1 if you have the timeval struct. */",
        "#define HAVE_STRUCT_TIMEVAL 1",
        "",
        "/* Define to 1 if you have the <sys/ioctl.h> header file. */",
        "#define HAVE_SYS_IOCTL_H 1",
        "",
        "/* Define to 1 if you have the <sys/param.h> header file. */",
        "#define HAVE_SYS_PARAM_H 1",
        "",
        "/* Define to 1 if you have the <sys/random.h> header file. */",
        "#define HAVE_SYS_RANDOM_H 1",
        "",
        "/* Define to 1 if you have the <sys/event.h> header file. */",
        "/* #undef HAVE_SYS_EVENT_H */",
        "",
        "/* Define to 1 if you have the <sys/epoll.h> header file. */",
        "#define HAVE_SYS_EPOLL_H 1",
        "",
        "/* Define to 1 if you have the <sys/select.h> header file. */",
        "#define HAVE_SYS_SELECT_H 1",
        "",
        "/* Define to 1 if you have the <sys/socket.h> header file. */",
        "#define HAVE_SYS_SOCKET_H 1",
        "",
        "/* Define to 1 if you have the <sys/stat.h> header file. */",
        "#define HAVE_SYS_STAT_H 1",
        "",
        "/* Define to 1 if you have the <sys/time.h> header file. */",
        "#define HAVE_SYS_TIME_H 1",
        "",
        "/* Define to 1 if you have the <sys/types.h> header file. */",
        "#define HAVE_SYS_TYPES_H 1",
        "",
        "/* Define to 1 if you have the <sys/uio.h> header file. */",
        "#define HAVE_SYS_UIO_H 1",
        "",
        "/* Define to 1 if you have the <time.h> header file. */",
        "#define HAVE_TIME_H 1",
        "",
        "/* Define to 1 if you have the <ifaddrs.h> header file. */",
        "#define HAVE_IFADDRS_H 1",
        "",
        "/* Define to 1 if you have the <unistd.h> header file. */",
        "#define HAVE_UNISTD_H 1",
        "",
        "/* Define to 1 if you have the windows.h header file. */",
        "/* #undef HAVE_WINDOWS_H */",
        "",
        "/* Define to 1 if you have the winsock2.h header file. */",
        "/* #undef HAVE_WINSOCK2_H */",
        "",
        "/* Define to 1 if you have the winsock.h header file. */",
        "/* #undef HAVE_WINSOCK_H */",
        "",
        "/* Define to 1 if you have the mswsock.h header file. */",
        "/* #undef HAVE_MSWSOCK_H */",
        "",
        "/* Define to 1 if you have the winternl.h header file. */",
        "/* #undef HAVE_WINTERNL_H */",
        "",
        "/* Define to 1 if you have the ntstatus.h header file. */",
        "/* #undef HAVE_NTSTATUS_H */",
        "",
        "/* Define to 1 if you have the ntdef.h header file. */",
        "/* #undef HAVE_NTDEF_H */",
        "",
        "/* Define to 1 if you have the writev function. */",
        "#define HAVE_WRITEV 1",
        "",
        "/* Define to 1 if you have the ws2tcpip.h header file. */",
        "/* #undef HAVE_WS2TCPIP_H */",
        "",
        "/* Define to 1 if you have the __system_property_get function */",
        "/* #undef HAVE___SYSTEM_PROPERTY_GET */",
        "",
        "/* Define if have arc4random_buf() */",
        "/* #undef HAVE_ARC4RANDOM_BUF */",
        "",
        "/* Define if have getifaddrs() */",
        "#define HAVE_GETIFADDRS 1",
        "",
        "/* Define if have stat() */",
        "#define HAVE_STAT 1",
        "",
        "/* a suitable file/device to read random data from */",
        "#define CARES_RANDOM_FILE \"/dev/urandom\"",
        "",
        "/* Define to the type qualifier pointed by arg 5 for recvfrom. */",
        "#define RECVFROM_QUAL_ARG5 ",
        "",
        "/* Define to the type of arg 1 for recvfrom. */",
        "#define RECVFROM_TYPE_ARG1 int",
        "",
        "/* Define to the type pointed by arg 2 for recvfrom. */",
        "#define RECVFROM_TYPE_ARG2 void *",
        "",
        "/* Define to 1 if the type pointed by arg 2 for recvfrom is void. */",
        "#define RECVFROM_TYPE_ARG2_IS_VOID 0",
        "",
        "/* Define to the type of arg 3 for recvfrom. */",
        "#define RECVFROM_TYPE_ARG3 size_t",
        "",
        "/* Define to the type of arg 4 for recvfrom. */",
        "#define RECVFROM_TYPE_ARG4 int",
        "",
        "/* Define to the type pointed by arg 5 for recvfrom. */",
        "#define RECVFROM_TYPE_ARG5 struct sockaddr *",
        "",
        "/* Define to 1 if the type pointed by arg 5 for recvfrom is void. */",
        "#define RECVFROM_TYPE_ARG5_IS_VOID 0",
        "",
        "/* Define to the type pointed by arg 6 for recvfrom. */",
        "#define RECVFROM_TYPE_ARG6 socklen_t *",
        "",
        "/* Define to 1 if the type pointed by arg 6 for recvfrom is void. */",
        "#define RECVFROM_TYPE_ARG6_IS_VOID 0",
        "",
        "/* Define to the function return type for recvfrom. */",
        "#define RECVFROM_TYPE_RETV ssize_t",
        "",
        "/* Define to the type of arg 1 for recv. */",
        "#define RECV_TYPE_ARG1 int",
        "",
        "/* Define to the type of arg 2 for recv. */",
        "#define RECV_TYPE_ARG2 void *",
        "",
        "/* Define to the type of arg 3 for recv. */",
        "#define RECV_TYPE_ARG3 size_t",
        "",
        "/* Define to the type of arg 4 for recv. */",
        "#define RECV_TYPE_ARG4 int",
        "",
        "/* Define to the function return type for recv. */",
        "#define RECV_TYPE_RETV ssize_t",
        "",
        "/* Define to the type of arg 1 for send. */",
        "#define SEND_TYPE_ARG1 int",
        "",
        "/* Define to the type of arg 2 for send. */",
        "#define SEND_TYPE_ARG2 const void *",
        "",
        "/* Define to the type of arg 3 for send. */",
        "#define SEND_TYPE_ARG3 size_t",
        "",
        "/* Define to the type of arg 4 for send. */",
        "#define SEND_TYPE_ARG4 int",
        "",
        "/* Define to the function return type for send. */",
        "#define SEND_TYPE_RETV ssize_t",
        "",
        "/* Define to disable non-blocking sockets. */",
        "#undef USE_BLOCKING_SOCKETS",
        "",
        "/* Define to avoid automatic inclusion of winsock.h */",
        "#undef WIN32_LEAN_AND_MEAN",
        "",
        "/* Define to 1 if you have the pthread.h header file. */",
        "#define HAVE_PTHREAD_H 1",
        "",
        "/* Define to 1 if you have the pthread_np.h header file. */",
        "/* #undef HAVE_PTHREAD_NP_H */",
        "",
        "/* Define to 1 if threads are enabled */",
        "#define CARES_THREADS 1",
        "",
        "/* Define to 1 if pthread_init() exists */",
        "/* #undef HAVE_PTHREAD_INIT */",
        "",
        "EOF",
    ]),
)

template_rule(
    name = "ares_config_h",
    src = ":ares_config_h_in",
    out = "src/lib/ares_config.h",
    substitutions = select({
        "@platforms//os:linux": {
        },
        "@platforms//os:osx": {
            "#define GETSERVBYPORT_R_ARGS 6": "#define GETSERVBYPORT_R_ARGS",
            "#define GETSERVBYNAME_R_ARGS 6": "#define GETSERVBYNAME_R_ARGS",
            "/* #undef HAVE_CONNECTX */": "#define HAVE_CONNECTX 1",
            "#define HAVE_PIPE2 1": "/* #undef HAVE_PIPE2 */",
            "/* #undef HAVE_KQUEUE */": "#define HAVE_KQUEUE 1",
            "#define HAVE_EPOLL 1": "/* #undef HAVE_EPOLL */",
            "/* #undef HAVE_GETADDRINFO_THREADSAFE */": "#define HAVE_GETADDRINFO_THREADSAFE 1",
            "#define HAVE_GETRANDOM 1": "/* #undef HAVE_GETRANDOM */",
            "#define HAVE_GETSERVBYPORT_R 1": "/* #undef HAVE_GETSERVBYPORT_R */",
            "#define HAVE_GETSERVBYNAME_R 1": "/* #undef HAVE_GETSERVBYNAME_R */",
            "/* #undef HAVE_LIBRESOLV */": "#define HAVE_LIBRESOLV 1",
            "#define HAVE_MALLOC_H 1": "/* #undef HAVE_MALLOC_H */",
            "/* #undef HAVE_AVAILABILITYMACROS_H */": "#define HAVE_AVAILABILITYMACROS_H 1",
            "/* #undef HAVE_SYS_EVENT_H */": "#define HAVE_SYS_EVENT_H 1",
            "#define HAVE_SYS_EPOLL_H 1": "/* #undef HAVE_SYS_EPOLL_H */",
            "/* #undef HAVE_ARC4RANDOM_BUF */": "#define HAVE_ARC4RANDOM_BUF 1",
            "/* #undef HAVE_INET_NET_PTON */": "#define HAVE_INET_NET_PTON 1",
        },
        "@bazel_template//bazel:cross_compiling_for_windows_gcc": {
            "/* #undef HAVE_WINDOWS_H */": "#define HAVE_WINDOWS_H 1",
            "/* #undef HAVE_WINSOCK2_H */": "#define HAVE_WINSOCK2_H 1",
            "/* #undef HAVE_WINSOCK_H */": "#define HAVE_WINSOCK_H 1",
            "/* #undef HAVE_MSWSOCK_H */": "#define HAVE_MSWSOCK_H 1",
            "/* #undef HAVE_WINTERNL_H */": "#define HAVE_WINTERNL_H 1",
            "/* #undef HAVE_NTSTATUS_H */": "#define HAVE_NTSTATUS_H 1",
            "/* #undef HAVE_NTDEF_H */": "#define HAVE_NTDEF_H 1",
            "/* #undef HAVE_WS2TCPIP_H */": "#define HAVE_WS2TCPIP_H 1",
            "#define GETHOSTNAME_TYPE_ARG2 size_t": "#define GETHOSTNAME_TYPE_ARG2 int",
            "#define HAVE_ARPA_INET_H 1": "/* #undef HAVE_ARPA_INET_H */",
            "#define HAVE_ARPA_NAMESER_COMPAT_H 1": "/* #undef HAVE_ARPA_NAMESER_COMPAT_H */",
            "#define HAVE_ARPA_NAMESER_H 1": "/* #undef HAVE_ARPA_NAMESER_H */",
            "/* #undef HAVE_CLOSESOCKET */": "#define HAVE_CLOSESOCKET 1",
            "#define HAVE_DLFCN_H 1": "/* #undef HAVE_DLFCN_H */",
            "/* #undef HAVE_CXX11 */": "#define HAVE_CXX14 1",
            "#define HAVE_EPOLL 1": "/* #undef HAVE_EPOLL */",
            "#define HAVE_FCNTL 1": "/* #undef HAVE_FCNTL */",
            "#define HAVE_GETIFADDRS 1": "/* #undef HAVE_GETIFADDRS */",
            "#define CARES_SYMBOL_HIDING 1": "/* #undef CARES_SYMBOL_HIDING */",
            "/* #undef HAVE_CONVERTINTERFACEINDEXTOLUID */": "#define HAVE_CONVERTINTERFACEINDEXTOLUID 1",
            "/* #undef HAVE_CONVERTINTERFACELUIDTONAMEA */": "#define HAVE_CONVERTINTERFACELUIDTONAMEA 1",
            "#define HAVE_FCNTL_O_NONBLOCK 1": "/* #undef HAVE_FCNTL_O_NONBLOCK */",
            "#define HAVE_GETRANDOM 1": "/* #undef HAVE_GETRANDOM */",
            "#define HAVE_GETSERVBYPORT_R 1": "/* #undef HAVE_GETSERVBYPORT_R */",
            "#define HAVE_IFADDRS_H 1": "/* #undef HAVE_IFADDRS_H */",
            "#define HAVE_INET_NET_PTON 1": "/* #undef HAVE_INET_NET_PTON */",
            "#define HAVE_IOCTL 1": "/* #undef HAVE_IOCTL */",
            "/* #undef HAVE_IOCTLSOCKET */": "#define HAVE_IOCTLSOCKET 1",
            "/* #undef HAVE_IOCTLSOCKET_FIONBIO */": "#define HAVE_IOCTLSOCKET_FIONBIO 1",
            "#define HAVE_IOCTL_FIONBIO 1": "/* #undef HAVE_IOCTL_FIONBIO */",
            "/* #undef HAVE_IPHLPAPI_H */": "#define HAVE_IPHLPAPI_H 1",
            "#define HAVE_NETDB_H 1": "/* #undef HAVE_NETDB_H */",
            "#define HAVE_NETINET_IN_H 1": "/* #undef HAVE_NETINET_IN_H */",
            "#define HAVE_NETINET_TCP_H 1": "/* #undef HAVE_NETINET_TCP_H */",
            "/* #undef HAVE_NETIOAPI_H */": "#define HAVE_NETIOAPI_H 1",
            "#define HAVE_NET_IF_H 1": "/* #undef HAVE_NET_IF_H */",
            "#define HAVE_PIPE 1": "/* #undef HAVE_PIPE */",
            "#define HAVE_PIPE2 1": "/* #undef HAVE_PIPE2 */",
            "#define HAVE_POLL 1": "/* #undef HAVE_POLL */",
            "#define HAVE_POLL_H 1": "/* #undef HAVE_POLL_H */",
            "#define HAVE_PTHREAD_H 1": "/* #undef HAVE_PTHREAD_H */",
            "#define HAVE_PTHREAD_PRIO_INHERIT 1": "/* #undef HAVE_PTHREAD_PRIO_INHERIT */",
            "/* #undef HAVE_STRICMP */": "#define HAVE_STRICMP 1",
            "/* #undef HAVE_STRNICMP */": "#define HAVE_STRNICMP 1",
            "#define HAVE_SYS_EPOLL_H 1": "/* #undef HAVE_SYS_EPOLL_H */",
            "#define HAVE_SYS_RANDOM_H 1": "/* #undef HAVE_SYS_RANDOM_H */",
            "#define HAVE_SYS_SELECT_H 1": "/* #undef HAVE_SYS_SELECT_H */",
            "#define HAVE_SYS_SOCKET_H 1": "/* #undef HAVE_SYS_SOCKET_H */",
            "#define HAVE_SYS_UIO_H 1": "/* #undef HAVE_SYS_UIO_H */",
            "#define HAVE_USER_NAMESPACE 1": "/* #undef HAVE_USER_NAMESPACE */",
            "#define HAVE_UTS_NAMESPACE 1": "/* #undef HAVE_UTS_NAMESPACE */",
            "#define HAVE_WRITEV 1": "/* #undef HAVE_WRITEV */",
            "/* #undef HAVE_WS2IPDEF_H */": "#define HAVE_WS2IPDEF_H 1",
            "#define RECVFROM_TYPE_ARG1 int": "#define RECVFROM_TYPE_ARG1 SOCKET",
            "#define RECVFROM_TYPE_ARG2 void *": "#define RECVFROM_TYPE_ARG2 char *",
            "#define RECVFROM_TYPE_ARG3 size_t": "#define RECVFROM_TYPE_ARG3 int",
            "#define RECVFROM_TYPE_RETV ssize_t": "#define RECVFROM_TYPE_RETV int",
            "#define RECV_TYPE_ARG1 int": "#define RECV_TYPE_ARG1 SOCKET",
            "#define RECV_TYPE_ARG2 void *": "#define RECV_TYPE_ARG2 char *",
            "#define RECV_TYPE_ARG3 size_t": "#define RECV_TYPE_ARG3 int",
            "#define RECV_TYPE_RETV ssize_t": "#define RECV_TYPE_RETV int",
            "#define SEND_TYPE_ARG1 int": "#define SEND_TYPE_ARG1 SOCKET",
            "#define SEND_TYPE_ARG3 size_t": "#define SEND_TYPE_ARG3 int",
            "#define SEND_TYPE_RETV ssize_t": "#define SEND_TYPE_RETV int",
            "/* #undef _FILE_OFFSET_BITS */": "#define _FILE_OFFSET_BITS 64",
            "#define HAVE_SYS_IOCTL_H 1": "/* #undef HAVE_SYS_IOCTL_H */",
            "#define GETSERVBYPORT_R_ARGS 6": "/* #undef GETSERVBYPORT_R_ARGS */",
        },
        "@bazel_template//bazel:not_cross_compiling_on_windows": {
            "/* #undef GETSERVBYPORT_R_ARGS */": "#define GETSERVBYPORT_R_ARGS",
            "#define GETSERVBYNAME_R_ARGS 6": "#define GETSERVBYNAME_R_ARGS",
            "#define HAVE_CLOCK_GETTIME_MONOTONIC 1": "/* #undef HAVE_CLOCK_GETTIME_MONOTONIC */",
            "/* #undef HAVE_GETADDRINFO_THREADSAFE */": "#define HAVE_GETADDRINFO_THREADSAFE 1",
            "#define HAVE_GETSERVBYNAME_R 1": "/* #undef HAVE_GETSERVBYNAME_R */",
            "#define HAVE_GETTIMEOFDAY 1": "/* #undef HAVE_GETTIMEOFDAY */",
            "#define HAVE_INET_NTOP 1": "/* #undef HAVE_INET_NTOP */",
            "#define HAVE_INET_PTON 1": "/* #undef HAVE_INET_PTON */",
            "#define HAVE_IOCTL_SIOCGIFADDR 1": "/* #undef HAVE_IOCTL_SIOCGIFADDR */",
            "#define HAVE_MSG_NOSIGNAL 1": "/* #undef HAVE_MSG_NOSIGNAL */",
            "#define HAVE_STRCASECMP 1": "/* #undef HAVE_STRCASECMP */",
            "/* #undef HAVE_STRCMPI */": "#define HAVE_STRCMPI 1",
            "#define HAVE_STRINGS_H 1": "/* #undef HAVE_STRINGS_H */",
            "#define HAVE_STRNCASECMP 1": "/* #undef HAVE_STRNCASECMP */",
            "#define HAVE_SYS_PARAM_H 1": "/* #undef HAVE_SYS_PARAM_H */",
            "#define HAVE_NTSTATUS_H 1": "/* #undef HAVE_NTSTATUS_H */",
            "#define HAVE_NTDEF_H 1": "/* #undef HAVE_NTDEF_H */",
            "#define GETHOSTNAME_TYPE_ARG2 size_t": "#define GETHOSTNAME_TYPE_ARG2 int",
            "#define HAVE_ARPA_INET_H 1": "/* #undef HAVE_ARPA_INET_H */",
            "#define HAVE_ARPA_NAMESER_COMPAT_H 1": "/* #undef HAVE_ARPA_NAMESER_COMPAT_H */",
            "#define HAVE_ARPA_NAMESER_H 1": "/* #undef HAVE_ARPA_NAMESER_H */",
            "#define GETSERVBYPORT_R_ARGS 6": "#define GETSERVBYPORT_R_ARGS",
            "/* #undef HAVE_CLOSESOCKET */": "#define HAVE_CLOSESOCKET 1",
            "#define HAVE_DLFCN_H 1": "/* #undef HAVE_DLFCN_H */",
            "#define HAVE_EPOLL 1": "/* #undef HAVE_EPOLL */",
            "#define HAVE_FCNTL 1": "/* #undef HAVE_FCNTL */",
            "#define HAVE_PIPE 1": "/* #undef HAVE_PIPE */",
            "#define HAVE_PIPE2 1": "/* #undef HAVE_PIPE2 */",
            "#define HAVE_POLL 1": "/* #undef HAVE_POLL */",
            "#define HAVE_POLL_H 1": "/* #undef HAVE_POLL_H */",
            "#define HAVE_FCNTL_O_NONBLOCK 1": "/* #undef HAVE_FCNTL_O_NONBLOCK */",
            "#define HAVE_GETRANDOM 1": "/* #undef HAVE_GETRANDOM */",
            "#define HAVE_GETSERVBYPORT_R 1": "/* #undef HAVE_GETSERVBYPORT_R */",
            "/* #undef HAVE_CONVERTINTERFACEINDEXTOLUID */": "#define HAVE_CONVERTINTERFACEINDEXTOLUID 1",
            "/* #undef HAVE_CONVERTINTERFACELUIDTONAMEA */": "#define HAVE_CONVERTINTERFACELUIDTONAMEA 1",
            "#define HAVE_IOCTL 1": "/* #undef HAVE_IOCTL */",
            "/* #undef HAVE_IOCTLSOCKET */": "#define HAVE_IOCTLSOCKET 1",
            "/* #undef HAVE_IOCTLSOCKET_FIONBIO */": "#define HAVE_IOCTLSOCKET_FIONBIO 1",
            "#define HAVE_IOCTL_FIONBIO 1": "/* #undef HAVE_IOCTL_FIONBIO */",
            "/* #undef HAVE_IPHLPAPI_H */": "#define HAVE_IPHLPAPI_H 1",
            "#define HAVE_NETDB_H 1": "/* #undef HAVE_NETDB_H */",
            "/* #undef HAVE_NETIOAPI_H */": "#define HAVE_NETIOAPI_H 1",
            "#define HAVE_NETINET_IN_H 1": "/* #undef HAVE_NETINET_IN_H */",
            "#define HAVE_NETINET_TCP_H 1": "/* #undef HAVE_NETINET_TCP_H */",
            "#define HAVE_NET_IF_H 1": "/* #undef HAVE_NET_IF_H */",
            "#define HAVE_PTHREAD_H 1": "/* #undef HAVE_PTHREAD_H */",
            "#define HAVE_PTHREAD_PRIO_INHERIT 1": "/* #undef HAVE_PTHREAD_PRIO_INHERIT */",
            "/* #undef HAVE_STRICMP */": "#define HAVE_STRICMP 1",
            "/* #undef HAVE_STRNICMP */": "#define HAVE_STRNICMP 1",
            "#define HAVE_SYS_EPOLL_H 1": "/* #undef HAVE_SYS_EPOLL_H */",
            "#define HAVE_SYS_RANDOM_H 1": "/* #undef HAVE_SYS_RANDOM_H */",
            "#define HAVE_SYS_SELECT_H 1": "/* #undef HAVE_SYS_SELECT_H */",
            "#define HAVE_SYS_SOCKET_H 1": "/* #undef HAVE_SYS_SOCKET_H */",
            "#define HAVE_SYS_UIO_H 1": "/* #undef HAVE_SYS_UIO_H */",
            "#define HAVE_SYS_IOCTL_H 1": "/* #undef HAVE_SYS_IOCTL_H */",
            "#define HAVE_SYS_TIME_H 1": "/* #undef HAVE_SYS_TIME_H */",
            "#define RECVFROM_TYPE_ARG1 int": "#define RECVFROM_TYPE_ARG1 SOCKET",
            "#define RECVFROM_TYPE_ARG2 void *": "#define RECVFROM_TYPE_ARG2 char *",
            "#define RECVFROM_TYPE_ARG3 size_t": "#define RECVFROM_TYPE_ARG3 int",
            "#define RECVFROM_TYPE_RETV ssize_t": "#define RECVFROM_TYPE_RETV int",
            "#define RECV_TYPE_ARG1 int": "#define RECV_TYPE_ARG1 SOCKET",
            "#define RECV_TYPE_ARG2 void *": "#define RECV_TYPE_ARG2 char *",
            "#define RECV_TYPE_ARG3 size_t": "#define RECV_TYPE_ARG3 int",
            "#define RECV_TYPE_RETV ssize_t": "#define RECV_TYPE_RETV int",
            "#define SEND_TYPE_ARG1 int": "#define SEND_TYPE_ARG1 SOCKET",
            "#define SEND_TYPE_ARG3 size_t": "#define SEND_TYPE_ARG3 int",
            "#define SEND_TYPE_RETV ssize_t": "#define SEND_TYPE_RETV int",
            "/* #undef _FILE_OFFSET_BITS */": "#define _FILE_OFFSET_BITS 64",
            "#define HAVE_IFADDRS_H 1": "/* #undef HAVE_IFADDRS_H */",
            "#define HAVE_UNISTD_H 1": "/* #undef HAVE_UNISTD_H */",
            "/* #undef HAVE_WINDOWS_H */": "#define HAVE_WINDOWS_H 1",
            "/* #undef HAVE_WINSOCK2_H */": "#define HAVE_WINSOCK2_H 1",
            "/* #undef HAVE_WINSOCK_H */": "#define HAVE_WINSOCK_H 1",
            "/* #undef HAVE_MSWSOCK_H */": "#define HAVE_MSWSOCK_H 1",
            "/* #undef HAVE_WINTERNL_H */": "#define HAVE_WINTERNL_H 1",
            "/* #undef HAVE_WS2TCPIP_H */": "#define HAVE_WS2TCPIP_H 1",
            "#define HAVE_GETIFADDRS 1": "/* #undef HAVE_GETIFADDRS */",
            "#define HAVE_WRITEV 1": "/* #undef HAVE_WRITEV */",
        },
        "//conditions:default": {},
    }),
)

genrule(
    name = "ares_build_h_in",
    outs = ["include/ares_build.h.in"],
    cmd = "\n".join([
        "cat <<'EOF' >$@",
        "#ifndef __CARES_BUILD_H",
        "#define __CARES_BUILD_H",
        "",
        "#define CARES_TYPEOF_ARES_SOCKLEN_T socklen_t",
        "#define CARES_TYPEOF_ARES_SSIZE_T ssize_t",
        "",
        "#define CARES_HAVE_SYS_TYPES_H 1",
        "#define CARES_HAVE_SYS_SOCKET_H 1",
        "#define CARES_HAVE_SYS_SELECT_H 1",
        "/* #undef CARES_HAVE_WINDOWS_H */",
        "/* #undef CARES_HAVE_WS2TCPIP_H */",
        "/* #undef CARES_HAVE_WINSOCK2_H */",
        "#define CARES_HAVE_ARPA_NAMESER_H 1",
        "#define CARES_HAVE_ARPA_NAMESER_COMPAT_H 1",
        "",
        "#ifdef CARES_HAVE_SYS_TYPES_H",
        "#  include <sys/types.h>",
        "#endif",
        "",
        "#ifdef CARES_HAVE_SYS_SOCKET_H",
        "#  include <sys/socket.h>",
        "#endif",
        "",
        "#ifdef CARES_HAVE_SYS_SELECT_H",
        "#  include <sys/select.h>",
        "#endif",
        "",
        "#ifdef CARES_HAVE_WINSOCK2_H",
        "#  include <winsock2.h>",
        "#endif",
        "",
        "#ifdef CARES_HAVE_WS2TCPIP_H",
        "#  include <ws2tcpip.h>",
        "#endif",
        "",
        "#ifdef CARES_HAVE_WINDOWS_H",
        "#  include <windows.h>",
        "#endif",
        "",
        "#endif /* __CARES_BUILD_H */",
        "EOF",
    ]),
)

template_rule(
    name = "ares_build_h",
    src = "include/ares_build.h.in",
    out = "include/ares_build.h",
    substitutions = selects.with_or({
        "@bazel_template//bazel:cross_compiling_for_windows_gcc": {
            "/* #undef CARES_HAVE_WINDOWS_H */": "#define CARES_HAVE_WINDOWS_H 1",
            "/* #undef CARES_HAVE_WS2TCPIP_H */": "#define CARES_HAVE_WS2TCPIP_H 1",
            "/* #undef CARES_HAVE_WINSOCK2_H */": "#define CARES_HAVE_WINSOCK2_H 1",
            "#define CARES_HAVE_SYS_SOCKET_H 1": "/* #undef CARES_HAVE_SYS_SOCKET_H */",
            "#define CARES_HAVE_SYS_SELECT_H 1": "/* #undef CARES_HAVE_SYS_SELECT_H */",
            "#define CARES_HAVE_ARPA_NAMESER_H 1": "/* #undef CARES_HAVE_ARPA_NAMESER_H */",
            "#define CARES_HAVE_ARPA_NAMESER_COMPAT_H 1": "/* #undef CARES_HAVE_ARPA_NAMESER_COMPAT_H */",
        },
        "@bazel_template//bazel:not_cross_compiling_on_windows": {
            "/* #undef CARES_HAVE_WINDOWS_H */": "#define CARES_HAVE_WINDOWS_H 1",
            "/* #undef CARES_HAVE_WS2TCPIP_H */": "#define CARES_HAVE_WS2TCPIP_H 1",
            "/* #undef CARES_HAVE_WINSOCK2_H */": "#define CARES_HAVE_WINSOCK2_H 1",
            "#define CARES_HAVE_SYS_SOCKET_H 1": "/* #undef CARES_HAVE_SYS_SOCKET_H */",
            "#define CARES_HAVE_SYS_SELECT_H 1": "/* #undef CARES_HAVE_SYS_SELECT_H */",
            "#define CARES_HAVE_ARPA_NAMESER_H 1": "/* #undef CARES_HAVE_ARPA_NAMESER_H */",
            "#define CARES_HAVE_ARPA_NAMESER_COMPAT_H 1": "/* #undef CARES_HAVE_ARPA_NAMESER_COMPAT_H */",
            "#define CARES_TYPEOF_ARES_SSIZE_T ssize_t": "#define CARES_TYPEOF_ARES_SSIZE_T __int64",
        },
        "//conditions:default": {},
    }),
)
