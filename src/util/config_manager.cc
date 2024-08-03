/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "src/util/config_manager.h"

namespace bazel_template {
namespace util {

static folly::Singleton<ConfigManager> config_manager;

bool ConfigManager::Init(const std::string& base_config_path) {
  std::string content = Util::LoadContent(base_config_path);
  if (!Util::JsonToMessage(content, &base_config_)) {
    LOG(ERROR) << "parse base config error, path: " << base_config_path
               << ", content: " << content;
    return false;
  }
  return true;
}

uint32_t ConfigManager::GrpcServerPort() {
  return base_config_.grpc_server_port();
}

uint32_t ConfigManager::HttpServerPort() {
  return base_config_.http_server_port();
}

std::string ConfigManager::ServerAddr() { return base_config_.server_addr(); }

}  // namespace util
}  // namespace bazel_template
