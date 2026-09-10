/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include <atomic>
#include <condition_variable>
#include <csignal>
#include <cstring>
#include <exception>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

#include "folly/init/Init.h"
#include "gflags/gflags.h"
#include "glog/logging.h"
// MSVC needs grpc included wherever a module reaching grpc is imported: the
// BMI carries grpc's static inline helpers as declarations only.
#include "grpc++/grpc++.h"

import bazel_template.server.context;
import bazel_template.server.grpc_server;
import bazel_template.server.http_server;
import bazel_template.util.config_manager;

// A signal handler may only touch objects with static storage duration.
// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
bazel_template::server::HttpServer* http_server_ptr = nullptr;
bazel_template::server::GrpcServer* grpc_server_ptr = nullptr;
std::atomic_bool shutdown_required{false};
std::mutex mutex;
std::condition_variable cv;
// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

namespace {

const char* SignalName(int sig) {
#ifdef _WIN32
  (void)sig;
  return "signal";
#else
  return strsignal(sig);
#endif
}

void SignalHandler(int sig) {
  LOG(INFO) << "Got signal: " << SignalName(sig) << " (" << sig << ")";
  shutdown_required.store(true);
  cv.notify_all();
}

void ShutdownCheckingThread() {
  std::unique_lock<std::mutex> lock(mutex);
  cv.wait(lock, []() { return shutdown_required.load(); });
  if (grpc_server_ptr != nullptr) {
    grpc_server_ptr->Shutdown();
  }
  if (http_server_ptr != nullptr) {
    http_server_ptr->Shutdown();
  }
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

}  // namespace

namespace {

// noexcept: reporting from a catch handler must not throw again, which is
// what keeps main() exception-safe.
void ReportFailure(const char* what) noexcept {
  try {
    LOG(ERROR) << "Server failed" << (what == nullptr ? "" : ": ")
               << (what == nullptr ? "" : what);
  } catch (...) {  // NOLINT(bugprone-empty-catch)
  }
}

}  // namespace

int main(int argc, char** argv) {
  try {
    LOG(INFO) << "Server initializing ...";
    gflags::ParseCommandLineFlags(&argc, &argv, false);
    FLAGS_log_dir = "./log";
    FLAGS_stop_logging_if_full_disk = true;
    FLAGS_logbufsecs = 0;
    folly::Init init(&argc, &argv, false);
    google::EnableLogCleaner(7);
    google::SetStderrLogging(google::GLOG_INFO);
    LOG(INFO) << "CommandLine: " << google::GetArgv();

    if (!bazel_template::util::ConfigManager::Instance()->Init(
            "./conf/server_config.json")) {
      LOG(ERROR) << "Cannot load ./conf/server_config.json";
      return 1;
    }

    RegisterSignalHandler();
    std::jthread shutdown_thread(ShutdownCheckingThread);

    auto server_context =
        std::make_shared<bazel_template::server::ServerContext>();

    bazel_template::server::GrpcServer grpc_server(server_context);
    ::grpc_server_ptr = &grpc_server;
    grpc_server.Start();

    bazel_template::server::HttpServer http_server(server_context);
    ::http_server_ptr = &http_server;
    http_server.Start();

    LOG(INFO) << "All servers running. Waiting for shutdown signal...";
    if (shutdown_thread.joinable()) {
      shutdown_thread.join();
    }
    LOG(INFO) << "Server shutdown complete";
    return 0;
  } catch (const std::exception& e) {
    ReportFailure(e.what());
    return 1;
  } catch (...) {
    ReportFailure(nullptr);
    return 1;
  }
}
