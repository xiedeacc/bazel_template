/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include <filesystem>
#include <format>
#include <fstream>
#include <iterator>
#include <memory>
#include <ostream>
#include <string>
#include <string_view>

#include "boost/algorithm/string/replace.hpp"
#include "google/protobuf/compiler/code_generator.h"
#include "google/protobuf/compiler/plugin.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/io/printer.h"
#include "google/protobuf/io/zero_copy_stream.h"
#include "src/common/logging.h"

using google::protobuf::FileDescriptor;
using google::protobuf::compiler::GeneratorContext;
using google::protobuf::io::Printer;
using google::protobuf::io::ZeroCopyOutputStream;
using std::string;

namespace bazel_template::common::proto_plugin {

class PBCodeGenerator : public google::protobuf::compiler::CodeGenerator {
 public:
  PBCodeGenerator();

  bool Generate(const google::protobuf::FileDescriptor* file,
                const std::string& parameter,
                google::protobuf::compiler::GeneratorContext* generator_context,
                std::string* error) const override;

 private:
  [[nodiscard]] bool Generate() const;

  bool Print(std::string_view file_name, std::string_view content) const;
  [[nodiscard]] std::string GetFullPath(std::string_view file_name) const;

  const mutable google::protobuf::FileDescriptor* file_{nullptr};
  mutable google::protobuf::compiler::GeneratorContext* generator_context_{
      nullptr};
  mutable std::string template_dir_;
  mutable std::string out_path_;
};

}  // namespace bazel_template::common::proto_plugin

namespace bazel_template::common::proto_plugin {

namespace {

constexpr std::string_view kErrorCodePlaceholder =
    "/** GENERATED_ERROR_CODE **/";
constexpr std::string_view kTemplateName = "error.h";

bool LoadSmallFile(std::string_view path, string* content) {
  std::ifstream in(std::filesystem::path(path), std::ios::binary);
  if (!in || !in.is_open()) {
    LOG(ERROR) << "Fail to open " << path
               << ", please check file exists and file permission";
    return false;
  }

  in.seekg(0, std::ios::end);
  content->reserve(in.tellg());
  in.seekg(0, std::ios::beg);

  std::copy((std::istreambuf_iterator<char>(in)),
            std::istreambuf_iterator<char>(), std::back_inserter(*content));
  in.close();
  return true;
}

}  // namespace

PBCodeGenerator::PBCodeGenerator() = default;

bool PBCodeGenerator::Generate(const FileDescriptor* file,
                               const string& parameter,
                               GeneratorContext* generator_context,
                               string* /*error*/) const {
  generator_context_ = generator_context;
  file_ = file;
  template_dir_ = parameter.substr(0, parameter.find(','));
  out_path_ = parameter.substr(parameter.find(',') + 1);
  LOG(INFO) << "template_dir: " << template_dir_ << ", out_dir: " << out_path_;
  return Generate();
}

bool PBCodeGenerator::Generate() const {
  string error_code_content;
  for (int idx = 0; idx < file_->enum_type_count(); ++idx) {
    const auto& enum_l1 = *(file_->enum_type(idx));
    for (int j = 0; j < enum_l1.value_count(); ++j) {
      const auto* value_descriptor = enum_l1.value(j);
      error_code_content.append("#define Err_");
      error_code_content.append(value_descriptor->name());
      error_code_content.append(" ");
      error_code_content.append(std::to_string(value_descriptor->number()));
      error_code_content.append("\n");
    }
  }

  string error_h_content;
  LoadSmallFile(GetFullPath("error.h"), &error_h_content);
  boost::algorithm::replace_all(error_h_content,
                                string("/** GENERATED_ERROR_CODE **/"),
                                error_code_content);
  if (!Print(out_path_, error_h_content)) {
    return false;
  }
  LOG(INFO) << error_h_content;
  return true;
}

bool PBCodeGenerator::Print(std::string_view file_name,
                            std::string_view content) const {
  if (file_name.empty() || content.empty()) {
    LOG(ERROR) << "Empty file or context.file_name=" << file_name;
    return false;
  }
  std::unique_ptr<ZeroCopyOutputStream> output(
      generator_context_->Open(string(file_name)));
  Printer printer(output.get(), '$', nullptr);
  printer.Print(content);
  return true;
}

string PBCodeGenerator::GetFullPath(std::string_view file_name) const {
  return std::format("{}/{}", template_dir_, file_name);
}

}  // namespace bazel_template::common::proto_plugin

int main(int argc, char* argv[]) {
  bazel_template::common::proto_plugin::PBCodeGenerator generator;
  return google::protobuf::compiler::PluginMain(argc, argv, &generator);
}
