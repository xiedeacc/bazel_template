/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include <csignal>

// #include "gperftools/profiler.h"

#include "folly/init/Init.h"
#include "gflags/gflags.h"
#include "src/common/logging.h"
#include "src/server/http_server_impl.h"
#include "src/server/server_context.h"
#include "src/util/config_manager.h"

// https://github.com/grpc/grpc/issues/24884
bazel_template::server::HttpServer* http_server_ptr = nullptr;
bool shutdown_required = false;
std::mutex mutex;
std::condition_variable cv;

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

void ShutdownCheckingThread() {
  std::unique_lock<std::mutex> lock(mutex);
  cv.wait(lock, []() { return shutdown_required; });
  http_server_ptr->Shutdown();
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
  // ProfilerStart("bazel_template_profile");
  LOG(INFO) << "Server initializing ...";

  gflags::ParseCommandLineFlags(&argc, &argv, false);

  folly::Init init(&argc, &argv, false);
  bazel_template::logging::Initialize(argv[0], "./log");
  LOG(INFO) << "CommandLine: "
            << bazel_template::logging::CommandLine(argc, argv);

  bazel_template::util::ConfigManager::Instance()->Init(
      "./conf/server_config.json");

  RegisterSignalHandler();

  std::thread shutdown_thread(ShutdownCheckingThread);

  std::shared_ptr<bazel_template::server::ServerContext> server_context =
      std::make_shared<bazel_template::server::ServerContext>();

  bazel_template::server::HttpServer http_server(server_context);

  ::http_server_ptr = &http_server;

  http_server.Start();

  LOG(INFO) << "Now stopped http server";

  if (shutdown_thread.joinable()) {
    shutdown_thread.join();
  }

  // ProfilerStop();
  return 0;
}
