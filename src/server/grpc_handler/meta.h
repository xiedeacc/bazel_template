/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef bazel_template_SERVER_GRPC_HANDLER_META_H_
#define bazel_template_SERVER_GRPC_HANDLER_META_H_

#include "src/proto/service.pb.h"

namespace bazel_template {
namespace server {
namespace grpc_handler {

struct EC2InstanceManagementMethod {
  static constexpr const char* MethodName() {
    return "/bazel_template.proto.MEGAService/EC2InstanceManagement";
  }
  using IncomingType = bazel_template::proto::EC2InstanceRequest;
  using OutgoingType = bazel_template::proto::EC2InstanceResponse;
};

struct Route53ManagementMethod {
  static constexpr const char* MethodName() {
    return "/bazel_template.proto.MEGAService/Route53Management";
  }
  using IncomingType = bazel_template::proto::Route53Request;
  using OutgoingType = bazel_template::proto::Route53Response;
};

}  // namespace grpc_handler
}  // namespace server
}  // namespace bazel_template

#endif  // bazel_template_SERVER_GRPC_HANDLER_META_H_
