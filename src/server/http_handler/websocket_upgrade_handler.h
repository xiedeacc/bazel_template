/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef BAZEL_TEMPLATE_SERVER_HTTP_HANDLER_WEBSOCKET_UPGRADE_HANDLER_H_
#define BAZEL_TEMPLATE_SERVER_HTTP_HANDLER_WEBSOCKET_UPGRADE_HANDLER_H_

#include <memory>
#include <string>
#include <utility>

#include "folly/io/IOBuf.h"
#include "proxygen/httpserver/RequestHandler.h"
#include "proxygen/httpserver/ResponseBuilder.h"
#include "proxygen/lib/utils/UtilInl.h"
#include "src/common/logging.h"
#include "src/server/http_handler/websocket_handler.h"

namespace bazel_template::server::http_handler {

// Performs the RFC 6455 upgrade handshake and then feeds raw frames to
// WebSocketHandler. Sec-WebSocket-Accept is computed by proxygen's HTTP/1.x
// codec once setEgressWebsocketHeaders() marks the response as an upgrade.
//
// Messages are echoed back to the peer. This deliberately carries no
// application payload handling: the LoadFolder interface that used to live
// here was removed in 81d0247 along with its protobuf messages.
class WebSocketUpgradeHandler : public proxygen::RequestHandler {
 public:
  void onRequest(
      std::unique_ptr<proxygen::HTTPMessage> request) noexcept override {
    // proxygen declares this noexcept, but header lookups, the response
    // builder and the handler allocation can all throw. Rejecting the
    // upgrade beats terminating the server.
    try {
      HandleUpgradeRequest(std::move(request));
    } catch (const std::exception& e) {
      LOG(ERROR) << "WebSocket upgrade failed: " << e.what();
    } catch (...) {
      LOG(ERROR) << "WebSocket upgrade failed";
    }
  }

  void onBody(std::unique_ptr<folly::IOBuf> body) noexcept override {
    // proxygen declares this noexcept; frame parsing and logging can throw.
    try {
      if (!ws_handler_) {
        LOG(ERROR) << "WebSocket handler not initialized";
        return;
      }
      if (!ws_handler_->ProcessFrame(std::move(body))) {
        LOG(ERROR) << "Failed to process WebSocket frame";
      }
    } catch (...) {  // NOLINT(bugprone-empty-catch)
    }
  }

  void onEOM() noexcept override {}

  void onUpgrade(proxygen::UpgradeProtocol /*prot*/) noexcept override {}

  void requestComplete() noexcept override {
    ws_handler_.reset();
    delete this;
  }

  void onError(proxygen::ProxygenError err) noexcept override {
    // proxygen declares this noexcept; logging can throw. `delete this` must
    // still run, or the handler leaks.
    try {
      LOG(INFO) << "Request error: " << proxygen::getErrorString(err);
    } catch (...) {  // NOLINT(bugprone-empty-catch)
    }
    ws_handler_.reset();
    delete this;
  }

 private:
  void HandleUpgradeRequest(std::unique_ptr<proxygen::HTTPMessage> request) {
    request_ = std::move(request);

    if (!request_->getHeaders().exists(proxygen::HTTP_HEADER_UPGRADE) ||
        !request_->getHeaders().exists(proxygen::HTTP_HEADER_CONNECTION)) {
      LOG(ERROR) << "Missing Upgrade/Connection header";
      proxygen::ResponseBuilder(downstream_).rejectUpgradeRequest();
      return;
    }

    const std::string& proto =
        request_->getHeaders().getSingleOrEmpty(proxygen::HTTP_HEADER_UPGRADE);
    if (!proxygen::caseInsensitiveEqual(proto, kUpgradeTo)) {
      LOG(ERROR) << "Provided upgrade protocol: '" << proto << "', expected: '"
                 << kUpgradeTo << "'";
      proxygen::ResponseBuilder(downstream_).rejectUpgradeRequest();
      return;
    }

    if (request_->getHeaders().getSingleOrEmpty(kWSKeyHeader).empty()) {
      LOG(ERROR) << "Missing " << kWSKeyHeader << " header";
      proxygen::ResponseBuilder(downstream_).rejectUpgradeRequest();
      return;
    }

    proxygen::ResponseBuilder(downstream_)
        .status(101, "Switching Protocols")
        .setEgressWebsocketHeaders()
        .header(kWSVersionHeader, kWSVersion)
        .send();

    ws_handler_ = std::make_unique<WebSocketHandler>();
    ws_handler_->SetCallback(
        [this](const std::string& message) { HandleMessage(message); });
    ws_handler_->SetCloseCallback(
        []() { LOG(INFO) << "WebSocket connection closed by peer"; });
    ws_handler_->SetSendFrameCallback(
        [this](const std::string& frame) { SendFrame(frame); });

    LOG(INFO) << "WebSocket connection upgraded";
  }

  void HandleMessage(const std::string& message) {
    LOG(INFO) << "Echoing WebSocket message of " << message.size() << " bytes";
    SendFrame(ws_handler_->AssembleFrame(message, 0x1));  // 0x1: text frame
  }

  void SendFrame(const std::string& frame) {
    proxygen::ResponseBuilder(downstream_).body(frame).send();
  }

  std::unique_ptr<proxygen::HTTPMessage> request_;
  std::unique_ptr<WebSocketHandler> ws_handler_;
};

}  // namespace bazel_template::server::http_handler

#endif  // BAZEL_TEMPLATE_SERVER_HTTP_HANDLER_WEBSOCKET_UPGRADE_HANDLER_H_
