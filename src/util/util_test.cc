/*******************************************************************************
 * Copyright (c) 2023  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "src/util/util.h"

#include "glog/logging.h"
#include "gtest/gtest.h"

using std::map;
using std::string;
using std::vector;

namespace bazel_template {
namespace util {

TEST(Util, LoadContent) {
  auto content =
      Util::LoadContent(string("test_data/config_test/base_config.json"));
  LOG(INFO) << content;
}

TEST(Util, GetServerIp) {
  auto ip_addr = Util::GetServerIp();
  LOG(INFO) << ip_addr;
  ASSERT_EQ("192.168.220.128", ip_addr);
}

TEST(Util, ListDir) {
  vector<string> files;
  EXPECT_EQ(true, Util::ListDir("test_data/config_test", &files));
  EXPECT_EQ(2, files.size());
}

TEST(Util, ToInt) {
  auto str = "500";
  uint32_t value;
  ASSERT_EQ(true, Util::ToInt(str, &value));
  ASSERT_EQ(500, value);
}

TEST(Util, FileMd5) {
  std::string md5_value =
      Util::FileMd5("test_data/config_test/base_config.json");
  std::string real_md5 = "769ab1652caae8e5afa39f7dcad361b6";
  LOG(INFO) << "real md5: " << real_md5 << ", got md5: " << md5_value;
  EXPECT_EQ(md5_value, real_md5);
}

TEST(Util, Base64EncodeDecode) {
  std::string input = "Send reinforcements";
  std::string encode_output = Util::Base64Encode(input);
  EXPECT_EQ(encode_output, "U2VuZCByZWluZm9yY2VtZW50cw==");
  EXPECT_EQ(encode_output.size(),
            std::string("U2VuZCByZWluZm9yY2VtZW50cw==").size());

  std::string decode_output = Util::Base64Decode(encode_output);
  EXPECT_EQ(decode_output, input);
}

TEST(Util, HexStrToUInt64) {
  uint64_t qid = Util::HexStrToUInt64("2eb568d3e24a5e69");
  LOG(INFO) << "qid: " << qid;
  EXPECT_EQ(qid, 3365711555773226601);
}

TEST(Util, UInt64ToHexStr) {
  string sid = Util::UInt64ToHexStr(6171972363329882614);
  LOG(INFO) << "sid: " << sid;
  EXPECT_EQ(sid, "55a74029f7ce49f6");

  sid = Util::UInt64ToHexStr(363455392779079689);
  LOG(INFO) << "sid: " << sid;
  EXPECT_EQ(sid, "050b40c10a4c1009");

  sid = Util::UInt64ToHexStr(5573924287409398148);
  LOG(INFO) << "sid: " << sid;
  EXPECT_EQ(sid, "4d5a8e9b83f5a184");
}

TEST(Util, StrTimeToTimestamp) {
  auto timestamp = Util::StrTimeToTimestamp("2023-10-08T16:34:31.000+0000");
  LOG(INFO) << "timestamp: " << timestamp;
  EXPECT_EQ(timestamp, 1696782871000);

  timestamp = Util::StrTimeToTimestamp("2023-10-08T16:34:31.000+0800");
  LOG(INFO) << "timestamp: " << timestamp;
  EXPECT_EQ(timestamp, 1696754071000);

  timestamp =
      Util::StrTimeToTimestamp("2023-10-08T16:34:31.000+0000", -28800000);
  LOG(INFO) << "timestamp: " << timestamp;
  EXPECT_EQ(timestamp, 1696754071000);
}

}  // namespace util
}  // namespace bazel_template
