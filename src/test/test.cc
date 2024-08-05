/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "folly/init/Init.h"
#include "gflags/gflags.h"
#include "glog/logging.h"
#include "src/util/config_manager.h"

using bazel_template::util::ConfigManager;

int main(int argc, char** argv) {
  char* arr = new char[8];
  LOG(INFO) << "TEST";
  strcpy(arr, "test");
  LOG(INFO) << arr;

  folly::Init init(&argc, &argv, false);
  google::SetStderrLogging(google::INFO);
  google::ParseCommandLineFlags(&argc, &argv, false);
  auto ret = ConfigManager::Instance()->Init("./conf/base_config.json");
  LOG(INFO) << ConfigManager::Instance()->ToString();
}
