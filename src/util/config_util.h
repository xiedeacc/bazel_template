/*******************************************************************************
 * Copyright (c) 2023  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/
#pragma once

#include <exception>
#include <string>

#include "Nacos.h"
#include "absl/base/internal/spinlock.h"
#include "factory/NacosServiceFactory.h"
#include "src/config/base_config.h"
#include "src/config/config_wrapper.h"
#include "src/proto/config.pb.h"
#include "src/util/singleton.h"
#include "src/util/util.h"

namespace bazel_template {
namespace util {

class ConfigUtil : public Singleton<ConfigUtil> {
  friend class SingletonBase<ConfigUtil>;

 private:
  ConfigUtil() {}

  virtual ~ConfigUtil() {}

 public:
  bool Init(const std::string &base_config_path);

  config::BaseConfigPtr BaseConfig() {
    if (base_config_ != nullptr) {
      return base_config_;
    }

    config::BaseConfigPtr base_config = std::make_shared<config::BaseConfig>();
    if (!base_config->Make(Util::LoadContent(base_config_path_))) {
      LOG(ERROR) << "parse base config error, path: " << base_config_path_;
      return nullptr;
    }

    if (base_config == nullptr) {
      LOG(ERROR) << "base config nullptr";
      return base_config;
    }
    base_config_ = base_config;
    return base_config_;
  }

  bool GetNacosContent(const std::string &key, std::string *content) {
    try {
      auto base_config_ = BaseConfig();
      if (base_config_.get() == nullptr) {
        LOG(ERROR) << "Fail to get bazel_template config";
        return false;
      }
      *content = nacos_config_service_->getConfig(
          key, base_config_->config.nacos_group_name(), 1000);
    } catch (nacos::NacosException &e) {
      LOG(ERROR) << "Get content from nacos failed with curl code:"
                 << e.errorcode() << " Reason:" << e.what();
      return false;
    }
    return true;
  }

  bool GetNacosContent(const std::string &key, const std::string &group_name,
                       std::string *content) {
    try {
      *content = nacos_config_service_->getConfig(key, group_name, 1000);
    } catch (nacos::NacosException &e) {
      LOG(ERROR) << "Get content from nacos failed with curl code:"
                 << e.errorcode() << " Reason:" << e.what();
      return false;
    }
    return true;
  }

  bool GetNacosHistoryContent(const std::string &group, const std::string &key,
                              std::string *content) {
    try {
      *content = nacos_config_service_->getConfigHistory(key, group, 1000);
    } catch (nacos::NacosException &e) {
      LOG(ERROR) << "Get content from nacos failed with curl code:"
                 << e.errorcode() << " Reason:" << e.what();
      return false;
    }
    return true;
  }

  bool GetNacosHistoryDetailContent(const std::string &group,
                                    const std::string &key,
                                    const std::string &nid,
                                    std::string *content) {
    try {
      *content =
          nacos_config_service_->getConfigHistoryDetail(key, group, nid, 1000);
    } catch (nacos::NacosException &e) {
      LOG(ERROR) << "Get content from nacos failed with curl code:"
                 << e.errorcode() << " Reason:" << e.what();
      return false;
    }
    return true;
  }

  std::string GetContent(const std::string &key);
  std::string GetContent(const std::string &key, const std::string &group_name);
  void UpdateConfigCacheContent(const std::string &key,
                                const std::string &content);

  int BrpcServerPort() {
    {
      absl::base_internal::SpinLockHolder locker(&lock_);
      if (cached_content_.find("brpc_server_port") != cached_content_.end()) {
        return std::stoi(cached_content_["brpc_server_port"]);
      }
    }
    std::string content;
    if (GetNacosContent("brpc_server_port", &content)) {
      UpdateConfigCacheContent("brpc_server_port", content);
      return std::stoi(cached_content_["brpc_server_port"]);
    }
    return 10005;
  }

  int BrpcHttpServerPort() {
    {
      absl::base_internal::SpinLockHolder locker(&lock_);
      if (cached_content_.find("brpc_http_server_port") !=
          cached_content_.end()) {
        return std::stoi(cached_content_["brpc_http_server_port"]);
      }
    }
    std::string content;
    if (GetNacosContent("brpc_http_server_port", &content)) {
      UpdateConfigCacheContent("brpc_http_server_port", content);
      return std::stoi(cached_content_["brpc_http_server_port"]);
    }
    return 10006;
  }

  int BrpcConfigServerPort() {
    {
      absl::base_internal::SpinLockHolder locker(&lock_);
      if (cached_content_.find("brpc_config_server_port") !=
          cached_content_.end()) {
        return std::stoi(cached_content_["brpc_config_server_port"]);
      }
    }
    std::string content;
    if (GetNacosContent("brpc_config_server_port", &content)) {
      UpdateConfigCacheContent("brpc_config_server_port", content);
      return std::stoi(cached_content_["brpc_config_server_port"]);
    }
    return 10007;
  }

  int BrpcNumThreads() {
    {
      absl::base_internal::SpinLockHolder locker(&lock_);
      if (cached_content_.find("brpc_num_threads") != cached_content_.end()) {
        return std::stoi(cached_content_["brpc_num_threads"]);
      }
    }
    std::string content;
    if (GetNacosContent("brpc_num_threads", &content)) {
      UpdateConfigCacheContent("brpc_num_threads", content);
      return std::stoi(cached_content_["brpc_num_threads"]);
    }
    return 64;
  }

  int BrpcMaxConcurrency() {
    {
      absl::base_internal::SpinLockHolder locker(&lock_);
      if (cached_content_.find("brpc_max_concurrency") !=
          cached_content_.end()) {
        return std::stoi(cached_content_["brpc_max_concurrency"]);
      }
    }
    std::string content;
    if (GetNacosContent("brpc_max_concurrency", &content)) {
      UpdateConfigCacheContent("brpc_max_concurrency", content);
      return std::stoi(cached_content_["brpc_max_concurrency"]);
    }
    return 5000;
  }

  int DelayShutdownSec() {
    {
      absl::base_internal::SpinLockHolder locker(&lock_);
      if (cached_content_.find("delay_shutdown_sec") != cached_content_.end()) {
        return std::stoi(cached_content_["delay_shutdown_sec"]);
      }
    }

    std::string content;
    if (GetNacosContent("delay_shutdown_sec", &content)) {
      UpdateConfigCacheContent("delay_shutdown_sec", content);
      return std::stoi(cached_content_["delay_shutdown_sec"]);
    }
    return 5;
  }

 private:
  std::string base_config_path_;
  nacos::Properties props_;
  std::shared_ptr<nacos::NacosServiceFactory> nacos_config_service_factory_;
  std::shared_ptr<nacos::ConfigService> nacos_config_service_;
  mutable absl::base_internal::SpinLock lock_;
  std::map<std::string, std::string> cached_content_;

  config::BaseConfigPtr base_config_;
};

}  // namespace util
}  // namespace bazel_template
