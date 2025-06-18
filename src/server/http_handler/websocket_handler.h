/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef BAZEL_TEMPLATE_SERVER_HTTP_HANDLER_WEBSOCKET_HANDLER_H_
#define BAZEL_TEMPLATE_SERVER_HTTP_HANDLER_WEBSOCKET_HANDLER_H_

#include <arpa/inet.h>  // for htonl

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "folly/io/IOBuf.h"
#include "folly/io/IOBufQueue.h"

namespace bazel_template {
namespace server {
namespace http_handler {

const std::string kWSKeyHeader = "Sec-WebSocket-Key";
const std::string kWSProtocolHeader = "Sec-WebSocket-Protocol";
const std::string kWSExtensionsHeader = "Sec-WebSocket-Extensions";
const std::string kWSAcceptHeader = "Sec-WebSocket-Accept";
const std::string kWSVersionHeader = "Sec-WebSocket-Version";

const std::string kWSVersion = "13";
const std::string kUpgradeTo = "websocket";

// WebSocket frame header structure
struct WebSocketFrameHeader {
  bool fin;
  uint8_t opcode;
  bool mask;
  uint64_t payload_length;
  uint32_t masking_key;
};

class WebSocketHandler {
 public:
  using MessageCallback = std::function<void(const std::string&)>;
  using CloseCallback = std::function<void()>;
  using SendFrameCallback = std::function<void(const std::string&)>;

  WebSocketHandler()
      : current_message_(std::make_unique<folly::IOBufQueue>()),
        is_fragmented_(false),
        current_opcode_(0) {}

  // Add new function to assemble WebSocket frames
  std::string AssembleFrame(const std::string& message, uint8_t opcode = 0x1) {
    std::string frame;

    // First byte: FIN (1) + opcode
    frame.push_back(0x80 | (opcode & 0x0F));

    // Second byte: MASK (0) + payload length
    if (message.size() <= 125) {
      frame.push_back(message.size());
    } else if (message.size() <= 65535) {
      frame.push_back(126);
      uint16_t len = htons(message.size());
      frame.append(reinterpret_cast<char*>(&len), sizeof(len));
    } else {
      frame.push_back(127);
      uint64_t len = folly::Endian::big(message.size());
      frame.append(reinterpret_cast<char*>(&len), sizeof(len));
    }

    // Add payload without masking
    frame.append(message);

    return frame;
  }

  void SetCallback(MessageCallback callback) {
    message_callback_ = std::move(callback);
  }

  void SetCloseCallback(CloseCallback callback) {
    close_callback_ = std::move(callback);
  }

  void SetSendFrameCallback(SendFrameCallback callback) {
    send_frame_callback_ = std::move(callback);
  }

