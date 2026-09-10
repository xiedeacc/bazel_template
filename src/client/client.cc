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
#include "glog/logging.h"

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
    FLAGS_log_dir = "./log";
    FLAGS_stop_logging_if_full_disk = true;
    FLAGS_logbufsecs = 0;

    folly::Init init(&argc, &argv, false);
    google::EnableLogCleaner(7);
    // google::InitGoogleLogging(argv[0]); // already called in folly::Init
    google::SetStderrLogging(google::GLOG_INFO);
    LOG(INFO) << "CommandLine: " << google::GetArgv();

    bazel_template::util::ConfigManager::Instance()->Init(
        "./conf/client_config.json");

    RegisterSignalHandler();

    std::thread shutdown_thread(ShutdownCheckingThread);

    auto server_addr =
        bazel_template::util::ConfigManager::Instance()->ServerAddr();
    auto http_port =
        bazel_template::util::ConfigManager::Instance()->HttpServerPort();
    bazel_template::client::WebSocketClient websocket_client(
        server_addr, std::to_string(http_port));
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
