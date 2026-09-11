/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef BAZEL_TEMPLATE_PROTO_CONFIG_FWD_H_
#define BAZEL_TEMPLATE_PROTO_CONFIG_FWD_H_

// Forward declaration of the generated config message for module
// interfaces: a BMI must not reach google::protobuf::Message (MSVC C1001),
// so interfaces only name the type and implementation units include
// config.pb.h.
namespace bazel_template::proto {
class BaseConfig;
}  // namespace bazel_template::proto

#endif  // BAZEL_TEMPLATE_PROTO_CONFIG_FWD_H_
