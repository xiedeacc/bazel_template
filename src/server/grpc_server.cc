/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

module;

#include <format>
#include <memory>
#include <string>

#include "grpc++/grpc++.h"

module bazel_template.server.grpc_server;

import bazel_template.server.grpc_handler;
import bazel_template.util.config_manager;

namespace bazel_template::server {

GrpcServer::GrpcServer(const std::shared_ptr<ServerContext>& server_context)
    : terminated_(false) {
  async_grpc::Server::Builder server_builder;
  const std::string addr_port =
      std::format("{}:{}", util::ConfigManager::Instance()->ServerAddr(),
                  util::ConfigManager::Instance()->GrpcServerPort());
  server_builder.SetServerAddress(addr_port);
  server_builder.SetNumGrpcThreads(
      util::ConfigManager::Instance()->GrpcThreads());
  server_builder.SetNumEventThreads(
      util::ConfigManager::Instance()->EventThreads());
  grpc_handler::RegisterHandlers(server_builder);
  server_ = server_builder.Build();
  server_->SetExecutionContext(server_context);
}

}  // namespace bazel_template::server
