/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef BAZEL_TEMPLATE_PROTO_PLUGIN_PB_CODE_GENERATOR
#define BAZEL_TEMPLATE_PROTO_PLUGIN_PB_CODE_GENERATOR

#include <set>
#include <string>
#include <vector>

#include "google/protobuf/compiler/code_generator.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/io/printer.h"

namespace bazel_template {
namespace proto_plugin {

class PBCodeGenerator : public google::protobuf::compiler::CodeGenerator {
 public:
  explicit PBCodeGenerator();

 public:
  bool Generate(const google::protobuf::FileDescriptor* file,
                const std::string& parameter,
                google::protobuf::compiler::GeneratorContext* generator_context,
                std::string* error) const override;

 private:
  bool Generate() const;

 private:
  bool Print(const std::string& file_name, const std::string& content) const;
  std::string GetFullPath(const std::string& file_name) const;

 private:
  const mutable google::protobuf::FileDescriptor* file_;
  mutable google::protobuf::compiler::GeneratorContext* generator_context_;
  mutable std::string template_dir_;
  mutable std::string out_path_;
};

}  // namespace proto_plugin
}  // namespace bazel_template

#endif  // BAZEL_TEMPLATE_PROTO_PLUGIN_PB_CODE_GENERATOR
