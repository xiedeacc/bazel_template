/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

// bazel_template.util: hashing, hex and JSON helpers. google::protobuf::Message
// is only forward-declared (util_fwd.h); util.cc has the protobuf headers.
module;

#include <cstddef>
#include <cstdint>
#include <string>

#include "openssl/evp.h"
#include "src/util/util_fwd.h"

export module bazel_template.util;

export namespace bazel_template::util {

class Util final {
 public:
  static std::string ToHexStr(uint64_t in, bool use_upper_case = false);
  static void ToHexStr(const std::string& in, std::string* out,
                       bool use_upper_case = false);
  static std::string ToHexStr(const std::string& in,
                              bool use_upper_case = false);

  static EVP_MD_CTX* HashInit(const EVP_MD* type);
  static bool HashUpdate(EVP_MD_CTX* context, const std::string& str);
  static bool HashFinal(EVP_MD_CTX* context, std::string* out,
                        bool use_upper_case = false);

  static EVP_MD_CTX* SHA256Init();
  static bool SHA256Update(EVP_MD_CTX* context, const std::string& str);
  static bool SHA256Final(EVP_MD_CTX* context, std::string* out,
                          bool use_upper_case = false);

  static bool JsonToMessage(const std::string& json,
                            google::protobuf::Message* msg);
  static bool MessageToJson(const google::protobuf::Message& msg,
                            std::string* json);

  static bool LoadSmallFile(const std::string& path, std::string* content);

 private:
  static constexpr size_t kAESKeySize = 16;   // 128 bits
  static constexpr size_t kNonceSize = 12;    // 96 bits
  static constexpr size_t kAuthTagSize = 16;  // 128 bits
};

}  // namespace bazel_template::util
