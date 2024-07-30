/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "src/proto_plugin/pb_code_generator.h"

#include <algorithm>
#include <map>
#include <memory>
#include <ostream>
#include <utility>

#include "glog/logging.h"
#include "google/protobuf/compiler/plugin.h"
#include "google/protobuf/io/zero_copy_stream.h"
#include "src/util/util.h"

using bazel_template::util::Util;
using google::protobuf::Descriptor;
using google::protobuf::FieldDescriptor;
using google::protobuf::FileDescriptor;
using google::protobuf::compiler::GeneratorContext;
using google::protobuf::io::Printer;
using google::protobuf::io::ZeroCopyOutputStream;
using std::map;
using std::set;
using std::string;
using std::vector;

namespace bazel_template {
namespace proto_plugin {

PBCodeGenerator::PBCodeGenerator()
    : file_(nullptr), generator_context_(nullptr) {}

bool PBCodeGenerator::Generate(const FileDescriptor* file,
                               const string& parameter,
                               GeneratorContext* generator_context,
                               string* error) const {
  generator_context_ = generator_context;
  file_ = file;
  template_dir_ = parameter.substr(0, parameter.find(','));
  out_path_ = parameter.substr(parameter.find(',') + 1);
  LOG(INFO) << "template_dir: " << template_dir_ << ", out_dir: " << out_path_;
  return Generate();
}

bool PBCodeGenerator::Generate() const {
  string print_proto_message_name_content = "std::string msg_names = ";
  for (int idx = 0; idx < file_->message_type_count(); ++idx) {
    auto& level1_msg = *(file_->message_type(idx));
    print_proto_message_name_content.append("\"no ");
    print_proto_message_name_content.append(std::to_string(idx + 1));
    print_proto_message_name_content.append(": ");
    print_proto_message_name_content.append(level1_msg.name());
    print_proto_message_name_content.append("\"\n");
  }
  print_proto_message_name_content.append(";");

  for (int i = 0; i < file_->dependency_count(); ++i) {
    const FileDescriptor* dependency = file_->dependency(i);
    for (int j = 0; j < dependency->message_type_count(); ++j) {
      const Descriptor* message = dependency->message_type(j);
      LOG(INFO) << "Message name: " << message->name();
    }
  }

  string print_proto_message_name_final =
      Util::LoadContent(GetFullPath("print_proto_message_name.cc"));
  Util::Replace(string("/** print_proto_message_name_content **/"),
                print_proto_message_name_content,
                &print_proto_message_name_final);
  Print(out_path_, print_proto_message_name_final);

  LOG(INFO) << print_proto_message_name_final;
  return true;
}

bool PBCodeGenerator::Print(const string& file_name,
                            const string& content) const {
  LOG(INFO) << file_name;
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
}  // namespace bazel_template

int main(int argc, char* argv[]) {
  bazel_template::proto_plugin::PBCodeGenerator generator;
  return google::protobuf::compiler::PluginMain(argc, argv, &generator);
}
