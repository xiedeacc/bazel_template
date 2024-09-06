/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "src/common/ip_address.h"

#include "gtest/gtest.h"

using std::string;
using std::vector;

namespace bazel_template {
namespace common {

TEST(IPAddress, Ctor) {
  EXPECT_NO_THROW(IPAddress ip("1.2.3.4"));
  EXPECT_NO_THROW(IPAddress ip(string("1.2.3.4")));
  EXPECT_ANY_THROW(IPAddress ip("1.256.3.4"));
  EXPECT_ANY_THROW(IPAddress ip(string("1.2.3.4x")));
}

TEST(IPAddress, Parse) {
  EXPECT_TRUE(IPAddress("1.2.3.4") == IPAddress(1, 2, 3, 4));
  IPAddress address;
  EXPECT_FALSE(address.Assign("-1.2.3.4"));
  EXPECT_FALSE(address.Assign("1.2.3.4x"));
  EXPECT_FALSE(address.Assign("1.256.3.4"));
  EXPECT_TRUE(address.Assign("67.220.91.0017"));
  EXPECT_EQ("67.220.91.15", address.ToString());
}

TEST(IPAddress, Bytes) {
  IPAddress ip("1.2.3.4");
  EXPECT_EQ(1, ip.Bytes()[0]);
  EXPECT_EQ(2, ip.Bytes()[1]);
  EXPECT_EQ(3, ip.Bytes()[2]);
  EXPECT_EQ(4, ip.Bytes()[3]);
}

TEST(IPAddress, ByteOrder) {
  IPAddress ip("192.168.0.1");
  EXPECT_EQ(static_cast<in_addr_t>(192 + (168 << 8) + (0 << 16) + (1 << 24)),
            ip.ToInt());
  EXPECT_EQ(static_cast<in_addr_t>((192 << 24) + (168 << 16) + (0 << 8) + 1),
            ip.ToLocalInt());
}

TEST(IPAddress, CompareToSystemParsing) {
  IPAddress ip("192.168.0.1");
  EXPECT_EQ(inet_addr("192.168.0.1"), ip.ToInt());
}

TEST(IPAddress, WellKnown) {
  EXPECT_TRUE(IPAddress("0.0.0.0") == IPAddress::Any());
  EXPECT_TRUE(IPAddress("127.0.0.1") == IPAddress::Loopback());
  EXPECT_TRUE(IPAddress("255.255.255.255") == IPAddress::Broadcast());
  EXPECT_TRUE(IPAddress("255.255.255.255") == IPAddress::None());
}

TEST(IPAddress, Compare) {
  EXPECT_TRUE(IPAddress("0.0.0.0") == IPAddress("0.0.0.0"));
  EXPECT_TRUE(IPAddress("0.0.0.0") != IPAddress("0.0.0.1"));
  EXPECT_TRUE(IPAddress("0.0.0.0") < IPAddress("0.0.0.1"));
  EXPECT_TRUE(IPAddress("1.0.0.0") > IPAddress("0.1.1.1"));

  EXPECT_TRUE(IPAddress("0.0.0.1") >= IPAddress("0.0.0.0"));
  EXPECT_TRUE(IPAddress("0.0.0.0") >= IPAddress("0.0.0.0"));
  EXPECT_TRUE(IPAddress("0.0.0.0") >= IPAddress("0.0.0.0"));

  EXPECT_TRUE(IPAddress("0.0.0.0") <= IPAddress("0.0.0.0"));
  EXPECT_TRUE(IPAddress("0.0.0.0") <= IPAddress("0.0.0.1"));
}

TEST(IPAddress, Type) {
  EXPECT_TRUE(IPAddress("0.0.0.255").IsBroadcast());
  EXPECT_TRUE(!IPAddress("255.0.0.0").IsBroadcast());

  EXPECT_TRUE(IPAddress("127.0.0.1").IsLoopback());
  EXPECT_TRUE(IPAddress("127.1.1.1").IsLoopback());
  EXPECT_TRUE(!IPAddress("128.0.0.1").IsLoopback());

  EXPECT_TRUE(IPAddress("10.0.0.1").IsPrivate());
  EXPECT_TRUE(IPAddress("192.168.0.1").IsPrivate());
  EXPECT_TRUE(IPAddress("172.16.0.1").IsPrivate());
  EXPECT_TRUE(IPAddress("172.31.0.1").IsPrivate());
  EXPECT_TRUE(!IPAddress("172.11.0.1").IsPrivate());
  EXPECT_TRUE(!IPAddress("172.32.0.1").IsPrivate());
}

TEST(IPAddress, GetAddressList) {
  vector<IPAddress> ip_list;
  IPAddress address;
  EXPECT_EQ(IPAddress::GetLocalList(&ip_list),
            IPAddress::GetFirstLocalAddress(&address));
  EXPECT_EQ(IPAddress::GetPublicList(&ip_list),
            IPAddress::GetFirstPublicAddress(&address));
  EXPECT_EQ(IPAddress::GetPrivateList(&ip_list),
            IPAddress::GetFirstPrivateAddress(&address));
}

}  // namespace common
}  // namespace bazel_template
