/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef BAZEL_TEMPLATE_SERVER_HTTP_HANDLER_LOAD_FOLDER_HANDLER_H_
#define BAZEL_TEMPLATE_SERVER_HTTP_HANDLER_LOAD_FOLDER_HANDLER_H_

#include <fstream>
#include <memory>

#include "folly/io/IOBuf.h"
#include "proxygen/httpserver/RequestHandler.h"
#include "proxygen/httpserver/ResponseBuilder.h"
#include "src/proto/enums.pb.h"
#include "src/proto/error.pb.h"
#include "src/proto/service.pb.h"
#include "src/server/http_handler/websocket_handler.h"
#include "src/util/util.h"

namespace bazel_template {
namespace server {
namespace http_handler {

class LoadFolderHandler : public proxygen::RequestHandler {
 public:
  void onRequest(
      std::unique_ptr<proxygen::HTTPMessage> request) noexcept override {
    request_ = std::move(request);

    // Check if Upgrade and Connection headers are present
    if (!request_->getHeaders().exists(proxygen::HTTP_HEADER_UPGRADE) ||
        !request_->getHeaders().exists(proxygen::HTTP_HEADER_CONNECTION)) {
      LOG(ERROR) << "Missing Upgrade/Connection header";
      proxygen::ResponseBuilder(downstream_).rejectUpgradeRequest();
      return;
    }

    LOG(INFO) << "kWSVersionHeader: "
              << request_->getHeaders().getSingleOrEmpty(kWSVersionHeader);

    const std::string& proto =
        request_->getHeaders().getSingleOrEmpty(proxygen::HTTP_HEADER_UPGRADE);
    if (!proxygen::caseInsensitiveEqual(proto, kUpgradeTo)) {
      LOG(ERROR) << "Provided upgrade protocol: '" << proto << "', expected: '"
                 << kUpgradeTo << "'";
      proxygen::ResponseBuilder(downstream_).rejectUpgradeRequest();
      return;
    }

    // Get WebSocket key
    const std::string& ws_key =
        request_->getHeaders().getSingleOrEmpty(kWSKeyHeader);
    if (ws_key.empty()) {
      LOG(ERROR) << "Missing Sec-WebSocket-Key header";
      proxygen::ResponseBuilder(downstream_).rejectUpgradeRequest();
      return;
    }

    // Build the upgrade response.
    proxygen::ResponseBuilder response(downstream_);
    response.status(101, "Switching Protocols")
        .setEgressWebsocketHeaders()
        .header(kWSVersionHeader, kWSVersion)
        .send();

    // Initialize WebSocket handler after successful upgrade
    ws_handler_ = std::make_unique<WebSocketHandler>();
    ws_handler_->SetCallback([this](const std::string& message) {
      HandleWebSocketMessage(message);
    });
    ws_handler_->SetCloseCallback([this]() { HandleWebSocketClose(); });
    ws_handler_->SetSendFrameCallback(
        [this](const std::string& frame) { SendWebSocketFrame(frame); });

    LOG(INFO) << "WebSocket connection upgraded";
  }

  void onBody(std::unique_ptr<folly::IOBuf> body) noexcept override {
    if (!ws_handler_) {
      LOG(ERROR) << "WebSocket handler not initialized";
      return;
    }

    if (!ws_handler_->ProcessFrame(std::move(body))) {
      LOG(ERROR) << "Failed to process WebSocket frame";
      // Don't send a response here, just log the error
    }
  }

  void onEOM() noexcept override {
    // Nothing to do here for WebSocket connections
  }

  void onUpgrade(proxygen::UpgradeProtocol) noexcept override {
    LOG(INFO) << "WebSocket connection upgraded";
  }

  void requestComplete() noexcept override {
    LOG(INFO) << "Request complete, cleaning up WebSocket handler";
    ws_handler_.reset();
    delete this;
  }

  void onError(proxygen::ProxygenError err) noexcept override {
    LOG(INFO) << "Request error: " << proxygen::getErrorString(err)
              << ", cleaning up WebSocket handler";
    ws_handler_.reset();
    delete this;
  }

 private:
  static constexpr size_t CHUNK_SIZE = 1 * 1024 * 1024;  // 1MB chunks
  static constexpr size_t OP_CODE_SIZE = 4;              // 4 bytes for op code

