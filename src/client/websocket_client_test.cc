/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include <chrono>
#include <exception>
#include <thread>

#include "gtest/gtest.h"
#include "src/proto/service.pb.h"

import bazel_template.client.websocket_client;

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

// Nothing listens on the test port: Connect() must report that as an
// exception rather than hang or crash, and Stop() must then be safe to call.
// Pointing host/port at a running server turns this into a live smoke test.
TEST_F(WebSocketClientTest, ConnectFailureIsReported) {
  EXPECT_THROW(client_->Connect(), std::exception);
  EXPECT_NO_THROW(client_->Stop());
}

}  // namespace
}  // namespace client
}  // namespace bazel_template
