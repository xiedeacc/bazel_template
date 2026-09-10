/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

// bazel_template.client.websocket_client. The boost.beast stream and the
// folly executor live in Impl (websocket_client.cc): neither library may
// appear in a module interface (gcc cannot merge such a BMI).
module;

#include <atomic>
#include <memory>
#include <string>
#include <string_view>
#include <thread>

#include "src/common/blocking_queue.h"

export module bazel_template.client.websocket_client;

export namespace bazel_template::client {

class WebSocketClient {
 public:
  WebSocketClient(std::string_view host, std::string_view port);
  ~WebSocketClient();

  WebSocketClient(const WebSocketClient&) = delete;
  WebSocketClient& operator=(const WebSocketClient&) = delete;
  WebSocketClient(WebSocketClient&&) = delete;
  WebSocketClient& operator=(WebSocketClient&&) = delete;

  void Connect();
  void Start();
  void Stop();
  void SendMessage(const std::string& message, bool is_binary = true);

 private:
  void ProcessMessages();
  static void HandleMessage(std::string_view msg);
  void ReadMessage();

  std::string host_;
  std::string port_;
  struct Impl;
  std::unique_ptr<Impl> impl_;
  common::BlockingQueue<std::string> message_queue_;
  std::atomic<bool> running_;
  std::jthread io_thread_;
};

}  // namespace bazel_template::client
