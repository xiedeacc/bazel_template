/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

// bazel_template.server.grpc_server: the async_grpc server with the
// project's handlers. Registration happens in grpc_server.cc, which imports
// the handler module; this interface stays free of protobuf types.
module;

#include <atomic>
#include <memory>

// Included here as well as in the async_grpc module: MSVC otherwise reports
// grpc's static inline helpers from the imported BMI as declared but not
// defined.
#include "grpc++/grpc++.h"

export module bazel_template.server.grpc_server;

import bazel_template.async_grpc;
import bazel_template.server.context;

export namespace bazel_template::server {

class GrpcServer final {
 public:
  explicit GrpcServer(const std::shared_ptr<ServerContext>& server_context);

  void Start() {
    server_->Start();
    server_->GetContext<ServerContext>()->MarkedGrpcServerInitedDone();
  }

  void Shutdown() { server_->Shutdown(); }

  void WaitForShutdown() { server_->WaitForShutdown(); }

  [[nodiscard]] bool terminated() const { return terminated_.load(); }

  void set_terminated(bool value) { terminated_.store(value); }

 private:
  std::unique_ptr<async_grpc::Server> server_;
  std::atomic_bool terminated_;
};

}  // namespace bazel_template::server
