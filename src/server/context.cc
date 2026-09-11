/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

module;

#include "src/common/logging.h"

module bazel_template.server.context;

import bazel_template.util.config_manager;

namespace bazel_template::server {

void ServerContext::MarkedHttpServerInitedDone() {
  LOG(INFO) << "HTTP server started on: "
            << util::ConfigManager::Instance()->ServerAddr()
            << ", port: " << util::ConfigManager::Instance()->HttpServerPort();
}

void ServerContext::MarkedGrpcServerInitedDone() {
  LOG(INFO) << "gRPC server started on: "
            << util::ConfigManager::Instance()->ServerAddr()
            << ", port: " << util::ConfigManager::Instance()->GrpcServerPort();
  is_inited_ = true;
}

}  // namespace bazel_template::server
