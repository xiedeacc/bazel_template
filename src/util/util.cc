/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "src/util/util.h"

#include <algorithm>
#include <charconv>
#include <exception>
#include <filesystem>
#include <fstream>
#include <random>
#include <thread>
#include <utility>

#include "absl/time/clock.h"
#include "boost/algorithm/string/predicate.hpp"
#include "boost/algorithm/string/split.hpp"
#include "boost/algorithm/string/trim_all.hpp"
#include "boost/iostreams/device/mapped_file.hpp"
#include "fmt/core.h"
#include "glog/logging.h"
#include "google/protobuf/util/json_util.h"
#include "openssl/md5.h"
#include "src/MurmurHash2.h"
#include "src/MurmurHash3.h"
#include "src/common/ip_address.h"

using absl::FormatTime;
using absl::FromUnixMillis;
using absl::GetCurrentTimeNanos;
using absl::LoadTimeZone;
using absl::Milliseconds;
using absl::SleepFor;
using absl::Time;
using absl::TimeZone;
using google::protobuf::util::JsonParseOptions;
using google::protobuf::util::JsonPrintOptions;
using std::string;

namespace bazel_template {
namespace util {

const char *Util::kPathDelimeter = "/";

string Util::GetServerIp() {
  common::IPAddress ip_address;
  if (!common::IPAddress::GetFirstPrivateAddress(&ip_address)) {
    LOG(ERROR) << "Failed to get local ip address";
    return "";
  }
  return ip_address.ToString();
}

int64_t Util::CurrentTimeMillis() {
  return absl::GetCurrentTimeNanos() / 1000000;
}

int64_t Util::NanoTime() { return GetCurrentTimeNanos(); }

uint64_t Util::Now() {
  auto duration_since_epoch =
      std::chrono::system_clock::now().time_since_epoch();
  auto microseconds_since_epoch =
      std::chrono::duration_cast<std::chrono::microseconds>(
          duration_since_epoch)
          .count();
  time_t seconds_since_epoch =
      static_cast<time_t>(microseconds_since_epoch / 1000000);  // second
  return seconds_since_epoch;
}

uint64_t Util::StrTimeToTimestamp(const string &time, int32_t offset) {
  absl::Time t;
  string err;
  if (!absl::ParseTime("%Y-%m-%d%ET%H:%M:%E3S%Ez", time, &t, &err)) {
    LOG(ERROR) << "convert " << time << " " << err;
    return 0;
  }
  return absl::ToUnixMillis(t) + offset;
}

string Util::ToTimeString(const int64_t ts) {
  TimeZone time_zone;
  LoadTimeZone("Asia/Shanghai", &time_zone);
  return FormatTime("%Y-%m-%d %H:%M:%S", FromUnixMillis(ts), time_zone);
}

string Util::GetTodayString() {
  absl::Time now = absl::Now();
  absl::TimeZone loc = absl::LocalTimeZone();
  return absl::FormatTime("%Y-%m-%d", now, loc);
}

int64_t Util::Random(int64_t start, int64_t end) {
  static thread_local std::mt19937 generator(NanoTime());
  std::uniform_int_distribution<int64_t> distribution(start, end - 1);
  return distribution(generator);
}

void Util::Sleep(int64_t ms) {
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void Util::SleepUntil(const Time &time) {
  volatile bool signal = false;
  SleepUntil(time, &signal);
}

bool Util::SleepUntil(const Time &time, volatile bool *stop_signal) {
  auto now = absl::Now();
  if (time <= now) {
    return true;
  }
  while (absl::Now() < time) {
    if (*stop_signal) {
      return false;
    }
    SleepFor(Milliseconds(100));
  }
  return true;
}

bool Util::IsDir(const string &path) {
  std::filesystem::path file_path(path);
  return std::filesystem::is_directory(file_path);
}

bool Util::IsFile(const string &dir, const string &file) {
  std::filesystem::path file_path(dir + "/" + file);
  return std::filesystem::is_regular_file(file_path);
}

string Util::DirName(const string &path) {
  std::filesystem::path file_path(path);
  return file_path.parent_path().string();
}

string Util::BaseName(const string &path) {
  std::filesystem::path file_path(path);
  return file_path.filename().string();
}

std::string Util::FileExtention(const std::string &path) {
  std::filesystem::path file_path(path);
  return file_path.extension().string();
}

bool Util::Remove(const string &dir, const string &file) {
  string path;
  if (dir.empty()) {
    path = file;
  } else if (dir[dir.size() - 1] == '/') {
    path = dir + file;
  } else {
    path = dir + "/" + file;
  }

  try {
    if (std::filesystem::exists(path)) {
      return std::filesystem::remove(path);
    }
  } catch (std::exception &e) {
    LOG(ERROR) << e.what();
    return false;
  }

  return true;
}

bool Util::Remove(const string &dir) {
  try {
    if (std::filesystem::exists(dir)) {
      return std::filesystem::remove_all(dir);
    }
  } catch (const std::filesystem::filesystem_error &e) {
    LOG(ERROR) << e.what();
    return false;
  }

  return true;
}

bool Util::Mkdir(const string &dir) {
  try {
    std::filesystem::create_directories(dir);
  } catch (const std::exception &e) {
    LOG(ERROR) << "Mkdir " << dir << " error";
    return false;
  }
  return true;
}

bool Util::Exist(const string &path) { return std::filesystem::exists(path); }

string Util::TruncatePath(const string &src, const string &path) {
  string real_path = std::filesystem::canonical(src).string();
  std::string::size_type n = real_path.find(path);
  if (n != string::npos) {
    return real_path.substr(0, n);
  }
  return "";
}

string Util::RealPath(const string &path) {
  return std::filesystem::canonical(path).string();
}

bool Util::ListDir(const string &path, std::vector<string> *files,
                   const bool recursive) {
  if (!files) {
    return true;
  }
  try {
    std::filesystem::path dir_path(path);
    if (!std::filesystem::is_directory(dir_path)) {
      return false;
    }
    if (recursive) {
      std::filesystem::recursive_directory_iterator it(dir_path);
      std::filesystem::recursive_directory_iterator end;
      for (; it != end; ++it) {
        files->emplace_back(it->path().string());
      }
    } else {
      std::filesystem::directory_iterator it(dir_path);
      std::filesystem::directory_iterator end;
      for (; it != end; ++it) {
        files->emplace_back(it->path().string());
      }
    }
  } catch (const std::exception &e) {
    LOG(ERROR) << e.what();
    return false;
  }
  return true;
}

bool Util::ListFile(const string &path, std::vector<string> *files,
                    const bool recursive) {
  if (!files) {
    return true;
  }
  try {
    std::filesystem::path dir_path(path);
    if (!std::filesystem::is_directory(dir_path)) {
      return false;
    }
    if (recursive) {
      std::filesystem::recursive_directory_iterator it(dir_path);
      std::filesystem::recursive_directory_iterator end;
      for (; it != end; ++it) {
        if (std::filesystem::is_regular_file(it->status())) {
          files->emplace_back(it->path().string());
        }
      }
    } else {
      std::filesystem::directory_iterator it(dir_path);
      std::filesystem::directory_iterator end;
      for (; it != end; ++it) {
        if (std::filesystem::is_regular_file(it->status())) {
          files->emplace_back(it->path().string());
        }
      }
    }
  } catch (const std::exception &e) {
    LOG(ERROR) << e.what();
    return false;
  }
  return true;
}

bool Util::ListFile(const string &path,
                    std::vector<std::filesystem::path> *files,
                    const std::string &name_pattern,
                    const std::string &ignore_ext, const bool recursive) {
  if (!files) {
    return true;
  }

  try {
    std::filesystem::path dir_path(path);
    if (!std::filesystem::is_directory(dir_path)) {
      LOG(ERROR) << path << " not directory";
      return false;
    }

    if (recursive) {
      std::filesystem::recursive_directory_iterator it(dir_path);
      std::filesystem::recursive_directory_iterator end;
      for (; it != end; ++it) {
        if (std::filesystem::is_regular_file(it->status()) &&
            it->path().extension().string() != ignore_ext) {
          if (!name_pattern.empty() &&
              it->path().string().find(name_pattern) == string::npos) {
            continue;
          }
          files->emplace_back(it->path().string());
        }
      }
    } else {
      std::filesystem::directory_iterator it(dir_path);
      std::filesystem::directory_iterator end;
      for (; it != end; ++it) {
        if (std::filesystem::is_regular_file(it->status()) &&
            it->path().extension().string() != ignore_ext) {
          if (!name_pattern.empty() &&
              it->path().string().find(name_pattern) == string::npos) {
            continue;
          }
          files->emplace_back(it->path());
        }
      }
    }
  } catch (const std::exception &e) {
    LOG(ERROR) << e.what();
    return false;
  }

  return true;
}

bool Util::CopyFile(const string &src_file_path, const string dst_file_path,
                    const std::filesystem::copy_options opt) {
  try {
    return std::filesystem::copy_file(src_file_path, dst_file_path, opt);
  } catch (std::exception &e) {
    LOG(ERROR) << "copy " << src_file_path << " to " << dst_file_path
               << " exception, " << e.what();
  }
  return false;
}

bool Util::Copy(const string &src_path, const string dst_path) {
  try {
    std::filesystem::copy(src_path, dst_path,
                          std::filesystem::copy_options::recursive);
  } catch (std::exception &e) {
    LOG(ERROR) << "copy " << src_path << " to " << dst_path << " exception, "
               << e.what();
    return false;
  }
  return true;
}

bool Util::WriteToFile(const string &dir, const string &file,
                       const string &content) {
  std::filesystem::path path;
  if (dir.empty()) {
    path = file;
  } else if (dir[dir.size() - 1] == '/') {
    path = dir + file;
  } else {
    path = dir + "/" + file;
  }

  try {
    if (!dir.empty() && !std::filesystem::exists(dir)) {
      std::filesystem::create_directories(dir);
    }

    if (std::filesystem::exists(path)) {
      std::filesystem::remove(path);
    }

    std::ofstream ofs(path);
    if (ofs.is_open()) {
      ofs << content;
      ofs.close();
    }
  } catch (std::exception &e) {
    LOG(ERROR) << path.string() << " " << e.what();
    return false;
  }

  return true;
}

std::istream &Util::GetLine(std::istream &is, std::string *line) {
  line->clear();
  std::istream::sentry se(is, true);
  std::streambuf *sb = is.rdbuf();

  for (;;) {
    int c = sb->sbumpc();
    switch (c) {
      case '\n':
        return is;
      case '\r':
        if (sb->sgetc() == '\n') sb->sbumpc();
        return is;
      case std::streambuf::traits_type::eof():
        if (line->empty()) is.setstate(std::ios::eofbit);
        return is;
      default:
        line->push_back(static_cast<char>(c));
    }
  }
}

string Util::LoadContent(const string &file_name) {
  std::ifstream in(file_name);
  if (!in || !in.is_open()) {
    LOG(ERROR) << "Fail to open " << file_name;
    return "";
  }
  std::stringstream buffer;
  buffer << in.rdbuf();
  in.close();
  return buffer.str();
}

std::vector<string> Util::LoadLines(const string &file_name) {
  std::ifstream in(file_name);
  if (!in) {
    LOG(ERROR) << "Fail to open " << file_name;
    return std::vector<string>();
  }
  std::vector<string> ret;
  string line;
  while (getline(in, line)) {
    Util::Trim(&line);
    if (line.empty()) {
      continue;
    }
    if (line[0] == '#') {
      continue;
    }
    ret.emplace_back(std::move(line));
  }
  in.close();
  return ret;
}

string Util::FileMd5(const string &file_path) {
  unsigned char result[MD5_DIGEST_LENGTH];
  boost::iostreams::mapped_file_source src(file_path);
  MD5((unsigned char *)src.data(), src.size(), result);
  std::ostringstream sout;
  sout << std::hex << std::setfill('0');
  for (auto c : result) sout << std::setw(2) << static_cast<int>(c);

  return sout.str();
}

string Util::ToUpper(const string &str) {
  string ret = str;
  transform(ret.begin(), ret.end(), ret.begin(),
            [](unsigned char c) { return toupper(c); });
  // transform(ret.begin(), ret.end(), ret.begin(), ::toupper);
  return ret;
}

string Util::ToLower(const string &str) {
  string ret = str;
  transform(ret.begin(), ret.end(), ret.begin(), ::tolower);
  return ret;
}

void Util::ToLower(string *str) {
  transform(str->begin(), str->end(), str->begin(), ::tolower);
}

void Util::Trim(string *str) {
  boost::algorithm::trim_right(*str);
  boost::algorithm::trim_left(*str);
}

string Util::Trim(const string &str) {
  string trimmed_str = str;
  boost::algorithm::trim_right(trimmed_str);
  boost::algorithm::trim_left(trimmed_str);
  return trimmed_str;
}

bool Util::ToInt(const string &str, uint32_t *value) {
  auto result = std::from_chars(str.data(), str.data() + str.size(), *value);
  if (result.ec != std::errc{}) {
    return false;
  }
  return true;
}

bool Util::StartWith(const string &str, const string &prefix) {
  return boost::starts_with(str, prefix);
}

bool Util::EndWith(const string &str, const string &postfix) {
  return boost::ends_with(str, postfix);
}

void Util::ReplaceAll(string *s, const string &from, const string &to) {
  boost::algorithm::replace_all(*s, from, to);
}

void Util::ReplaceAll(string *s, const string &from, const char *const to) {
  boost::algorithm::replace_all(*s, from, to);
}

void Util::Split(const string &str, const string &delim,
                 std::vector<string> *result, bool trim_empty) {
  result->clear();
  if (str.empty()) {
    return;
  }
  if (trim_empty) {
    string trimed_str = boost::algorithm::trim_all_copy(str);
    boost::split(*result, trimed_str, boost::is_any_of(delim));
    return;
  }
  boost::split(*result, str, boost::is_any_of(delim));
}

string Util::ToString(const std::set<uint64_t> &ids) {
  string ret;
  ret.resize(512);
  bool first = true;
  for (auto id : ids) {
    if (first) {
      first = false;
      ret.append(std::to_string(id));
      continue;
    }
    ret.append(",");
    ret.append(std::to_string(id));
  }
  return ret;
}

std::string Util::ToString(const std::map<string, string> &vars) {
  string ret;
  ret.resize(512);
  bool first = true;
  for (auto &p : vars) {
    if (first) {
      first = false;
      ret.append(p.first);
      ret.append("=");
      ret.append(p.second);
      continue;
    }
    ret.append("&");
    ret.append(p.first);
    ret.append("=");
    ret.append(p.second);
  }
  return ret;
}

string Util::StandardBase64Encode(const string &input) {
  string output;
  // try {
  // output.resize(boost::beast::detail::base64::encoded_size(input.size()));
  // auto const ret = boost::beast::detail::base64::encode(
  // output.data(), input.data(), input.size());
  // output.resize(ret);
  //} catch (exception &e) {
  // LOG(INFO) << "Base64 encode error";
  //}
  return output;
}

string Util::StandardBase64Decode(const string &input) {
  string output;
  // try {
  // output.resize(boost::beast::detail::base64::decoded_size(input.size()));
  // auto const ret = boost::beast::detail::base64::decode(
  // output.data(), input.data(), input.size());
  // output.resize(ret.first);
  //} catch (exception &e) {
  // LOG(INFO) << "Base64 decode error";
  //}
  return output;
}

static const char *base64_chars[2] = {
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789"
    "+/",
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789"
    "-_"};

static unsigned int CharPos(const unsigned char chr) {
  if (chr >= 'A' && chr <= 'Z')
    return chr - 'A';
  else if (chr >= 'a' && chr <= 'z')
    return chr - 'a' + ('Z' - 'A') + 1;
  else if (chr >= '0' && chr <= '9')
    return chr - '0' + ('Z' - 'A') + ('z' - 'a') + 2;
  else if (chr == '+' || chr == '-')
    return 62;  // Be liberal with input and accept both url ('-') and non-url
                // ('+') base 64 characters (
  else if (chr == '/' || chr == '_')
    return 63;  // Ditto for '/' and '_'
  else
    throw std::runtime_error("Input is not valid base64-encoded data.");
}

static std::string InsertLinebreaks(std::string str, size_t distance) {
  if (!str.length()) {
    return "";
  }

  size_t pos = distance;
  while (pos < str.size()) {
    str.insert(pos, "\n");
    pos += distance + 1;
  }
  return str;
}

std::string Base64EncodeImpl(unsigned char const *bytes_to_encode,
                             size_t in_len, bool url) {
  size_t len_encoded = (in_len + 2) / 3 * 4;
  unsigned char trailing_char = url ? '.' : '=';
  const char *base64_chars_ = base64_chars[url];
  std::string ret;
  ret.reserve(len_encoded);

  unsigned int pos = 0;
  while (pos < in_len) {
    ret.push_back(base64_chars_[(bytes_to_encode[pos + 0] & 0xfc) >> 2]);
    if (pos + 1 < in_len) {
      ret.push_back(base64_chars_[((bytes_to_encode[pos + 0] & 0x03) << 4) +
                                  ((bytes_to_encode[pos + 1] & 0xf0) >> 4)]);

      if (pos + 2 < in_len) {
        ret.push_back(base64_chars_[((bytes_to_encode[pos + 1] & 0x0f) << 2) +
                                    ((bytes_to_encode[pos + 2] & 0xc0) >> 6)]);
        ret.push_back(base64_chars_[bytes_to_encode[pos + 2] & 0x3f]);
      } else {
        ret.push_back(base64_chars_[(bytes_to_encode[pos + 1] & 0x0f) << 2]);
        ret.push_back(trailing_char);
      }
    } else {
      ret.push_back(base64_chars_[(bytes_to_encode[pos + 0] & 0x03) << 4]);
      ret.push_back(trailing_char);
      ret.push_back(trailing_char);
    }
    pos += 3;
  }
  return ret;
}

template <typename String>
static std::string Base64EncodeImpl(String s, bool url) {
  return Base64EncodeImpl(reinterpret_cast<const unsigned char *>(s.data()),
                          s.length(), url);
}

template <typename String, unsigned int line_length>
static std::string EncodeWithLineBreaks(String s) {
  return InsertLinebreaks(Base64EncodeImpl(s, false), line_length);
}

template <typename String>
static std::string Base64EncodePemImpl(String s) {
  return EncodeWithLineBreaks<String, 64>(s);
}

template <typename String>
static std::string Base64EncodeMimeImpl(String s) {
  return EncodeWithLineBreaks<String, 76>(s);
}

string Util::Base64Encode(std::string_view s, bool url) {
  return Base64EncodeImpl(s, url);
}

string Util::Base64EncodePem(std::string_view s) {
  return Base64EncodePemImpl(s);
}

string Util::Base64EncodeMime(std::string_view s) {
  return Base64EncodeMimeImpl(s);
}

template <typename String>
static std::string Base64DecodeImpl(String const &encoded_string,
                                    bool remove_linebreaks) {
  if (encoded_string.empty()) return std::string();
  if (remove_linebreaks) {
    std::string copy(encoded_string);
    copy.erase(std::remove(copy.begin(), copy.end(), '\n'), copy.end());
    return Base64DecodeImpl(copy, false);
  }
  size_t length_of_string = encoded_string.length();
  size_t pos = 0;

  size_t approx_length_of_decoded_string = length_of_string / 4 * 3;
  std::string ret;
  ret.reserve(approx_length_of_decoded_string);

  while (pos < length_of_string) {
    size_t CharPos_1 = CharPos(encoded_string.at(pos + 1));
    ret.push_back(static_cast<std::string::value_type>(
        ((CharPos(encoded_string.at(pos + 0))) << 2) +
        ((CharPos_1 & 0x30) >> 4)));

    if ((pos + 2 <
         length_of_string) &&  // Check for data that is not padded with equal
                               // signs (which is allowed by RFC 2045)
        encoded_string.at(pos + 2) != '=' &&
        encoded_string.at(pos + 2) !=
            '.'  // accept URL-safe base 64 strings, too, so check for '.' also.
    ) {
      unsigned int CharPos_2 = CharPos(encoded_string.at(pos + 2));
      ret.push_back(static_cast<std::string::value_type>(
          ((CharPos_1 & 0x0f) << 4) + ((CharPos_2 & 0x3c) >> 2)));

      if ((pos + 3 < length_of_string) && encoded_string.at(pos + 3) != '=' &&
          encoded_string.at(pos + 3) != '.') {
        ret.push_back(static_cast<std::string::value_type>(
            ((CharPos_2 & 0x03) << 6) + CharPos(encoded_string.at(pos + 3))));
      }
    }
    pos += 4;
  }
  return ret;
}

std::string Base64Decode(std::string const &s, bool remove_linebreaks) {
  return Base64DecodeImpl(s, remove_linebreaks);
}
std::string Util::Base64Decode(std::string_view s, bool remove_linebreaks) {
  return Base64DecodeImpl(s, remove_linebreaks);
}

string Util::Md5(const string &str, bool use_upper_case) {
  unsigned char result[MD5_DIGEST_LENGTH];
  MD5((unsigned char *)str.data(), str.size(), result);
  string hex_str;
  hex_str.reserve(512);

  string format_para = "{:02x}";
  if (use_upper_case) {
    format_para = "{:02X}";
  }

  for (auto c : result) {
    hex_str.append(fmt::format(format_para, c));
  }
  return hex_str;
}

uint64_t Util::HexStrToUInt64(const string &in) {
  try {
    return std::stoull(in, nullptr, 16);
  } catch (std::exception &e) {
    LOG(ERROR) << "Parse error: " << in;
  }
  return 0;
}

string Util::UInt64ToHexStr(const uint64_t in) {
  try {
    return fmt::format("{:016x}", in);
  } catch (std::exception &e) {
    LOG(ERROR) << "Parse error: " << in;
  }
  return "";
}

string Util::Hash(const string &str) {
  int32_t res;
  MurmurHash3_x86_32(str.c_str(), str.length(), 0, &res);
  res = abs(res);
  return std::to_string(res);
}

int64_t Util::Hash64(const string &str) {
  return MurmurHash64A(str.c_str(), str.length(), 42L);
}

void Util::PrintProtoMessage(const google::protobuf::Message &msg) {
  JsonPrintOptions option;
  option.add_whitespace = false;
  // option.always_print_primitive_fields = true;
  option.preserve_proto_field_names = true;
  string json_value;
  if (!MessageToJsonString(msg, &json_value, option).ok()) {
    LOG(ERROR) << "to json string failed";
  }
  LOG(INFO) << "json_value: " << json_value;
}

void Util::PrintProtoMessage(const google::protobuf::Message &msg,
                             string *json) {
  JsonPrintOptions option;

  if (!MessageToJsonString(msg, json, option).ok()) {
    LOG(ERROR) << "to json string failed";
  }
}

bool Util::JsonToMessage(const std::string &json,
                         google::protobuf::Message *msg) {
  JsonParseOptions option;
  option.case_insensitive_enum_parsing = false;
  option.ignore_unknown_fields = true;
  if (!google::protobuf::util::JsonStringToMessage(json, msg).ok()) {
    LOG(ERROR) << "json string to msg failed";
    return false;
  }
  return true;
}

void Util::Replace(const string &from, const string &to, string *str) {
  size_t start_pos = str->find(from);
  if (start_pos == std::string::npos) {
    return;
  }
  str->replace(start_pos, from.length(), to);
}

bool Util::ConfDiff(const std::filesystem::path &origin_file,
                    const std::filesystem::path &final_file, std::string *out) {
  if (!std::filesystem::exists(origin_file)) {
    *out = origin_file.string() + " replace error, origin file not exists";
    return false;
  }

  if (!std::filesystem::exists(final_file)) {
    *out = final_file.string() + " replace error, final file not exists";
    return false;
  }

  std::vector<std::string> origin_lines;
  std::vector<std::string> final_lines;

  std::ifstream origin(origin_file);
  if (origin.is_open()) {
    std::string line;
    while (std::getline(origin, line)) {
      origin_lines.push_back(line);
    }
    origin.close();
  } else {
    *out = origin_file.string() + " replace error, origin file open failed";
    return false;
  }

  std::ifstream fi(final_file);
  if (fi.is_open()) {
    std::string line;
    while (std::getline(fi, line)) {
      final_lines.push_back(line);
    }
    fi.close();
  } else {
    *out = origin_file.string() + " replace error, final file open failed";
    return false;
  }

  if (origin_lines.size() != final_lines.size()) {
    *out = origin_file.string() + " replace error, line num not equal";
    return false;
  }

  std::string name = final_file.filename();
  boost::algorithm::replace_all(name, ".", "_");
  std::stringstream buff;

  buff << "template_rule(" << std::endl;
  buff << "    name = \"" << name << "\"," << std::endl;
  buff << "src= \"" << origin_file.string() << "\"," << std::endl;
  buff << "out = \"" << final_file.string() << "\"," << std::endl;
  buff << "    substitutions = {" << std::endl;

  for (size_t i = 0; i < origin_lines.size(); ++i) {
    // if (origin_lines[i].empty() || origin_lines[i][0] != '#') {
    // continue;
    //}
    if (origin_lines[i] == final_lines[i]) {
      continue;
    }
    boost::algorithm::replace_all(origin_lines[i], "\\", "\\\\");
    boost::algorithm::replace_all(final_lines[i], "\\", "\\\\");

    boost::algorithm::replace_all(origin_lines[i], "\"", "\\\"");
    boost::algorithm::replace_all(final_lines[i], "\"", "\\\"");
    buff << "\"" << origin_lines[i] << "\""
         << ": "
         << "\"" << final_lines[i] << "\"," << std::endl;
  }

  buff << "}," << std::endl;
  buff << ")" << std::endl;

  *out = buff.str();
  return true;
}

bool Util::ConfReplace(const std::filesystem::path &origin_file,
                       const std::filesystem::path &final_file,
                       std::string *out) {
  if (!std::filesystem::exists(origin_file)) {
    *out = origin_file.string() + " replace error, origin file not exists";
    return false;
  }

  if (!std::filesystem::exists(final_file)) {
    *out = final_file.string() + " replace error, final file not exists";
    return false;
  }

  std::vector<std::string> origin_lines;
  std::vector<std::string> final_lines;

  std::ifstream origin(origin_file);
  if (origin.is_open()) {
    std::string line;
    while (std::getline(origin, line)) {
      origin_lines.push_back(line);
    }
    origin.close();
  } else {
    *out = origin_file.string() + " replace error, origin file open failed";
    return false;
  }

  std::ifstream fi(final_file);
  if (fi.is_open()) {
    std::string line;
    while (std::getline(fi, line)) {
      final_lines.push_back(line);
    }
    fi.close();
  } else {
    *out = origin_file.string() + " replace error, final file open failed";
    return false;
  }

  if (origin_lines.size() != final_lines.size()) {
    *out = origin_file.string() + " replace error, line num not equal";
    return false;
  }

  std::string name = final_file.filename();
  boost::algorithm::replace_all(name, ".", "_");
  std::stringstream buff;

  buff << "template_rule(" << std::endl;
  buff << "    name = \"" << name << "\"," << std::endl;
  buff << "src= \"" << origin_file.string() << "\"," << std::endl;
  buff << "out = \"" << final_file.string() << "\"," << std::endl;
  buff << "    substitutions = {" << std::endl;

  for (size_t i = 0; i < origin_lines.size(); ++i) {
    if (origin_lines[i].empty() || origin_lines[i][0] != '#') {
      continue;
    }
    boost::algorithm::replace_all(origin_lines[i], "\\", "\\\\");
    boost::algorithm::replace_all(final_lines[i], "\\", "\\\\");

    boost::algorithm::replace_all(origin_lines[i], "\"", "\\\"");
    boost::algorithm::replace_all(final_lines[i], "\"", "\\\"");
    buff << "\"" << origin_lines[i] << "\""
         << ": "
         << "\"" << final_lines[i] << "\"," << std::endl;
  }

  buff << "}," << std::endl;
  buff << ")" << std::endl;

  *out = buff.str();
  return true;
}

bool Util::ConfGen(const std::filesystem::path &origin_file, std::string *out) {
  if (!std::filesystem::exists(origin_file)) {
    *out = origin_file.string() + " gen error, file not exists";
    return false;
  }

  std::vector<std::string> origin_lines;
  std::ifstream origin(origin_file);
  if (origin.is_open()) {
    std::string line;
    while (std::getline(origin, line)) {
      origin_lines.push_back(line);
    }
    origin.close();
  } else {
    *out = origin_file.string() + " gen error, open failed";
    return false;
  }

  std::string name = origin_file.filename();
  boost::algorithm::replace_all(name, ".", "_");
  std::stringstream buff;

  buff << "genrule(" << std::endl;
  buff << "    name = \"" << name << "\"," << std::endl;
  buff << "outs = [\"" << origin_file.string() << "\"]," << std::endl;
  buff << "    cmd = \"\\n\".join([" << std::endl;
  buff << "\"cat <<'EOF' >$@\"," << std::endl;

  for (size_t i = 0; i < origin_lines.size(); ++i) {
    boost::algorithm::replace_all(origin_lines[i], "\\", "\\\\");
    boost::algorithm::replace_all(origin_lines[i], "\"", "\\\"");
    buff << "\"" << origin_lines[i] << "\"," << std::endl;
  }

  buff << "\"EOF\"," << std::endl;
  buff << "    ])," << std::endl;
  buff << ")" << std::endl;

  *out = buff.str();
  return true;
}

}  // namespace util
}  // namespace bazel_template
