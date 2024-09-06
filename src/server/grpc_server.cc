/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "folly/init/Init.h"
#include "gflags/gflags.h"
#include "glog/logging.h"
// #include "gperftools/profiler.h"
#include "src/common/module.h"
#include "src/server/grpc_server_impl.h"
#include "src/util/config_manager.h"

int main(int argc, char **argv) {
  // ProfilerStart("bazel_template_profile");

  folly::Init init(&argc, &argv, false);
  // google::InitGoogleLogging(argv[0]); // called in folly::Init
  google::SetStderrLogging(google::GLOG_INFO);
  gflags::ParseCommandLineFlags(&argc, &argv, false);

  LOG(INFO) << "Program initializing ...";
  LOG(INFO) << "CommandLine: " << google::GetArgv();

  bazel_template::common::InitAllModules(&argc, &argv);

  bazel_template::util::ConfigManager::Instance()->Init(
      "./conf/base_config.json");

  bazel_template::server::GrpcServer server(
      bazel_template::util::ConfigManager::Instance()->GrpcServerPort());
  // server.WaitForShutdown();

  // ProfilerStop();
  return 0;
}
