/*******************************************************************************
 * Copyright (c) 2023  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

// #include "gperftools/profiler.h"

#include <signal.h>

#include "gflags/gflags.h"
#include "src/server/service_impl.h"

using bazel_template::server::ServiceImpl;
using bazel_template::util::ConfigUtil;

void RegisterSignalHandler() {
  signal(SIGTERM, &ServiceImpl::SignalHandler);
  signal(SIGINT, &ServiceImpl::SignalHandler);
  signal(SIGQUIT, &ServiceImpl::SignalHandler);
  signal(SIGKILL, &ServiceImpl::SignalHandler);
  signal(SIGHUP, SIG_IGN);
  signal(SIGPIPE, SIG_IGN);
}

void CoreWriter(const char *data, int size) { LOG(ERROR) << data; }

// std::string GetGitVer() {
//#ifdef REV
//#define _GIT_REV(y) (#y)
//#define GIT_REV(y) _GIT_REV(y)
// std::string str_ver = GIT_REV(REV);
//#undef GIT_REV
//#undef _GIT_REV
//#endif
// return str_ver;
//}

int main(int argc, char **argv) {
  // ProfilerStart("bazel_template_profile");
  LOG(INFO) << "Start bazel_template server...";
  google::ParseCommandLineFlags(&argc, &argv, true);
  FLAGS_log_dir = "./log";
  FLAGS_stop_logging_if_full_disk = true;
  google::InitGoogleLogging(argv[0]);
  google::EnableLogCleaner(7);
  RegisterSignalHandler();
  // google::InstallFailureSignalHandler();      // 捕捉信号
  // google::InstallFailureWriter(&CoreWriter);  // 打印错误信息

  // LOG(INFO) << GetGitVer();

  if (!ConfigUtil::Instance().Init("./conf/base_config.json")) {
    return -1;
  }

  ServiceImpl service_impl;
  if (!service_impl.Init()) {
    LOG(ERROR) << "ServiceImpl init failed, now exist...";
    return -1;
  }
  service_impl.Run();
  // ProfilerStop();
  return 0;
}
