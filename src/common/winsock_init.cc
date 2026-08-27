/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

// Winsock must be initialized with WSAStartup() before any socket call. On
// Windows some libraries construct namespace-scope objects that resolve an
// address in their constructor -- proxygen's
// HTTPSessionAcceptor::unknownSocketAddress_ ("0.0.0.0") is one. Static
// initialization order across translation units is unspecified, so such an
// object can run before folly's own Winsock initializer and fail with
// WSANOTINITIALISED (10093), which terminates the process before main().
//
// C initializers (.CRT$XI*) are guaranteed to run before C++ static
// initializers (.CRT$XC*), so registering WSAStartup there makes Winsock ready
// for every static object regardless of link order.
//
// This file is a no-op on non-Windows platforms.

#if defined(_WIN32)

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

namespace {

int InitWinsock() {
  WSADATA wsa_data;
  // Failures are ignored on purpose: a later socket call reports the real
  // error with far more context than we could here, before main() runs.
  WSAStartup(MAKEWORD(2, 2), &wsa_data);
  return 0;
}

}  // namespace

#pragma section(".CRT$XIU", long, read)
extern "C" __declspec(allocate(".CRT$XIU")) int (
    *bazel_template_winsock_init_)() = InitWinsock;

#endif  // defined(_WIN32)
