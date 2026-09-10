/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

module;

#include "grpc++/grpc++.h"

module bazel_template.server.grpc_handler;

namespace bazel_template::server::grpc_handler {

void RegisterHandlers(async_grpc::Server::Builder& builder) {
  RegisterEC2Handler(builder);
  RegisterRoute53Handler(builder);
}

}  // namespace bazel_template::server::grpc_handler
