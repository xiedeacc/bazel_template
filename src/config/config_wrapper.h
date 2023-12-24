/*******************************************************************************
 * Copyright (c) 2023  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/
#pragma once

#include <atomic>
#include <mutex>
#include <shared_mutex>

namespace bazel_template {
namespace config {

template <typename TypeName>
class ConfigWrapper {
 public:
  ConfigWrapper() : config_(nullptr), pos_(0) {}

  bool Make(const std::string &value) {
    std::unique_lock<std::shared_mutex> write_lock(mutex_);
    bool ret = buffer_configs_[1 - pos_.load()].Make(value);
    if (ret) {
      pos_.store(1 - pos_.load());
      config_ = &buffer_configs_[pos_.load()];
    }
    return ret;
  }

  TypeName *Hold() {
    std::shared_lock<std::shared_mutex> read_lock(mutex_);
    return config_;
  }

 private:
  TypeName buffer_configs_[2];
  TypeName *config_;
  std::atomic<int> pos_;
  std::shared_mutex mutex_;
};

}  // namespace config
}  // namespace bazel_template
