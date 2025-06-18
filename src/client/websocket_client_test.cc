/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "src/client/websocket_client.h"

#include <chrono>
#include <thread>

#include "gtest/gtest.h"
#include "src/proto/service.pb.h"
#include "src/util/util.h"

namespace bazel_template {
namespace client {
namespace {

class WebSocketClientTest : public ::testing::Test {
 protected:
  void SetUp() override {
    client_ = std::make_unique<WebSocketClient>("127.0.0.1", "10003");
  }

  void TearDown() override {
    if (client_) {
      client_->Stop();
    }
  }

  std::unique_ptr<WebSocketClient> client_;
};

TEST_F(WebSocketClientTest, ConnectAndDisconnect) {
  EXPECT_NO_THROW(client_->Connect());
  std::this_thread::sleep_for(std::chrono::seconds(1));
}

TEST_F(WebSocketClientTest, SendLoadFolderRequest) {
  EXPECT_NO_THROW(client_->Connect());

  // Create LoadFolderRequest
  proto::LoadFolderRequest req;
  req.set_folder_id("test_folder_123");

  // Convert to JSON
  std::string json_str;
  if (!util::Util::MessageToJson(req, &json_str)) {
    FAIL() << "Failed to convert LoadFolderRequest to JSON";
  }

  // Assemble protocol message
  std::string protocol_msg;
  protocol_msg.reserve(4 + json_str.size());  // op_code(4) + json

  // Add op_code (OP_LOAD_FOLDER) in network byte order
  uint32_t op_code =
      htonl(static_cast<uint32_t>(proto::OpCode::OP_LOAD_FOLDER));
  protocol_msg.append(reinterpret_cast<char*>(&op_code), sizeof(op_code));

  // Add JSON payload
  protocol_msg.append(json_str);

  // Send the message
  client_->SendMessage(protocol_msg);

  // Give some time for the message to be sent
  std::this_thread::sleep_for(std::chrono::seconds(30));
}

}  // namespace
}  // namespace client
}  // namespace bazel_template
