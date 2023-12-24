/*******************************************************************************
 * Copyright (c) 2023  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/
#pragma once

#include <string>

#include "google/protobuf/util/json_util.h"

namespace bazel_template {
namespace config {

class AbstractConfig {
 public:
  AbstractConfig() = delete;

  explicit AbstractConfig(const std::string &name) : name_(name) {
    parse_options_.case_insensitive_enum_parsing = true;
    parse_options_.ignore_unknown_fields = true;
    option_.add_whitespace = false;
    option_.always_print_enums_as_ints = true;
    option_.always_print_primitive_fields = true;
    option_.preserve_proto_field_names = true;
    DefaultValue();
  }

  virtual ~AbstractConfig() {}

  virtual bool Make(const std::string &value) = 0;

 protected:
  virtual bool CheckInvalidate() { return true; }
  virtual void DefaultValue() {}

 protected:
  std::string name_;
  google::protobuf::util::JsonParseOptions parse_options_;
  google::protobuf::util::JsonOptions option_;
};

}  // namespace config
}  // namespace bazel_template
