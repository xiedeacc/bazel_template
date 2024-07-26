/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef BAZEL_TEMPLATE_UTIL_CONFIG_MANAGER_H
#define BAZEL_TEMPLATE_UTIL_CONFIG_MANAGER_H

#include <string>

#include "folly/Singleton.h"
#include "folly/init/Init.h"
#include "src/proto/config.pb.h"

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
  uint32_t MetricRatio() { return base_config_.metric_ratio(); };
  uint32_t MetricIntervalSec() { return base_config_.metric_interval_sec(); }
  uint32_t DiscardRatio() { return base_config_.discard_ratio(); }
  uint32_t ThreadPoolSize() { return base_config_.thread_pool_size(); }

 private:
  bazel_template::proto::BaseConfig base_config_;
};

static folly::Singleton<ConfigManager> config_manager;

}  // namespace util
}  // namespace bazel_template

#endif /* BAZEL_TEMPLATE_UTIL_CONFIG_MANAGER_H */
