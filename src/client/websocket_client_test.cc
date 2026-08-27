/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "src/client/websocket_client.h"

#include <chrono>
#include <future>
#include <memory>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "folly/SocketAddress.h"
#include "folly/io/async/AsyncSocketBase.h"
#include "gtest/gtest.h"
#include "proxygen/httpserver/HTTPServer.h"
#include "proxygen/httpserver/HTTPServerOptions.h"
#include "proxygen/httpserver/RequestHandlerFactory.h"
#include "src/proto/service.pb.h"
#include "src/server/http_handler/http_handler_factory.h"
#include "src/util/util.h"

namespace bazel_template {
namespace client {
namespace {

// Brings up an in-process HTTP server carrying the real WebSocket handler, so
// the test is self-contained and does not need //src/server:server running.
class WebSocketClientTest : public ::testing::Test {
 protected:
  void SetUp() override {
    proxygen::HTTPServerOptions options;
    options.threads = 1;
    options.idleTimeout = std::chrono::milliseconds(60000);
    options.handlerFactories =
        proxygen::RequestHandlerChain()
            .addThen<server::http_handler::HTTPHandlerFactory>()
            .build();

    server_ = std::make_unique<proxygen::HTTPServer>(std::move(options));

    // Port 0 lets the OS pick a free port, so concurrent test runs on the same
    // machine cannot collide on a fixed one.
    std::vector<proxygen::HTTPServer::IPConfig> ips{
        {folly::SocketAddress("127.0.0.1", 0, true),
         proxygen::HTTPServer::Protocol::HTTP}};
    server_->bind(std::move(ips));

    // Shared, not captured by reference: the server thread outlives SetUp().
    auto started = std::make_shared<std::promise<void>>();
    auto started_future = started->get_future();
    server_thread_ = std::thread([this, started]() {
      server_->start([started]() { started->set_value(); });
    });
    ASSERT_EQ(started_future.wait_for(std::chrono::seconds(30)),
              std::future_status::ready)
        << "in-process HTTP server failed to start";

    const auto sockets = server_->getSockets();
    ASSERT_FALSE(sockets.empty());
    folly::SocketAddress bound;
    sockets[0]->getAddress(&bound);

    client_ = std::make_unique<WebSocketClient>(
        "127.0.0.1", std::to_string(bound.getPort()));
  }

  void TearDown() override {
    if (client_) {
      client_->Stop();
      client_.reset();
    }
    if (server_) {
      server_->stop();
    }
    if (server_thread_.joinable()) {
      server_thread_.join();
    }
    server_.reset();
  }

  std::unique_ptr<proxygen::HTTPServer> server_;
  std::thread server_thread_;
  std::unique_ptr<WebSocketClient> client_;
};

TEST_F(WebSocketClientTest, ConnectAndDisconnect) {
  EXPECT_NO_THROW(client_->Connect());
  std::this_thread::sleep_for(std::chrono::seconds(1));
}

}  // namespace
}  // namespace client
}  // namespace bazel_template
