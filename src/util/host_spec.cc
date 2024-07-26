/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "src/util/host_spec.h"

#include "absl/strings/numbers.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_split.h"

using absl::AlphaNum;
using absl::ByChar;
using absl::SimpleAtoi;
using absl::StrAppend;
using absl::StrSplit;
using std::exception;
using std::string;
using std::vector;

namespace bazel_template {
namespace util {

HostSpec::HostSpec() : port_(0) {}

HostSpec::HostSpec(const string &ip, uint16_t port) : ip_(ip), port_(port) {}

bool HostSpec::operator==(const HostSpec &other) const {
  return port_ == other.port_ && ip_ == other.ip_;
}

const string HostSpec::GetIPAddress() const { return ip_.ToString(); }

uint16_t HostSpec::GetPort() const { return port_; }

const string HostSpec::ToString() const {
  string result = ip_.ToString();
  result += ":";
  StrAppend(&result, AlphaNum(port_));
  return result;
}

bool HostSpec::FromString(const string &ip_port) {
  vector<string> splited_ip_port = StrSplit(ip_port, ByChar(':'));
  if (splited_ip_port.size() != 2) {
    return false;
  }

  try {
    IPAddress ip(splited_ip_port[0]);
    uint32_t port;
    if (!SimpleAtoi(splited_ip_port[1], &port) || port > 65535) {
      return false;
    }
    port_ = port;
    ip_ = ip;
  } catch (const exception &ex) {
    return false;
  }

  return true;
}

HostSpecSameWith::HostSpecSameWith(const HostSpec &host) : host_(host) {}

bool HostSpecSameWith::operator()(const HostSpec &host) {
  return host_ == host;
}

}  // namespace util
}  // namespace bazel_template
