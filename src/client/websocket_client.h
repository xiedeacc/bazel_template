/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef BAZEL_TEMPLATE_CLIENT_WEBSOCKET_CLIENT_H_
#define BAZEL_TEMPLATE_CLIENT_WEBSOCKET_CLIENT_H_

#include <memory>
#include <string>

#include "boost/asio/connect.hpp"
#include "boost/asio/io_context.hpp"
#include "boost/asio/ip/tcp.hpp"
#include "boost/beast/core/buffers_to_string.hpp"
#include "boost/beast/websocket/stream.hpp"
#include "folly/executors/CPUThreadPoolExecutor.h"
#include "glog/logging.h"
#include "src/client/handler/load_folder_handler.h"
#include "src/common/blocking_queue.h"
#include "src/proto/enums.pb.h"

namespace bazel_template {
namespace client {

class WebSocketClient {
 public:
  WebSocketClient(const std::string& host, const std::string& port)
      : host_(host),
        port_(port),
        ioc_(),
        ws_(ioc_),
        message_queue_(),
        running_(false),
        thread_pool_(std::make_shared<folly::CPUThreadPoolExecutor>(2)) {}

  ~WebSocketClient() { Stop(); }

  void Connect() {
    try {
      // Resolve the host
      boost::asio::ip::tcp::resolver resolver(ioc_);
      auto const results = resolver.resolve(host_, port_);

      // Connect to the server
      boost::asio::connect(ws_.next_layer(), results.begin(), results.end());

      // Set WebSocket options
      ws_.set_option(boost::beast::websocket::stream_base::decorator(
          [](boost::beast::websocket::request_type& req) {
            req.set(boost::beast::http::field::sec_websocket_version, "13");
          }));

      // Set message size limit (100MB)
      ws_.read_message_max(100 * 1024 * 1024);

      // Perform the WebSocket handshake with the correct API path
      ws_.handshake(host_, "/api/v1/folder/load");

      LOG(INFO) << "Connected to WebSocket server at " << host_ << ":" << port_;

      // Start reading messages
      ReadMessage();

      // Start the I/O service and message processing
      running_ = true;

      Start();

    } catch (const std::exception& e) {
      LOG(ERROR) << "Connection error: " << e.what();
      throw;
    }
  }

  void Start() {
    // Start the I/O service in a single thread
    std::thread io_thread([this]() {
      try {
        LOG(INFO) << "I/O service thread started";
        while (running_) {
          ioc_.run();
        }
        LOG(INFO) << "I/O service thread finished";
      } catch (const std::exception& e) {
        LOG(ERROR) << "I/O service error: " << e.what();
      }
    });
    io_thread.detach();  // Let it run independently

    // Submit message processing to thread pool,
    // thread number use config may be better, but for now, just use 2 threads
    for (int i = 0; i < 2; ++i) {
      thread_pool_->add([this]() {
        try {
          ProcessMessages();
        } catch (const std::exception& e) {
          LOG(ERROR) << "Message processing error: " << e.what();
        }
      });
    }

    // Give threads time to start
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    LOG(INFO) << "WebSocket client started with 2 message processing threads";
  }

  void Stop() {
    if (!running_) return;

    LOG(INFO) << "Stopping WebSocket client...";

    // First stop accepting new messages
    running_ = false;
    message_queue_.Close();
    LOG(INFO) << "Message queue closed";

    // Close WebSocket
    if (ws_.is_open()) {
      try {
        // Use async_close to properly handle the close handshake
        LOG(INFO) << "Closing WebSocket with status 1000";
        ws_.async_close(
            boost::beast::websocket::close_code(
                1000),  // Explicitly use 1000 for normal closure
            [](boost::beast::error_code ec) {
              if (ec && ec != boost::beast::websocket::error::closed) {
                LOG(ERROR) << "Error in async_close: " << ec.message();
              }
              // Let the read handler handle the close response
            });

        // Give some time for the close handshake to complete
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      } catch (const std::exception& e) {
        LOG(ERROR) << "Error closing WebSocket: " << e.what();
      }
    }

    // Now stop the I/O service and thread pool
    ioc_.stop();
    LOG(INFO) << "I/O service stopped";

    thread_pool_->stop();
    LOG(INFO) << "Thread pool stopped";

    LOG(INFO) << "WebSocket client stopped";
  }

  void SendMessage(const std::string& message, bool is_binary = true) {
    if (!ws_.is_open()) {
      LOG(ERROR) << "WebSocket is not open";
      return;
    }

    try {
      // Set binary mode if needed
      ws_.binary(is_binary);

      // Write the message
      ws_.async_write(
          boost::asio::buffer(message),
          [](boost::beast::error_code ec, std::size_t bytes_transferred) {
            if (ec) {
              LOG(ERROR) << "Write error: " << ec.message();
              return;
            }
            LOG(INFO) << "Message sent successfully, size: "
                      << bytes_transferred;
          });
    } catch (const std::exception& e) {
      LOG(ERROR) << "Error sending message: " << e.what();
    }
  }

 private:
  void ProcessMessages() {
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

  void HandleMessage(const std::string& msg) {
    LOG(INFO) << "Handling message";
    if (msg.size() < 4) {
      LOG(ERROR) << "Message too short to contain op_code";
      return;
    }

    // Extract op_code from first 4 bytes (network byte order)
    uint32_t op_code = ntohl(*reinterpret_cast<const uint32_t*>(msg.data()));

    switch (static_cast<proto::OpCode>(op_code)) {
      case proto::OpCode::OP_UNUSED:
        LOG(INFO) << "Received OP_UNUSED message";
        break;
      case proto::OpCode::OP_LOAD_FOLDER:
        LOG(INFO) << "Received OP_LOAD_FOLDER message";
        handler::FolderLoader::Instance()->Load(msg);
        break;
      case proto::OpCode::OP_FILE_KEY:
        LOG(INFO) << "Received OP_FILE_KEY message";
        break;
      case proto::OpCode::OP_FILE_UPLOAD:
        LOG(INFO) << "Received OP_FILE_UPLOAD message";
        break;
      default:
        LOG(WARNING) << "Unknown op code: " << op_code;
        break;
    }
  }

  void ReadMessage() {
    ws_.async_read(read_buffer_, [this](boost::beast::error_code ec,
                                        std::size_t /*bytes_transferred*/) {
      if (ec) {
        if (ec == boost::beast::websocket::error::closed) {
          LOG(INFO) << "WebSocket closed by server";
          // Server has closed the connection, we can stop now
          running_ = false;
          ioc_.stop();
        } else {
          LOG(ERROR) << "Read error: " << ec.message();
        }
        return;
      }

      try {
        // Get the message payload (Boost.Beast has already handled the
        // WebSocket protocol)
        std::string msg = boost::beast::buffers_to_string(read_buffer_.data());
        LOG(INFO) << "Received message of size: " << msg.size();
        message_queue_.Push(std::move(msg));

        // Clear the buffer
        read_buffer_.consume(read_buffer_.size());

        // Continue reading
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

 private:
  std::string host_;
  std::string port_;
  boost::asio::io_context ioc_;
  boost::beast::websocket::stream<boost::asio::ip::tcp::socket> ws_;

  bazel_template::common::BlockingQueue<std::string> message_queue_;
  std::atomic<bool> running_;
  std::shared_ptr<folly::CPUThreadPoolExecutor> thread_pool_;
  boost::beast::flat_buffer read_buffer_;
};

}  // namespace client
}  // namespace bazel_template

#endif  // BAZEL_TEMPLATE_CLIENT_WEBSOCKET_CLIENT_H_