  std::string CreateMessage(proto::OpCode op_code, proto::ErrCode err_code,
                            const std::string& payload) {
    if (!ws_handler_) {
      LOG(ERROR) << "WebSocket handler not initialized";
      return "";
    }

    // Create our protocol message
    std::string protocol_msg;
    protocol_msg.reserve(OP_CODE_SIZE + OP_CODE_SIZE + payload.size());

    // Convert op_code to network byte order (big-endian)
    uint32_t op = htonl(static_cast<uint32_t>(op_code));
    protocol_msg.append(reinterpret_cast<char*>(&op), OP_CODE_SIZE);

    uint32_t err = htonl(static_cast<uint32_t>(err_code));
    protocol_msg.append(reinterpret_cast<char*>(&err), OP_CODE_SIZE);

    protocol_msg.append(payload);

    // Wrap our protocol message in a WebSocket binary frame
    return ws_handler_->AssembleFrame(protocol_msg,
                                      0x2);  // 0x2 is binary frame
  }

  void HandleWebSocketMessage(const std::string& message) {
    LOG(INFO) << "Handling WebSocket message: " << message;

    if (message.size() < OP_CODE_SIZE) {
      SendWebSocketFrame(CreateMessage(proto::OpCode::OP_LOAD_FOLDER,
                                       proto::ErrCode::FAIL,
                                       "Message too short"));
      return;
    }

    // Parse the request message
    proto::LoadFolderRequest req;
    if (!util::Util::JsonToMessage(message.substr(OP_CODE_SIZE), &req)) {
      SendWebSocketFrame(CreateMessage(proto::OpCode::OP_LOAD_FOLDER,
                                       proto::ErrCode::FAIL,
                                       "Invalid request format"));
      LOG(INFO) << "Invalid request format";
      return;
    }
    LOG(INFO) << "Received LoadFolderRequest: " << req.folder_id();

    // Open the large JSON file
    std::ifstream file("large.json", std::ios::binary);
    if (!file) {
      SendWebSocketFrame(CreateMessage(proto::OpCode::OP_LOAD_FOLDER,
                                       proto::ErrCode::FAIL,
                                       "Failed to open data file"));
      LOG(INFO) << "Failed to open data file";
      return;
    }

    // Get file size
    file.seekg(0, std::ios::end);
    file.seekg(0, std::ios::beg);

    // Read and send in chunks
    std::vector<char> buffer(CHUNK_SIZE);

    while (file) {
      file.read(buffer.data(), CHUNK_SIZE);
      size_t bytes_read = file.gcount();
      if (bytes_read == 0) break;

      // Create chunk message
      std::string chunk_msg;
      chunk_msg.append(buffer.data(), bytes_read);

      // Send chunk
      LOG(INFO) << "Sending chunk message";
      SendWebSocketFrame(CreateMessage(proto::OpCode::OP_LOAD_FOLDER,
                                       proto::ErrCode::SUCCESS, chunk_msg));

      // Optional: Add a small delay between chunks
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Send completion message
    SendWebSocketFrame(CreateMessage(proto::OpCode::OP_LOAD_FOLDER,
                                     proto::ErrCode::FINISH, ""));
  }

  void HandleWebSocketClose() {
    LOG(INFO) << "WebSocket connection closed by client";

    // Send close response frame
    if (ws_handler_) {
      std::string close_frame;
      close_frame.push_back(0x88);  // FIN + CLOSE opcode
      close_frame.push_back(0x82);  // MASK + 2 bytes payload length

      // Generate random masking key
      uint32_t masking_key = rand();
      close_frame.append(reinterpret_cast<char*>(&masking_key),
                         sizeof(masking_key));

      // Add status code in network byte order
      uint16_t response_code = htons(1000);  // Normal Closure
      std::string payload;
      payload.append(reinterpret_cast<char*>(&response_code),
                     sizeof(response_code));

      // Apply masking
      for (size_t i = 0; i < payload.size(); i++) {
        payload[i] ^= (masking_key >> ((3 - (i % 4)) * 8)) & 0xFF;
      }
      close_frame.append(payload);

      LOG(INFO) << "Sending close response frame";
      SendWebSocketFrame(close_frame);
    }
  }

  void SendWebSocketFrame(const std::string& frame) {
    if (!downstream_) {
      LOG(ERROR) << "No downstream connection available";
      return;
    }
    proxygen::ResponseBuilder(downstream_).body(frame).send();
  }

  std::unique_ptr<proxygen::HTTPMessage> request_;
  std::unique_ptr<folly::IOBuf> body_;
  std::unique_ptr<WebSocketHandler> ws_handler_;
};

}  // namespace http_handler
}  // namespace server
}  // namespace bazel_template

#endif  // BAZEL_TEMPLATE_SERVER_HTTP_HANDLER_LOAD_FOLDER_HANDLER_H_
