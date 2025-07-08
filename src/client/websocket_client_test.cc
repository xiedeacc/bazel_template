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

}  // namespace
}  // namespace client
}  // namespace bazel_template