  bool ProcessFrame(std::unique_ptr<folly::IOBuf> frame) {
    LOG(INFO) << "Processing frame";
    if (!frame || frame->length() < 2) {
      LOG(ERROR) << "Frame too short";
      return false;
    }

    // Parse frame header
    const uint8_t* data = frame->data();
    WebSocketFrameHeader header;
    header.fin = (data[0] & 0x80) != 0;
    header.opcode = data[0] & 0x0F;
    header.mask = (data[1] & 0x80) != 0;

    // Validate opcode
    if (header.opcode > 0xA) {
      LOG(ERROR) << "Invalid opcode: " << header.opcode;
      return false;
    }

    // Handle control frames
    if (header.opcode >= 0x8) {  // Control frames
      if (!header.fin) {
        LOG(ERROR) << "Control frame must not be fragmented";
        return false;
      }
      if (frame->length() > 125) {
        LOG(ERROR) << "Control frame payload too large";
        return false;
      }

      switch (header.opcode) {
        case 0x8:  // Close
          LOG(INFO) << "Received close frame";
          if (frame->length() >= 4) {
            // Parse frame header
            const uint8_t* data = frame->data();
            bool fin = (data[0] & 0x80) != 0;
            uint8_t opcode = data[0] & 0x0F;
            bool mask = (data[1] & 0x80) != 0;
            uint8_t length_field = data[1] & 0x7F;

            // Calculate header size
            size_t header_size = 2;  // Base header size
            if (length_field == 126) {
              header_size += 2;
            } else if (length_field == 127) {
              header_size += 8;
            }

            // Get masking key if present
            uint32_t masking_key = 0;
            if (mask) {
              if (frame->length() < header_size + 4) {
                LOG(ERROR) << "Frame too short for masking key";
                return false;
              }
              masking_key =
                  ntohl(*reinterpret_cast<const uint32_t*>(data + header_size));
              header_size += 4;
            }

            // Get payload and unmask if needed
            const uint8_t* payload = data + header_size;
            uint16_t status_code;
            if (mask) {
              // Create a copy of the payload to unmask
              std::vector<uint8_t> unmasked_payload(payload, payload + 2);
              for (size_t i = 0; i < unmasked_payload.size(); i++) {
                unmasked_payload[i] ^=
                    (masking_key >> ((3 - (i % 4)) * 8)) & 0xFF;
              }
              status_code = ntohs(
                  *reinterpret_cast<const uint16_t*>(unmasked_payload.data()));
            } else {
              status_code = ntohs(*reinterpret_cast<const uint16_t*>(payload));
            }

            LOG(INFO) << "Close frame: fin=" << fin
                      << ", opcode=" << (int)opcode << ", mask=" << mask
                      << ", length=" << (int)length_field
                      << ", header_size=" << header_size << ", masking_key=0x"
                      << std::hex << masking_key << std::dec
                      << ", status_code=" << status_code << " (0x" << std::hex
                      << status_code << std::dec << ")"
                      << " [raw bytes: 0x" << std::hex << (int)payload[0]
                      << " 0x" << (int)payload[1] << std::dec << "]";

            // Send close frame in response
            if (send_frame_callback_) {
              std::string close_frame;
              close_frame.push_back(0x88);  // FIN + CLOSE opcode
              close_frame.push_back(0x82);  // MASK + 2 bytes payload length

              // Generate random masking key
              uint32_t response_masking_key = rand();
              close_frame.append(reinterpret_cast<char*>(&response_masking_key),
                                 sizeof(response_masking_key));

              // Add status code in network byte order
              uint16_t response_code = htons(1000);  // Normal Closure
              std::string response_payload;
              response_payload.append(reinterpret_cast<char*>(&response_code),
                                      sizeof(response_code));

              // Apply masking
              for (size_t i = 0; i < response_payload.size(); i++) {
                response_payload[i] ^=
                    (response_masking_key >> ((3 - (i % 4)) * 8)) & 0xFF;
              }
              close_frame.append(response_payload);

              LOG(INFO) << "Sending close frame with status: " << 1000 << " (0x"
                        << std::hex << 1000 << std::dec << ")"
                        << " [masking_key: 0x" << std::hex
                        << response_masking_key << std::dec << "]";
              send_frame_callback_(close_frame);
            }
          }
          if (close_callback_) {
            close_callback_();
          }
          return true;

        case 0x9:  // Ping
          LOG(INFO) << "Received ping frame";
          if (send_frame_callback_) {
            // Create pong frame
            std::string pong_frame;
            pong_frame.push_back(0x8A);  // FIN + PONG opcode
            pong_frame.push_back(0x80);  // MASK + 0 bytes payload length

            // Generate random masking key
            uint32_t masking_key = rand();
            pong_frame.append(reinterpret_cast<char*>(&masking_key),
                              sizeof(masking_key));

            LOG(INFO) << "Sending pong frame [masking_key: 0x" << std::hex
                      << masking_key << std::dec << "]";
            send_frame_callback_(pong_frame);
          }
          return true;

        case 0xA:  // Pong
          LOG(INFO) << "Received pong frame";
          return true;

        default:
          LOG(ERROR) << "Unknown control frame opcode: " << header.opcode;
          return false;
      }
    }

    // Handle data frames
    if (header.opcode == 0x0) {  // Continuation frame
      if (!is_fragmented_) {
        LOG(ERROR) << "Unexpected continuation frame";
        return false;
      }
    } else {  // New message
      if (is_fragmented_) {
        LOG(ERROR) << "Previous message not finished";
        return false;
      }
      is_fragmented_ = true;
      current_opcode_ = header.opcode;
      current_message_ = std::make_unique<folly::IOBufQueue>();
    }

    // Parse payload length
    size_t header_size = 2;
    uint8_t length_field = data[1] & 0x7F;
    uint64_t payload_length;
    if (length_field == 126) {
      if (frame->length() < 4) return false;
      payload_length = ntohs(*reinterpret_cast<const uint16_t*>(data + 2));
      header_size = 4;
    } else if (length_field == 127) {
      if (frame->length() < 10) return false;
      payload_length =
          folly::Endian::big(*reinterpret_cast<const uint64_t*>(data + 2));
      header_size = 10;
    } else {
      payload_length = length_field;
    }

    // Validate payload length
    if (payload_length > 100 * 1024 * 1024) {  // 100MB limit
      LOG(ERROR) << "Payload too large: " << payload_length;
      return false;
    }

    // Handle masking
    if (header.mask) {
      if (frame->length() < header_size + 4) return false;
      header.masking_key =
          ntohl(*reinterpret_cast<const uint32_t*>(data + header_size));
      header_size += 4;
    }

    // Skip header
    frame->trimStart(header_size);

    // Unmask payload if needed
    if (header.mask) {
      uint8_t* payload = frame->writableData();
      for (size_t i = 0; i < frame->length(); i++) {
        payload[i] ^= (header.masking_key >> ((3 - (i % 4)) * 8)) & 0xFF;
      }
    }

    // Add payload to current message
    current_message_->append(std::move(frame));

    // If this is the final frame, process the complete message
    if (header.fin) {
      is_fragmented_ = false;
      auto complete_message = current_message_->move();
      current_message_ = std::make_unique<folly::IOBufQueue>();

      // Convert the complete message to string and call the callback
      if (message_callback_) {
        LOG(INFO) << "Calling callback for "
                  << (current_opcode_ == 0x1 ? "text" : "binary") << " message";
        std::string message_str =
            complete_message->moveToFbString().toStdString();
        message_callback_(message_str);
      }
    }

    return true;
  }

 private:
  std::unique_ptr<folly::IOBufQueue> current_message_;
  MessageCallback message_callback_;
  CloseCallback close_callback_;
  SendFrameCallback send_frame_callback_;
  bool is_fragmented_;
  uint8_t current_opcode_;
};

}  // namespace http_handler
}  // namespace server
}  // namespace bazel_template

#endif  // BAZEL_TEMPLATE_SERVER_HTTP_HANDLER_WEBSOCKET_HANDLER_H_
