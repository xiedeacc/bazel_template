/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

module;

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>

#include "glog/logging.h"
#include "src/proto/config.pb.h"

module bazel_template.util.config_manager;

import bazel_template.util;

namespace bazel_template::util {

ConfigManager::ConfigManager()
    : base_config_(std::make_unique<proto::BaseConfig>()) {}

ConfigManager::~ConfigManager() = default;

std::shared_ptr<ConfigManager> ConfigManager::Instance() {
  static std::shared_ptr<ConfigManager> instance(new ConfigManager());
  return instance;
}

bool ConfigManager::Init(std::string_view base_config_path) {
  const std::string path(base_config_path);
  std::string content;
  if (!Util::LoadSmallFile(path, &content)) {
    LOG(ERROR) << "Cannot read base config, path: " << path;
    return false;
  }
  if (!Util::JsonToMessage(content, base_config_.get())) {
    LOG(ERROR) << "Parse base config error, path: " << path
               << ", content: " << content;
    return false;
  }
  LOG(INFO) << "Base config: " << ToString();
  return true;
}

std::string ConfigManager::ServerAddr() const {
  return base_config_->server_addr();
}

uint32_t ConfigManager::HttpServerPort() const {
  return base_config_->http_server_port();
}

uint32_t ConfigManager::GrpcServerPort() const {
  return base_config_->grpc_server_port();
}

uint32_t ConfigManager::GrpcThreads() const {
  const uint32_t threads = base_config_->grpc_threads();
  return threads > 0 ? threads : kDefaultGrpcThreads;
}

uint32_t ConfigManager::EventThreads() const {
  const uint32_t threads = base_config_->event_threads();
  return threads > 0 ? threads : kDefaultEventThreads;
}

uint32_t ConfigManager::ClientWorkerThreadPoolSize() const {
  return base_config_->client_worker_thread_pool_size();
}

std::string ConfigManager::ToString() const {
  std::string json;
  Util::MessageToJson(*base_config_, &json);
  return json;
}

}  // namespace bazel_template::util
