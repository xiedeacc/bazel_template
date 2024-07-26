/*******************************************************************************
 * Copyright (c) 2022 Copyright 2022- xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef GRPC_FRAMEWORK_TAG_BASE_H
#define GRPC_FRAMEWORK_TAG_BASE_H

#include <string>

namespace bazel_template {
namespace common {
namespace grpc_framework {

class TagBase {
 public:
  virtual void Process() = 0;
  virtual void OnError() = 0;
  virtual std::string Name() = 0;
};

}  // namespace grpc_framework
}  // namespace common
}  // namespace bazel_template

#endif  // GRPC_FRAMEWORK_TAG_BASE_H
