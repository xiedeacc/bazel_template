/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

// bazel_template.server.grpc_handler: registration of the gRPC handlers.
//
// The handler classes are not declared here on purpose: they derive from
// async_grpc::RpcHandler<> on generated protobuf messages, and a BMI whose
// declarations reach google::protobuf::Message crashes MSVC (C1001). Each
// handler lives in its own implementation unit and registers itself through
// the module-linkage functions at the end of this file.
module;

#include "grpc++/grpc++.h"

export module bazel_template.server.grpc_handler;

import bazel_template.async_grpc;

export namespace bazel_template::server::grpc_handler {

/// @brief Register every gRPC handler of the server with the builder.
void RegisterHandlers(async_grpc::Server::Builder& builder);

}  // namespace bazel_template::server::grpc_handler

// Module linkage: each implementation unit defines the one for its handler.
namespace bazel_template::server::grpc_handler {

void RegisterEC2Handler(async_grpc::Server::Builder& builder);
void RegisterRoute53Handler(async_grpc::Server::Builder& builder);

}  // namespace bazel_template::server::grpc_handler
