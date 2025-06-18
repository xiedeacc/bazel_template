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

struct LoadFolderMethod {
  static constexpr const char* MethodName() {
    return "/bazel_template.proto.MEGAService/LoadFolder";
  }
  using IncomingType = bazel_template::proto::LoadFolderRequest;
  using OutgoingType = bazel_template::proto::LoadFolderResponse;
};

}  // namespace grpc_handler
}  // namespace server
}  // namespace bazel_template

#endif  // bazel_template_SERVER_GRPC_HANDLER_META_H_
