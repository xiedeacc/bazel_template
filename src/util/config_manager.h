/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef BAZEL_TEMPLATE_UTIL_CONFIG_MANAGER_H
#define BAZEL_TEMPLATE_UTIL_CONFIG_MANAGER_H

#include <memory>
#include <string>

#include "folly/Singleton.h"
#include "glog/logging.h"
#include "src/proto/config.pb.h"
#include "src/util/util.h"

namespace bazel_template {
namespace util {

class ConfigManager {
 private:
  friend class folly::Singleton<ConfigManager>;
  ConfigManager() = default;

 public:
  static std::shared_ptr<ConfigManager> Instance();

  bool Init(const std::string& base_config_path) {
    std::string content = Util::LoadContent(base_config_path);
    if (!Util::JsonToMessage(content, &base_config_)) {
      LOG(ERROR) << "parse base config error, path: " << base_config_path
                 << ", content: " << content;
      return false;
    }
    LOG(INFO) << "base config: " << ToString();
    return true;
  }

  std::string ServerAddr() { return base_config_.server_addr(); }
  uint32_t GrpcServerPort() { return base_config_.grpc_server_port(); }
  uint32_t HttpServerPort() { return base_config_.http_server_port(); }
  uint32_t MetricRatio() { return base_config_.metric_ratio(); }
  uint32_t MetricIntervalSec() { return base_config_.metric_interval_sec(); }
  uint32_t DiscardRatio() { return base_config_.discard_ratio(); }
  uint32_t GrpcThreads() { return base_config_.grpc_threads(); }
  uint32_t EventThreads() { return base_config_.event_threads(); }

  std::string ToString() {
    std::string json;
    Util::PrintProtoMessage(base_config_, &json);
    return json;
  }

 private:
  bazel_template::proto::BaseConfig base_config_;
};

}  // namespace util
}  // namespace bazel_template

#endif  // BAZEL_TEMPLATE_UTIL_CONFIG_MANAGER_H
