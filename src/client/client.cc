/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

// #include "gperftools/profiler.h"

#include <atomic>
#include <condition_variable>
#include <csignal>
#include <cstring>
#include <exception>
#include <mutex>
#include <string>
#include <thread>

#include "folly/init/Init.h"
#include "gflags/gflags.h"
#include "src/common/logging.h"

import bazel_template.client.websocket_client;
import bazel_template.util.config_manager;

// A signal handler may only touch objects with static storage duration.
// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
std::atomic_bool shutdown_required{false};
std::mutex mutex;
std::condition_variable cv;
bazel_template::client::WebSocketClient* websocket_client_ptr = nullptr;
// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

void SignalHandler(int sig) {
#ifdef _WIN32
  LOG(INFO) << "Got signal: " << sig;
#else
  LOG(INFO) << "Got signal: " << strsignal(sig);
#endif
  shutdown_required.store(true);
  cv.notify_all();
}

// server_addr in the config is a *bind* address. Wildcards are not valid
// connect targets: Windows rejects them with WSAEADDRNOTAVAIL(10049), while
// Linux quietly treats them as loopback. Normalize so the client behaves the
// same everywhere.
std::string ConnectTargetHost(const std::string& bind_addr) {
  if (bind_addr.empty() || bind_addr == "0.0.0.0") {
    return "127.0.0.1";
  }
  if (bind_addr == "::" || bind_addr == "[::]") {
    return "::1";
  }
  return bind_addr;
}

void ShutdownCheckingThread() {
  std::unique_lock<std::mutex> lock(mutex);
  cv.wait(lock, []() { return shutdown_required.load(); });
  websocket_client_ptr->Stop();
}

void RegisterSignalHandler() {
  (void)signal(SIGTERM, &SignalHandler);
  (void)signal(SIGINT, &SignalHandler);
#ifndef _WIN32
  (void)signal(SIGQUIT, &SignalHandler);
  (void)signal(SIGHUP, SIG_IGN);
  (void)signal(SIGPIPE, SIG_IGN);
#endif
}

namespace {

// noexcept: reporting from a catch handler must not throw again, which is
// what keeps main() exception-safe.
void ReportFailure(const char* what) noexcept {
  try {
    LOG(ERROR) << "Client failed" << (what == nullptr ? "" : ": ")
               << (what == nullptr ? "" : what);
  } catch (...) {  // NOLINT(bugprone-empty-catch)
  }
}

}  // namespace

int main(int argc, char** argv) {
  try {
    // ProfilerStart("bazel_template_profile");
    LOG(INFO) << "Client initializing ...";

    gflags::ParseCommandLineFlags(&argc, &argv, false);

    folly::Init init(&argc, &argv, false);
    bazel_template::logging::Initialize(argv[0], "./log");
    LOG(INFO) << "CommandLine: "
              << bazel_template::logging::CommandLine(argc, argv);

    bazel_template::util::ConfigManager::Instance()->Init(
        "./conf/client_config.json");

    RegisterSignalHandler();

    std::thread shutdown_thread(ShutdownCheckingThread);

    auto server_addr =
        bazel_template::util::ConfigManager::Instance()->ServerAddr();
    auto http_port =
        bazel_template::util::ConfigManager::Instance()->HttpServerPort();
    bazel_template::client::WebSocketClient websocket_client(
        ConnectTargetHost(server_addr), std::to_string(http_port));
    websocket_client.Connect();

    websocket_client_ptr = &websocket_client;

    LOG(INFO) << "Now stopped websocket client";

    if (shutdown_thread.joinable()) {
      shutdown_thread.join();
    }
    // ProfilerStop();
    return 0;
  } catch (const std::exception& e) {
    ReportFailure(e.what());
    return 1;
  } catch (...) {
    ReportFailure(nullptr);
    return 1;
  }
}
