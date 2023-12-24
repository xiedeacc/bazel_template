/*******************************************************************************
 * Copyright (c) 2023  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "src/util/config_util.h"

#include "listen/Listener.h"

namespace bazel_template {
namespace util {

using std::map;
using std::shared_ptr;
using std::string;

#define AddNacosConfigListener(config_name)                                   \
  NacosConfigListener *config_name##_listener =                               \
      new NacosConfigListener(#config_name);                                  \
  nacos_config_service_->addListener(#config_name,                            \
                                     base_config_->config.nacos_group_name(), \
                                     config_name##_listener);

using nacos::ConfigService;
using nacos::NacosServiceFactory;
using nacos::PropertyKeyConst;
using namespace std;

class NacosConfigListener : public nacos::Listener {
 private:
  string key;

 public:
  NacosConfigListener(const string &key) { this->key = key; }

  void receiveConfigInfo(const NacosString &configInfo) {
    LOG(INFO) << "Watcher key: " << key << " UPDATED:" << configInfo;
    ConfigUtil::Instance().UpdateConfigCacheContent(key, configInfo);
  }
};

bool ConfigUtil::Init(const string &base_config_path) {
  base_config_path_ = std::move(base_config_path);
  if (!BaseConfig()) {
    LOG(ERROR) << "Init fail!";
    return false;
  }

  auto &base_config = base_config_->config;
  props_[PropertyKeyConst::SERVER_ADDR] = base_config.nacos_sdk_addr();
  props_[PropertyKeyConst::NAMESPACE] = base_config.nacos_namespace();

  LOG(INFO) << "nacos_sdk_addr: " << base_config.nacos_sdk_addr()
            << ", nacos_namespace: " << base_config.nacos_namespace();

  nacos_config_service_factory_ =
      std::shared_ptr<NacosServiceFactory>(new NacosServiceFactory(props_));
  nacos_config_service_ = std::shared_ptr<ConfigService>(
      nacos_config_service_factory_->CreateConfigService());

  AddNacosConfigListener(brpc_server_port);
  AddNacosConfigListener(brpc_http_server_port);
  AddNacosConfigListener(brpc_config_server_port);
  AddNacosConfigListener(brpc_max_concurrency);
  AddNacosConfigListener(brpc_num_threads);
  AddNacosConfigListener(delay_shutdown_sec);

  return true;
}

void ConfigUtil::UpdateConfigCacheContent(const string &key,
                                          const string &content) {
  try {
    if (content.empty()) {
      LOG(WARNING) << "Try to update a empty content, key: " << key;
      return;
    }

    {
      absl::base_internal::SpinLockHolder locker(&lock_);
      cached_content_[key] = content;
    }

  } catch (std::exception &e) {
    LOG(ERROR) << "Update config error, key: " << key
               << ", content: " << content;
  }
}

string ConfigUtil::GetContent(const string &key) {
  {
    absl::base_internal::SpinLockHolder locker(&lock_);
    if (cached_content_.find(key) != cached_content_.end() &&
        !cached_content_[key].empty()) {
      return cached_content_[key];
    }
  }

  string content;
  if (GetNacosContent(key, &content)) {
    UpdateConfigCacheContent(key, content);
    return cached_content_[key];
  }
  return "";
}

string ConfigUtil::GetContent(const string &key, const string &group_name) {
  {
    absl::base_internal::SpinLockHolder locker(&lock_);
    if (cached_content_.find(key) != cached_content_.end() &&
        !cached_content_[key].empty()) {
      return cached_content_[key];
    }
  }

  string content;
  if (GetNacosContent(key, group_name, &content)) {
    UpdateConfigCacheContent(key, content);
    return cached_content_[key];
  }
  return "";
}

}  // namespace util
}  // namespace bazel_template
