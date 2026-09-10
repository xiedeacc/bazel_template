/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

module;

#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <exception>
#include <memory>
#include <string>
#include <string_view>
#include <thread>
#include <utility>

#include "boost/asio/connect.hpp"
#include "boost/asio/io_context.hpp"
#include "boost/asio/ip/tcp.hpp"
#include "boost/beast/core/buffers_to_string.hpp"
#include "boost/beast/websocket/stream.hpp"
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif
#include "folly/executors/CPUThreadPoolExecutor.h"
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif
#include "glog/logging.h"
#include "src/common/blocking_queue.h"
#include "src/common/socket_compat.h"
#include "src/proto/enums.pb.h"

// windows.h (through boost.asio) defines SendMessage as a macro; the member
// below must keep its name, as declared in the interface.
#ifdef SendMessage
#undef SendMessage
#endif

module bazel_template.client.websocket_client;

namespace bazel_template::client {

struct WebSocketClient::Impl {
  Impl()
      : ws_(ioc_),
        thread_pool_(std::make_shared<folly::CPUThreadPoolExecutor>(2)) {}

  boost::asio::io_context ioc_;
  boost::beast::websocket::stream<boost::asio::ip::tcp::socket> ws_;
  std::shared_ptr<folly::CPUThreadPoolExecutor> thread_pool_;
  boost::beast::flat_buffer read_buffer_;
};

WebSocketClient::WebSocketClient(std::string_view host, std::string_view port)
    : host_(host),
      port_(port),
      impl_(std::make_unique<Impl>()),
      running_(false) {}

WebSocketClient::~WebSocketClient() {
  try {
    Stop();
  } catch (...) {  // NOLINT(bugprone-empty-catch)
  }
}

void WebSocketClient::Connect() {
  try {
    boost::asio::ip::tcp::resolver resolver(impl_->ioc_);
    auto const results = resolver.resolve(host_, port_);
    boost::asio::connect(impl_->ws_.next_layer(), results.begin(),
                         results.end());
    impl_->ws_.set_option(boost::beast::websocket::stream_base::decorator(
        [](boost::beast::websocket::request_type& req) {
          req.set(boost::beast::http::field::sec_websocket_version, "13");
        }));
    impl_->ws_.read_message_max(std::size_t{100} * 1024 * 1024);
    impl_->ws_.handshake(host_, "/api/v1/folder/load");
    LOG(INFO) << "Connected to WebSocket server at " << host_ << ":" << port_;
    ReadMessage();
    running_ = true;
    Start();
  } catch (const std::exception& e) {
    LOG(ERROR) << "Connection error: " << e.what();
    throw;
  }
}

void WebSocketClient::Start() {
  io_thread_ = std::jthread([this]() {
    try {
      LOG(INFO) << "I/O service thread started";
      while (running_) {
        impl_->ioc_.run();
      }
      LOG(INFO) << "I/O service thread finished";
    } catch (const std::exception& e) {
      LOG(ERROR) << "I/O service error: " << e.what();
    }
  });
  for (int i = 0; i < 2; ++i) {
    impl_->thread_pool_->add([this]() {
      try {
        ProcessMessages();
      } catch (const std::exception& e) {
        LOG(ERROR) << "Message processing error: " << e.what();
      }
    });
  }
  LOG(INFO) << "WebSocket client started with 2 message processing threads";
}

void WebSocketClient::Stop() {
  if (!running_) {
    return;
  }
  LOG(INFO) << "Stopping WebSocket client...";
  running_ = false;
  message_queue_.Close();
  LOG(INFO) << "Message queue closed";
  if (impl_->ws_.is_open()) {
    try {
      LOG(INFO) << "Closing WebSocket with status 1000";
      impl_->ws_.async_close(
          boost::beast::websocket::close_code(1000),
          [](boost::beast::error_code ec) {
            if (ec && ec != boost::beast::websocket::error::closed) {
              LOG(ERROR) << "Error in async_close: " << ec.message();
            }
          });
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    } catch (const std::exception& e) {
      LOG(ERROR) << "Error closing WebSocket: " << e.what();
    }
  }
  impl_->ioc_.stop();
  LOG(INFO) << "I/O service stopped";
  if (io_thread_.joinable()) {
    io_thread_.join();
  }
  impl_->thread_pool_->stop();
  LOG(INFO) << "Thread pool stopped";
  LOG(INFO) << "WebSocket client stopped";
}

void WebSocketClient::SendMessage(const std::string& message, bool is_binary) {
  if (!impl_->ws_.is_open()) {
    LOG(ERROR) << "WebSocket is not open";
    return;
  }
  try {
    impl_->ws_.binary(is_binary);
    impl_->ws_.async_write(
        boost::asio::buffer(message),
        [](boost::beast::error_code ec, std::size_t bytes_transferred) {
          if (ec) {
            LOG(ERROR) << "Write error: " << ec.message();
            return;
          }
          LOG(INFO) << "Message sent successfully, size: " << bytes_transferred;
        });
  } catch (const std::exception& e) {
    LOG(ERROR) << "Error sending message: " << e.what();
  }
}

void WebSocketClient::ProcessMessages() {
  LOG(INFO) << "Message processing thread started";
  while (running_) {
    try {
      std::string msg;
      if (message_queue_.TryPop(msg)) {
        HandleMessage(msg);
      }
    } catch (const std::exception& e) {
      LOG(ERROR) << "Error processing message: " << e.what();
    }
  }
  LOG(INFO) << "Message processing thread finished";
}

void WebSocketClient::HandleMessage(std::string_view msg) {
  LOG(INFO) << "Handling message";
  if (msg.size() < sizeof(uint32_t)) {
    LOG(ERROR) << "Message too short to contain op_code";
    return;
  }
  uint32_t network_order = 0;
  std::memcpy(&network_order, msg.data(), sizeof(network_order));
  const uint32_t op_code = ntohl(network_order);
  using enum proto::OpCode;
  switch (static_cast<proto::OpCode>(op_code)) {
    case OP_UNUSED:
      LOG(INFO) << "Received OP_UNUSED message";
      break;
    case OP_FILE_KEY:
      LOG(INFO) << "Received OP_FILE_KEY message";
      break;
    case OP_FILE_UPLOAD:
      LOG(INFO) << "Received OP_FILE_UPLOAD message";
      break;
    case OP_EC2_START:
      LOG(INFO) << "Received OP_EC2_START message";
      break;
    case OP_EC2_STOP:
      LOG(INFO) << "Received OP_EC2_STOP message";
      break;
    case OP_ROUTE53_UPDATE_A_RECORD:
      LOG(INFO) << "Received OP_ROUTE53_UPDATE_A_RECORD message";
      break;
    case OP_ROUTE53_UPDATE_CNAME_RECORD:
      LOG(INFO) << "Received OP_ROUTE53_UPDATE_CNAME_RECORD message";
      break;
    default:
      LOG(WARNING) << "Unknown op code: " << op_code;
      break;
  }
}

void WebSocketClient::ReadMessage() {
  impl_->ws_.async_read(
      impl_->read_buffer_,
      [this](boost::beast::error_code ec, std::size_t /*bytes_transferred*/) {
        if (ec) {
          if (ec == boost::beast::websocket::error::closed) {
            LOG(INFO) << "WebSocket closed by server";
            running_ = false;
            impl_->ioc_.stop();
          } else {
            LOG(ERROR) << "Read error: " << ec.message();
          }
          return;
        }
        try {
          std::string msg =
              boost::beast::buffers_to_string(impl_->read_buffer_.data());
          LOG(INFO) << "Received message of size: " << msg.size();
          message_queue_.Push(std::move(msg));
          impl_->read_buffer_.consume(impl_->read_buffer_.size());
          if (running_) {
            ReadMessage();
          }
        } catch (const std::exception& e) {
          LOG(ERROR) << "Error processing received message: " << e.what();
          if (running_) {
            ReadMessage();
          }
        }
      });
}

}  // namespace bazel_template::client
