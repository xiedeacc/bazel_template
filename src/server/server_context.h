/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef BAZEL_TEMPLATE_SERVER_SERVER_CONTEXT_H_
#define BAZEL_TEMPLATE_SERVER_SERVER_CONTEXT_H_

#include <atomic>
#include <future>

#include "fmt/core.h"
#include "glog/logging.h"
#include "src/server/version_info.h"
#include "src/util/config_manager.h"

namespace bazel_template {
namespace server {

class ServerContext {
 public:
  ServerContext() : is_inited_(false), git_commit_(GIT_VERSION) {}

  void MarkedHttpServerInitedDone() {
    LOG(INFO) << "http server started on: "
              << util::ConfigManager::Instance()->ServerAddr() << ", port: "
              << util::ConfigManager::Instance()->HttpServerPort();
  }

  bool IsInitYet() { return is_inited_.load(); }

  std::string ToString() {
    std::string info;
    info.reserve(1024);
    info.append(fmt::format("git commit: {}\n", git_commit_));
    return info;
  }

 private:
  std::atomic_bool is_inited_;
  std::string git_commit_;
};

}  // namespace server
}  // namespace bazel_template

#endif  // BAZEL_TEMPLATE_SERVER_SERVER_CONTEXT_H_
