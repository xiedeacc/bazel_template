/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef BAZEL_TEMPLATE_COMMON_HOST_SPEC_H
#define BAZEL_TEMPLATE_COMMON_HOST_SPEC_H

#include <string>

#include "src/common/ip_address.h"

namespace bazel_template {
namespace common {

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

}  // namespace common
}  // namespace bazel_template

#endif  // BAZEL_TEMPLATE_COMMON_HOST_SPEC_H
