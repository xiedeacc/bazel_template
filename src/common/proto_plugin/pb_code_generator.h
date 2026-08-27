/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef BAZEL_TEMPLATE_COMMON_PROTO_PLUGIN_PB_CODE_GENERATOR_H_
#define BAZEL_TEMPLATE_COMMON_PROTO_PLUGIN_PB_CODE_GENERATOR_H_

#include <string>

#include "google/protobuf/compiler/code_generator.h"
#include "google/protobuf/descriptor.h"

namespace bazel_template::common::proto_plugin {

class PBCodeGenerator : public google::protobuf::compiler::CodeGenerator {
 public:
  PBCodeGenerator();

  bool Generate(const google::protobuf::FileDescriptor* file,
                const std::string& parameter,
                google::protobuf::compiler::GeneratorContext* generator_context,
                std::string* error) const override;

 private:
  bool Generate() const;

  bool Print(const std::string& file_name, const std::string& content) const;
  std::string GetFullPath(const std::string& file_name) const;

  const mutable google::protobuf::FileDescriptor* file_{nullptr};
  mutable google::protobuf::compiler::GeneratorContext* generator_context_{
      nullptr};
  mutable std::string template_dir_;
  mutable std::string out_path_;
};

}  // namespace bazel_template::common::proto_plugin

#endif  // BAZEL_TEMPLATE_COMMON_PROTO_PLUGIN_PB_CODE_GENERATOR_H_
