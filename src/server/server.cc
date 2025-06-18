/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include <signal.h>

// #include "gperftools/profiler.h"

#include "folly/init/Init.h"
#include "gflags/gflags.h"
#include "glog/logging.h"
#include "src/server/http_server_impl.h"
#include "src/server/server_context.h"
#include "src/util/config_manager.h"

// https://github.com/grpc/grpc/issues/24884
bazel_template::server::HttpServer *http_server_ptr = nullptr;
bool shutdown_required = false;
std::mutex mutex;
std::condition_variable cv;

void SignalHandler(int sig) {
  LOG(INFO) << "Got signal: " << strsignal(sig) << std::endl;
  shutdown_required = true;
  cv.notify_all();
}

void ShutdownCheckingThread(void) {
  std::unique_lock<std::mutex> lock(mutex);
  cv.wait(lock, []() { return shutdown_required; });
  http_server_ptr->Shutdown();
}

void RegisterSignalHandler() {
  signal(SIGTERM, &SignalHandler);
  signal(SIGINT, &SignalHandler);
  signal(SIGQUIT, &SignalHandler);
  signal(SIGHUP, SIG_IGN);
  signal(SIGPIPE, SIG_IGN);
}

int main(int argc, char **argv) {
  // ProfilerStart("bazel_template_profile");
  LOG(INFO) << "Server initializing ...";

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
