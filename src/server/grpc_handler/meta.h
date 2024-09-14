/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef BAZEL_TEMPLATE_SERVER_GRPC_HANDLERS_META_H
#define BAZEL_TEMPLATE_SERVER_GRPC_HANDLERS_META_H

#include "src/async_grpc/type_traits.h"
#include "src/proto/service.pb.h"

namespace bazel_template {
namespace server {
namespace grpc_handler {

struct GetSumMethod {
  static constexpr const char* MethodName() {
    return "/bazel_template.proto.Math/GetSum";
  }
  using IncomingType = async_grpc::Stream<bazel_template::proto::GetSumRequest>;
  using OutgoingType = bazel_template::proto::GetSumResponse;
};

struct GetRunningSumMethod {
  static constexpr const char* MethodName() {
    return "/bazel_template.proto.Math/GetRunningSum";
  }
  using IncomingType = async_grpc::Stream<bazel_template::proto::GetSumRequest>;
  using OutgoingType =
      async_grpc::Stream<bazel_template::proto::GetSumResponse>;
};

}  // namespace grpc_handler
}  // namespace server
}  // namespace bazel_template

#endif  // BAZEL_TEMPLATE_SERVER_GRPC_HANDLERS_META_H
