/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include <chrono>
#include <filesystem>
#include <fstream>
#include <string>

#include "glog/logging.h"
#include "gtest/gtest.h"

import bazel_template.util.config_manager;

namespace bazel_template::util {
namespace {

// Written by the test itself: on Windows there is no runfiles tree to read
// conf/server_config.json from.
std::filesystem::path WriteTempConfig() {
  const auto stamp =
      std::chrono::steady_clock::now().time_since_epoch().count();
  const auto path =
      std::filesystem::temp_directory_path() /
      ("bazel_template_config_" + std::to_string(stamp) + ".json");
  std::ofstream config(path, std::ios::binary);
  config << R"({
  "server_addr": "127.0.0.1",
  "grpc_server_port": 10001,
  "http_server_port": 10003,
  "grpc_threads": 3,
  "event_threads": 5,
  "client_worker_thread_pool_size": 2
})";
  return path;
}

TEST(ConfigManager, Init) {
  const auto config_path = WriteTempConfig();
  auto config_manager = ConfigManager::Instance();
  ASSERT_TRUE(config_manager->Init(config_path.string()));
  std::error_code error;
  std::filesystem::remove(config_path, error);

  EXPECT_EQ(config_manager->ServerAddr(), "127.0.0.1");
  EXPECT_EQ(config_manager->GrpcServerPort(), 10001U);
  EXPECT_EQ(config_manager->HttpServerPort(), 10003U);
  EXPECT_EQ(config_manager->GrpcThreads(), 3U);
  EXPECT_EQ(config_manager->EventThreads(), 5U);
  EXPECT_EQ(config_manager->ClientWorkerThreadPoolSize(), 2U);
  LOG(INFO) << config_manager->ToString();
}

TEST(ConfigManager, InitRejectsMissingFile) {
  EXPECT_FALSE(ConfigManager::Instance()->Init("./does-not-exist.json"));
}

}  // namespace
}  // namespace bazel_template::util
