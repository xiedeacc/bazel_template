/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

// bazel_template.util.config_manager: the process configuration. The
// generated proto::BaseConfig is only forward-declared here and held through
// a unique_ptr; the accessors are in config_manager.cc.
module;

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>

#include "src/proto/config_fwd.h"

export module bazel_template.util.config_manager;

export namespace bazel_template::util {

class ConfigManager {
 private:
  ConfigManager();

 public:
  [[nodiscard]] static std::shared_ptr<ConfigManager> Instance();

  ~ConfigManager();

  ConfigManager(const ConfigManager&) = delete;
  ConfigManager& operator=(const ConfigManager&) = delete;
  ConfigManager(ConfigManager&&) = delete;
  ConfigManager& operator=(ConfigManager&&) = delete;

  [[nodiscard]] bool Init(std::string_view base_config_path);

  [[nodiscard]] std::string ServerAddr() const;
  [[nodiscard]] uint32_t HttpServerPort() const;
  [[nodiscard]] uint32_t GrpcServerPort() const;
  [[nodiscard]] uint32_t GrpcThreads() const;
  [[nodiscard]] uint32_t EventThreads() const;
  [[nodiscard]] uint32_t ClientWorkerThreadPoolSize() const;

  [[nodiscard]] std::string ToString() const;

 private:
  static constexpr uint32_t kDefaultGrpcThreads = 3;
  static constexpr uint32_t kDefaultEventThreads = 5;

  std::unique_ptr<proto::BaseConfig> base_config_;
};

}  // namespace bazel_template::util
