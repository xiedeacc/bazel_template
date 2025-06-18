/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef BAZEL_TEMPLATE_UTIL_CONFIG_MANAGER_H_
#define BAZEL_TEMPLATE_UTIL_CONFIG_MANAGER_H_

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
    std::string content;
    Util::LoadSmallFile(base_config_path, &content);
    if (!Util::JsonToMessage(content, &base_config_)) {
      LOG(ERROR) << "Parse base config error, path: " << base_config_path
                 << ", content: " << content;
      return false;
    }
    LOG(INFO) << "Base config: " << ToString();
    return true;
  }

  std::string ServerAddr() { return base_config_.server_addr(); }
  uint32_t HttpServerPort() { return base_config_.http_server_port(); }
  uint32_t ClientWorkerThreadPoolSize() {
    return base_config_.client_worker_thread_pool_size();
  }

  std::string ToString() {
    std::string json;
    Util::MessageToJson(base_config_, &json);
    return json;
  }

 private:
  bazel_template::proto::BaseConfig base_config_;
};

}  // namespace util
}  // namespace bazel_template

#endif  // BAZEL_TEMPLATE_UTIL_CONFIG_MANAGER_H_
