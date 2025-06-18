/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef BAZEL_TEMPLATE_UTIL_UTIL_H_
#define BAZEL_TEMPLATE_UTIL_UTIL_H_

#include <string>

#include "google/protobuf/message.h"
#include "openssl/evp.h"

namespace bazel_template {
namespace util {

class Util final {
 public:
  static std::string ToHexStr(const uint64_t in,
                              const bool use_upper_case = false);

  static void ToHexStr(const std::string &in, std::string *out,
                       const bool use_upper_case = false);

  static std::string ToHexStr(const std::string &in,
                              const bool use_upper_case = false);

  static EVP_MD_CTX *HashInit(const EVP_MD *type);
  static bool HashUpdate(EVP_MD_CTX *context, const std::string &str);
  static bool HashFinal(EVP_MD_CTX *context, std::string *out,
                        const bool use_upper_case = false);

  static EVP_MD_CTX *SHA256Init();
  static bool SHA256Update(EVP_MD_CTX *context, const std::string &str);
  static bool SHA256Final(EVP_MD_CTX *context, std::string *out,
                          const bool use_upper_case = false);
  static bool JsonToMessage(const std::string &json,
                            google::protobuf::Message *msg);
  static bool MessageToJson(const google::protobuf::Message &msg,
                            std::string *json);
  static bool LoadSmallFile(const std::string &path, std::string *content);

 private:
  static constexpr size_t kAESKeySize = 16;   // 128 bits
  static constexpr size_t kNonceSize = 12;    // 96 bits
  static constexpr size_t kAuthTagSize = 16;  // 128 bits
};

}  // namespace util
}  // namespace bazel_template

#endif  // BAZEL_TEMPLATE_UTIL_UTIL_H_
