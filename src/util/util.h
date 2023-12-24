/*******************************************************************************
 * Copyright (c) 2023  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/
#pragma once

#include <map>
#include <set>

#include "absl/time/time.h"
#include "google/protobuf/message.h"

namespace bazel_template {
namespace util {

class Util {
 private:
  Util() {}
  virtual ~Util() {}

 public:
  static std::string GetServerIp();

  static int64_t CurrentTimeMillis();

  static int64_t NanoTime();

  static uint64_t Now();

  static uint64_t StrTimeToTimestamp(const std::string &time,
                                     int32_t offset = 0);

  static std::string ToTimeString(const int64_t ts);

  static std::string GetTodayString();

  static int64_t Random(int64_t start, int64_t end);

  static void Sleep(int64_t ms);

  static void SleepUntil(const absl::Time &time);

  static bool SleepUntil(const absl::Time &time, volatile bool *stop_signal);

  static std::string GetNacosNamingServiceUrl(const std::string &nacos_sdk_addr,
                                              const std::string &service_name,
                                              const std::string &group_name,
                                              const std::string &namespace_id,
                                              const std::string &clusters,
                                              bool healthy_only);

  static std::string DirName(const std::string &path);

  static std::string BaseName(const std::string &path);

  static std::string FileExtention(const std::string &path);

  static bool Remove(const std::string &dir, const std::string &file);

  static bool Remove(const std::string &dir);

  static bool Mkdir(const std::string &dir);

  static bool Exist(const std::string &path);

  static bool ListDir(const std::string &path, std::vector<std::string> *files,
                      const bool recursive = false);

  static bool CopyFile(const std::string &src_file_path,
                       const std::string dst_file_path);

  static bool Copy(const std::string &src_path, const std::string dst_path);

  static bool WriteToFile(const std::string &dir, const std::string &file,
                          const std::string &content);

  static std::istream &GetLine(std::istream &is, std::string *line);

  static std::string LoadContent(const std::string &file_name);

  static std::vector<std::string> LoadLines(const std::string &file_name);

  static std::string FileMd5(const std::string &file_path);

  static std::string ToUpper(const std::string &str);

  static std::string ToLower(const std::string &str);

  static void ToLower(std::string *str);

  static void Trim(std::string &s);

  static bool ToInt(const std::string &str, uint32_t *value);

  static bool StartWith(const std::string &str, const std::string &prefix);

  static void ReplaceAll(std::string &s, const std::string &from,
                         const std::string &to);

  static void Split(const std::string &str, const std::string &delim,
                    std::vector<std::string> &result, bool trim_empty = true);

  static std::string ToString(const std::set<uint64_t> &ids);

  static std::string ToString(const std::map<std::string, std::string> &vars);

  static std::string Base64Encode(const std::string &input);

  static std::string Base64Decode(const std::string &input);

  static std::string Md5(const std::string &str, bool use_upper_case = false);

  static uint64_t HexStrToUInt64(const std::string &in);

  static std::string UInt64ToHexStr(const uint64_t in);

  static std::string Hash(const std::string &str);

  static void PrintProtoMessage(const google::protobuf::Message &msg);

 public:
  static const char *kPathDelimeter;
};

}  // namespace util
}  // namespace bazel_template
