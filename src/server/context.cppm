/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

// bazel_template.server.context: the execution context shared by the gRPC
// handlers. config_manager stays out of this interface (it reaches the
// generated config proto); the two methods that use it are in context.cc.
module;

#include <atomic>
#include <format>
#include <string>

#include "src/server/version_info.h"

export module bazel_template.server.context;

import bazel_template.async_grpc;

export namespace bazel_template::server {

class ServerContext : public async_grpc::ExecutionContext {
 public:
  ServerContext() : is_inited_(false), git_commit_(GIT_VERSION) {}

  static void MarkedHttpServerInitedDone();
  void MarkedGrpcServerInitedDone();

  [[nodiscard]] bool IsInitYet() const { return is_inited_.load(); }

  [[nodiscard]] std::string ToString() const {
    std::string info;
    info.reserve(1024);
    info.append(std::format("git commit: {}\n", git_commit_));
    return info;
  }

 private:
  std::atomic_bool is_inited_;
  std::string git_commit_;
};

}  // namespace bazel_template::server
