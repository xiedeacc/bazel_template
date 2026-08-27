/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

// #include "gperftools/profiler.h"

#include <signal.h>

#include <string>

#include "folly/init/Init.h"
#include "gflags/gflags.h"
#include "src/client/websocket_client.h"
#include "src/common/logging.h"
#include "src/util/config_manager.h"

bool shutdown_required = false;
std::mutex mutex;
std::condition_variable cv;
bazel_template::client::WebSocketClient* websocket_client_ptr = nullptr;

const char* SignalName(int sig);

void SignalHandler(int sig) {
  LOG(INFO) << "Got signal: " << SignalName(sig) << " (" << sig << ")" << '\n';
  shutdown_required = true;
  cv.notify_all();
}

const char* SignalName(int sig) {
#if defined(_WIN32)
  (void)sig;
  return "signal";
#else
  return strsignal(sig);
#endif
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
  cv.wait(lock, []() { return shutdown_required; });
  websocket_client_ptr->Stop();
}

void RegisterSignalHandler() {
  signal(SIGTERM, &SignalHandler);
  signal(SIGINT, &SignalHandler);
#if !defined(_WIN32)
  signal(SIGQUIT, &SignalHandler);
  signal(SIGHUP, SIG_IGN);
  signal(SIGPIPE, SIG_IGN);
#endif
}

int main(int argc, char** argv) {
  // An exception escaping main() calls std::terminate before anything gets
  // logged, which makes a startup failure look like a silent crash.
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
    LOG(ERROR) << "Client failed: " << e.what();
    return 1;
  } catch (...) {
    LOG(ERROR) << "Client failed with an unknown exception";
    return 1;
  }
}
