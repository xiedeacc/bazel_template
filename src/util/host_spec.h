/*******************************************************************************
 * Copyright (c) 2023  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/
#pragma once

#include "src/util/ip_address.h"

namespace bazel_template {
namespace util {

class HostSpec {
 public:
  HostSpec();
  HostSpec(const std::string &ip, uint16_t port);

  bool operator==(const HostSpec &other) const;

  const std::string GetIPAddress() const;
  uint16_t GetPort() const;

  const std::string ToString() const;
  bool FromString(const std::string &ip_port);

 private:
  IPAddress ip_;
  uint16_t port_;
};  // class HostSpec

struct HostSpecSameWith {
  explicit HostSpecSameWith(const HostSpec &host);
  bool operator()(const HostSpec &host);

 private:
  HostSpec host_;
};

}  // namespace util
}  // namespace bazel_template
