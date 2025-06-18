/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

// #include "gperftools/profiler.h"

#include "folly/init/Init.h"
#include "gflags/gflags.h"
#include "glog/logging.h"
#include "src/client/websocket_client.h"
#include "src/util/config_manager.h"

bool shutdown_required = false;
std::mutex mutex;
std::condition_variable cv;
bazel_template::client::WebSocketClient *websocket_client_ptr = nullptr;

void SignalHandler(int sig) {
  LOG(INFO) << "Got signal: " << strsignal(sig) << std::endl;
  shutdown_required = true;
  cv.notify_all();
}

void ShutdownCheckingThread(void) {
  std::unique_lock<std::mutex> lock(mutex);
  cv.wait(lock, []() { return shutdown_required; });
  websocket_client_ptr->Stop();
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
}
