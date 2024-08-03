/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef BAZEL_TEMPLATE_UTIL_CONFIG_MANAGER_H
#define BAZEL_TEMPLATE_UTIL_CONFIG_MANAGER_H

#include <memory>
#include <string>

#include "folly/Singleton.h"
#include "src/proto/config.pb.h"
#include "src/util/util.h"

namespace bazel_template {
namespace util {

class ConfigManager {
  friend class folly::Singleton<ConfigManager>;

 private:
  ConfigManager() {}

 public:
  static std::shared_ptr<ConfigManager> Instance() {
    return folly::Singleton<ConfigManager>::try_get();
  }

  bool Init(const std::string& base_config_path);
  std::string ServerAddr();
  uint32_t GrpcServerPort();
  uint32_t HttpServerPort();
  uint32_t MetricRatio() { return base_config_.metric_ratio(); }
  uint32_t MetricIntervalSec() { return base_config_.metric_interval_sec(); }
  uint32_t DiscardRatio() { return base_config_.discard_ratio(); }
  uint32_t ThreadPoolSize() { return base_config_.thread_pool_size(); }

  std::string ToString() {
    std::string json;
    Util::PrintProtoMessage(base_config_, &json);
    LOG(INFO) << "XXX: " << json;
    return json;
  }

 private:
  bazel_template::proto::BaseConfig base_config_;
};

}  // namespace util
}  // namespace bazel_template

#endif  // BAZEL_TEMPLATE_UTIL_CONFIG_MANAGER_H
