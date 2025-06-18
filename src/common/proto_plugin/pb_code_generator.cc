/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "src/common/proto_plugin/pb_code_generator.h"

#include <fstream>
#include <memory>
#include <ostream>
#include <string>

#include "boost/algorithm/string/replace.hpp"
#include "glog/logging.h"
#include "google/protobuf/compiler/plugin.h"
#include "google/protobuf/io/printer.h"
#include "google/protobuf/io/zero_copy_stream.h"

using google::protobuf::FileDescriptor;
using google::protobuf::compiler::GeneratorContext;
using google::protobuf::io::Printer;
using google::protobuf::io::ZeroCopyOutputStream;
using std::string;

namespace bazel_template {
namespace common {
namespace proto_plugin {

bool LoadSmallFile(const string& path, string* content) {
  std::ifstream in(path, std::ios::binary);
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

PBCodeGenerator::PBCodeGenerator()
    : file_(nullptr), generator_context_(nullptr) {}

bool PBCodeGenerator::Generate(const FileDescriptor* file,
                               const string& parameter,
                               GeneratorContext* generator_context,
                               string*) const {
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
    auto& enum_l1 = *(file_->enum_type(idx));
    for (int j = 0; j < enum_l1.value_count(); ++j) {
      auto* value_descriptor = enum_l1.value(j);
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
  Print(out_path_, error_h_content);
  LOG(INFO) << error_h_content;
  return true;
}

bool PBCodeGenerator::Print(const string& file_name,
                            const string& content) const {
  if (file_name.size() <= 0 || content.size() <= 0) {
    LOG(ERROR) << "Empty file or context.file_name=" << file_name;
    return false;
  }
  std::unique_ptr<ZeroCopyOutputStream> output(
      generator_context_->Open(file_name));
  Printer printer(output.get(), '$', nullptr);
  printer.Print(content.c_str());
  return true;
}

string PBCodeGenerator::GetFullPath(const string& file_name) const {
  return template_dir_ + "/" + file_name;
}

}  // namespace proto_plugin
}  // namespace common
}  // namespace bazel_template

int main(int argc, char* argv[]) {
  bazel_template::common::proto_plugin::PBCodeGenerator generator;
  return google::protobuf::compiler::PluginMain(argc, argv, &generator);
}
