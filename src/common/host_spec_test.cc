/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "src/common/host_spec.h"

#include "gtest/gtest.h"

namespace bazel_template {
namespace common {

TEST(HostSpec, Equal) {
  HostSpec host_spec1("1.2.3.4", 56);
  HostSpec host_spec2("1.2.3.4", 56);
  HostSpec host_spec3("2.2.3.4", 56);
  HostSpec host_spec4("1.2.3.4", 567);
  EXPECT_TRUE(host_spec1 == host_spec2);
  EXPECT_FALSE(host_spec1 == host_spec3);
  EXPECT_FALSE(host_spec1 == host_spec4);
}

TEST(HostSpec, ToString) {
  HostSpec host_spec("1.2.3.4", 56);
  EXPECT_EQ("1.2.3.4", host_spec.GetIPAddress());
  EXPECT_EQ(56, host_spec.GetPort());
  EXPECT_EQ("1.2.3.4:56", host_spec.ToString());
}

TEST(HostSpec, FromString) {
  HostSpec host_spec;
  EXPECT_EQ("0.0.0.0", host_spec.GetIPAddress());
  EXPECT_EQ(0, host_spec.GetPort());

  EXPECT_TRUE(host_spec.FromString("1.2.3.4:56"));
  EXPECT_EQ("1.2.3.4", host_spec.GetIPAddress());
  EXPECT_EQ(56, host_spec.GetPort());
}

TEST(HostSpec, FromBadString) {
  HostSpec host_spec;
  EXPECT_EQ("0.0.0.0", host_spec.GetIPAddress());
  EXPECT_EQ(0, host_spec.GetPort());

  // from empty string
  EXPECT_FALSE(host_spec.FromString(""));
  EXPECT_EQ("0.0.0.0", host_spec.GetIPAddress());
  EXPECT_EQ(0, host_spec.GetPort());

  // missing part
  EXPECT_FALSE(host_spec.FromString(":56"));
  EXPECT_EQ("0.0.0.0", host_spec.GetIPAddress());
  EXPECT_EQ(0, host_spec.GetPort());
  EXPECT_FALSE(host_spec.FromString("1.2.3.4:"));
  EXPECT_EQ("0.0.0.0", host_spec.GetIPAddress());
  EXPECT_EQ(0, host_spec.GetPort());

  // bad ip
  EXPECT_FALSE(host_spec.FromString("2.3.4:56"));
  EXPECT_EQ("0.0.0.0", host_spec.GetIPAddress());
  EXPECT_EQ(0, host_spec.GetPort());
  EXPECT_FALSE(host_spec.FromString(".2.3.4:56"));
  EXPECT_EQ("0.0.0.0", host_spec.GetIPAddress());
  EXPECT_EQ(0, host_spec.GetPort());
  EXPECT_FALSE(host_spec.FromString("abcde.2.3.4:56"));
  EXPECT_EQ("0.0.0.0", host_spec.GetIPAddress());
  EXPECT_EQ(0, host_spec.GetPort());
  EXPECT_FALSE(host_spec.FromString("257.2.3.4:56"));
  EXPECT_EQ("0.0.0.0", host_spec.GetIPAddress());
  EXPECT_EQ(0, host_spec.GetPort());

  // bad port
  EXPECT_FALSE(host_spec.FromString("1.2.3.4:-123456"));
  EXPECT_EQ("0.0.0.0", host_spec.GetIPAddress());
  EXPECT_EQ(0, host_spec.GetPort());
  EXPECT_FALSE(host_spec.FromString("1.2.3.4:abcdef"));
  EXPECT_EQ("0.0.0.0", host_spec.GetIPAddress());
  EXPECT_EQ(0, host_spec.GetPort());
}

}  // namespace common
}  // namespace bazel_template
