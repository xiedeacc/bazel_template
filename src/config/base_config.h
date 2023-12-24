/*******************************************************************************
 * Copyright (c) 2023  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/
#pragma once

#include <memory>

#include "glog/logging.h"
#include "src/config/abstract_config.h"
#include "src/proto/config.pb.h"

namespace bazel_template {
namespace config {

class BaseConfig : public AbstractConfig {
 public:
  BaseConfig() : AbstractConfig("BaseConfig") {}
  virtual ~BaseConfig() {}

  virtual bool Make(const std::string &value) {
    if (value.empty()) {
      LOG(ERROR) << "empty json string";
      return false;
    }

    if (!google::protobuf::util::JsonStringToMessage(value, &config,
                                                     parse_options_)
             .ok()) {
      LOG(ERROR) << "parse BaseConfig error, value: " << value;
      return false;
    }

    return CheckInvalidate();
  }

  virtual std::string ToString() {
    std::string json_value;
    if (!google::protobuf::util::MessageToJsonString(config, &json_value,
                                                     option_)
             .ok()) {
      LOG(ERROR) << "to json string failed";
    }
    return json_value;
  }

 protected:
  virtual void DefaultValue() { config.set_nacos_sdk_rcv_udp_port(0); }

  virtual bool CheckInvalidate() {
    if (config.nacos_sdk_addr().empty() || config.nacos_namespace().empty() ||
        config.nacos_service_name().empty() ||
        config.nacos_cluster_name().empty() ||
        config.nacos_group_name().empty()) {
      return false;
    }
    return true;
  }

 public:
  bazel_template::proto::BaseConfig config;
};

typedef std::shared_ptr<BaseConfig> BaseConfigPtr;

}  // namespace config
}  // namespace bazel_template
