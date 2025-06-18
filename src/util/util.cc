/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "src/util/util.h"

#include <fstream>

#include "fmt/core.h"
#include "google/protobuf/json/json.h"

using google::protobuf::json::ParseOptions;
using google::protobuf::json::PrintOptions;
using std::string;

namespace bazel_template {
namespace util {

void Util::ToHexStr(const string &in, string *out, const bool use_upper_case) {
  out->clear();
  out->reserve(in.size() * 2);
  for (std::size_t i = 0; i < in.size(); ++i) {
    if (use_upper_case) {
      out->append(fmt::format("{:02X}", (unsigned char)in[i]));
    } else {
      out->append(fmt::format("{:02x}", (unsigned char)in[i]));
    }
  }
}

string Util::ToHexStr(const string &in, const bool use_upper_case) {
  string out;
  out.reserve(in.size() * 2);
  for (std::size_t i = 0; i < in.size(); ++i) {
    if (use_upper_case) {
      out.append(fmt::format("{:02X}", (unsigned char)in[i]));
    } else {
      out.append(fmt::format("{:02x}", (unsigned char)in[i]));
    }
  }
  return out;
}

EVP_MD_CTX *Util::HashInit(const EVP_MD *type) {
  EVP_MD_CTX *context = EVP_MD_CTX_new();
  if (!context) {
    return nullptr;
  }
  if (EVP_DigestInit_ex(context, type, nullptr) != 1) {
    EVP_MD_CTX_free(context);
    return nullptr;
  }
  return context;
}

bool Util::HashUpdate(EVP_MD_CTX *context, const string &str) {
  if (EVP_DigestUpdate(context, str.data(), str.size()) != 1) {
    EVP_MD_CTX_free(context);
    return false;
  }
  return true;
}

bool Util::HashFinal(EVP_MD_CTX *context, string *out,
                     const bool use_upper_case) {
  unsigned char hash[EVP_MAX_MD_SIZE];
  unsigned int length;
  if (EVP_DigestFinal_ex(context, hash, &length) != 1) {
    EVP_MD_CTX_free(context);
    return false;
  }

  EVP_MD_CTX_free(context);

  string s(reinterpret_cast<const char *>(hash), length);
  Util::ToHexStr(s, out, use_upper_case);
  return true;
}

EVP_MD_CTX *Util::SHA256Init() { return HashInit(EVP_sha256()); }

bool Util::SHA256Update(EVP_MD_CTX *context, const string &str) {
  return HashUpdate(context, str);
}

bool Util::SHA256Final(EVP_MD_CTX *context, string *out,
                       const bool use_upper_case) {
  return HashFinal(context, out, use_upper_case);
}

bool Util::JsonToMessage(const string &json, google::protobuf::Message *msg) {
  static ParseOptions option = {true, false};
  if (!JsonStringToMessage(json, msg, option).ok()) {
    return false;
  }
  return true;
}

bool Util::MessageToJson(const google::protobuf::Message &msg, string *json) {
  static PrintOptions option = {false, true, true, true, true};
  if (!MessageToJsonString(msg, json, option).ok()) {
    return false;
  }
  return true;
}

bool Util::LoadSmallFile(const string &path, string *content) {
  std::ifstream in(path, std::ios::binary);
  if (!in || !in.is_open()) {
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

}  // namespace util
}  // namespace bazel_template
