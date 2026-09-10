/*
 * Copyright 2018 The Cartographer Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// Module interface for the blocking gRPC client wrappers. The request and
// response types are template parameters, so the concrete protobuf classes
// (and grpc's SerializationTraits for them, from proto_utils.h) only ever
// appear in the importing translation unit, which includes them before the
// import. Nothing here may pull protobuf's message.h into the global module
// fragment: MSVC dies writing a BMI that reaches the full Message (see
// docs/cpp20_modules.md).
module;

#include <chrono>
#include <functional>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <utility>

#include "grpc++/grpc++.h"
#include "grpc++/impl/codegen/client_unary_call.h"
#include "grpc++/impl/codegen/sync_stream.h"
#include "src/async_grpc/common/time.h"
#include "src/async_grpc/retry.h"
#include "src/async_grpc/rpc_service_method_traits.h"
#include "src/common/logging.h"

export module bazel_template.async_grpc.client;

export namespace async_grpc {
// Wraps a method invocation for all rpc types, unary, client streaming,
// server streaming, or bidirectional.
// It cannot be used for multiple invocations.
// It is not thread safe.
template <RpcServiceMethodSpec RpcServiceMethodConcept,
          ::grpc::internal::RpcMethod::RpcType StreamType =
              RpcServiceMethodTraits<RpcServiceMethodConcept>::StreamType>
class Client {};

// TODO(gaschler): Move specializations to separate header files.
template <RpcServiceMethodSpec RpcServiceMethodConcept>
class Client<RpcServiceMethodConcept, ::grpc::internal::RpcMethod::NORMAL_RPC> {
  using RpcServiceMethod = RpcServiceMethodTraits<RpcServiceMethodConcept>;
  using RequestType = RpcServiceMethod::RequestType;
  using ResponseType = RpcServiceMethod::ResponseType;

 public:
  explicit Client(std::shared_ptr<::grpc::Channel> channel)
      : channel_(std::move(channel)),
        client_context_(std::make_unique<::grpc::ClientContext>()),
        rpc_method_name_(RpcServiceMethod::MethodName()),
        rpc_method_(rpc_method_name_.c_str(), RpcServiceMethod::StreamType,
                    channel_) {}

  // 'timeout' is used for every 'Write' separately, but multiple retries
  // count towards a single timeout. It is a std::chrono duration, so any
  // duration that converts exactly (seconds, milliseconds, ...) also works.
  Client(std::shared_ptr<::grpc::Channel> channel,
         const common::Duration timeout, RetryStrategy retry_strategy = nullptr)
      : channel_(std::move(channel)),
        client_context_(std::make_unique<::grpc::ClientContext>()),
        rpc_method_name_(RpcServiceMethod::MethodName()),
        rpc_method_(rpc_method_name_.c_str(), RpcServiceMethod::StreamType,
                    channel_),
        timeout_(timeout),
        retry_strategy_(std::move(retry_strategy)) {}

  [[nodiscard]] bool Write(const RequestType& request,
                           ::grpc::Status* status = nullptr) {
    ::grpc::Status internal_status;
    std::optional<std::chrono::system_clock::time_point> deadline;
    if (timeout_.has_value()) {
      deadline =
          std::chrono::system_clock::now() +
          std::chrono::duration_cast<std::chrono::system_clock::duration>(
              *timeout_);
    }
    client_context_ = ResetContext(deadline);
    const bool result = RetryWithStrategy(
        retry_strategy_,
        [this, &request, &internal_status] {
          internal_status = MakeBlockingUnaryCall(request, &response_);
          return internal_status;
        },
        [this, deadline] { client_context_ = ResetContext(deadline); });
    if (status != nullptr) {
      *status = internal_status;
    }
    return result;
  }

  [[nodiscard]] const ResponseType& response() const { return response_; }

 private:
  [[nodiscard]] static std::unique_ptr<::grpc::ClientContext> ResetContext(
      const std::optional<std::chrono::system_clock::time_point> deadline) {
    auto context = std::make_unique<::grpc::ClientContext>();
    if (deadline.has_value()) {
      context->set_deadline(*deadline);
    }
    return context;
  }

  [[nodiscard]] ::grpc::Status MakeBlockingUnaryCall(const RequestType& request,
                                                     ResponseType* response) {
    return ::grpc::internal::BlockingUnaryCall(
        channel_.get(), rpc_method_, client_context_.get(), request, response);
  }

  std::shared_ptr<::grpc::Channel> channel_;
  std::unique_ptr<::grpc::ClientContext> client_context_;
  std::string rpc_method_name_;
  ::grpc::internal::RpcMethod rpc_method_;
  std::optional<common::Duration> timeout_;

  ResponseType response_;
  RetryStrategy retry_strategy_;
};

template <RpcServiceMethodSpec RpcServiceMethodConcept>
class Client<RpcServiceMethodConcept,
             ::grpc::internal::RpcMethod::CLIENT_STREAMING> {
  using RpcServiceMethod = RpcServiceMethodTraits<RpcServiceMethodConcept>;
  using RequestType = RpcServiceMethod::RequestType;
  using ResponseType = RpcServiceMethod::ResponseType;

 public:
  explicit Client(std::shared_ptr<::grpc::Channel> channel)
      : channel_(std::move(channel)),
        client_context_(std::make_unique<::grpc::ClientContext>()),
        rpc_method_name_(RpcServiceMethod::MethodName()),
        rpc_method_(rpc_method_name_.c_str(), RpcServiceMethod::StreamType,
                    channel_) {}

  [[nodiscard]] bool Write(const RequestType& request,
                           ::grpc::Status* status = nullptr) {
    ::grpc::Status internal_status;
    WriteImpl(request, &internal_status);
    if (status != nullptr) {
      *status = internal_status;
    }
    return internal_status.ok();
  }

  [[nodiscard]] bool StreamWritesDone() {
    InstantiateClientWriterIfNeeded();
    return client_writer_->WritesDone();
  }

  [[nodiscard]] ::grpc::Status StreamFinish() {
    InstantiateClientWriterIfNeeded();
    return client_writer_->Finish();
  }

  [[nodiscard]] const ResponseType& response() const { return response_; }

 private:
  bool WriteImpl(const RequestType& request, ::grpc::Status* /* status */) {
    InstantiateClientWriterIfNeeded();
    return client_writer_->Write(request);
  }

  void InstantiateClientWriterIfNeeded() {
    if (!client_writer_) {
      client_writer_.reset(
          ::grpc::internal::ClientWriterFactory<RequestType>::Create(
              channel_.get(), rpc_method_, client_context_.get(), &response_));
    }
  }

  std::shared_ptr<::grpc::Channel> channel_;
  std::unique_ptr<::grpc::ClientContext> client_context_;
  std::string rpc_method_name_;
  ::grpc::internal::RpcMethod rpc_method_;

  std::unique_ptr<::grpc::ClientWriter<RequestType>> client_writer_;
  ResponseType response_;
};

template <RpcServiceMethodSpec RpcServiceMethodConcept>
class Client<RpcServiceMethodConcept,
             ::grpc::internal::RpcMethod::SERVER_STREAMING> {
  using RpcServiceMethod = RpcServiceMethodTraits<RpcServiceMethodConcept>;
  using RequestType = RpcServiceMethod::RequestType;
  using ResponseType = RpcServiceMethod::ResponseType;

 public:
  explicit Client(std::shared_ptr<::grpc::Channel> channel)
      : channel_(std::move(channel)),
        client_context_(std::make_unique<::grpc::ClientContext>()),
        rpc_method_name_(RpcServiceMethod::MethodName()),
        rpc_method_(rpc_method_name_.c_str(), RpcServiceMethod::StreamType,
                    channel_) {}

  [[nodiscard]] bool StreamRead(ResponseType* response) {
    CHECK(client_reader_);
    return client_reader_->Read(response);
  }

  [[nodiscard]] bool Write(const RequestType& request,
                           ::grpc::Status* status = nullptr) {
    ::grpc::Status internal_status;
    WriteImpl(request, &internal_status);
    if (status != nullptr) {
      *status = internal_status;
    }
    return internal_status.ok();
  }

  [[nodiscard]] ::grpc::Status StreamFinish() {
    CHECK(client_reader_);
    return client_reader_->Finish();
  }

 private:
  bool WriteImpl(const RequestType& request, ::grpc::Status* /* status */) {
    InstantiateClientReader(request);
    return true;
  }

  void InstantiateClientReader(const RequestType& request) {
    client_reader_.reset(
        ::grpc::internal::ClientReaderFactory<ResponseType>::Create(
            channel_.get(), rpc_method_, client_context_.get(), request));
  }

  std::shared_ptr<::grpc::Channel> channel_;
  std::unique_ptr<::grpc::ClientContext> client_context_;
  std::string rpc_method_name_;
  ::grpc::internal::RpcMethod rpc_method_;

  std::unique_ptr<::grpc::ClientReader<ResponseType>> client_reader_;
};

template <RpcServiceMethodSpec RpcServiceMethodConcept>
class Client<RpcServiceMethodConcept,
             ::grpc::internal::RpcMethod::BIDI_STREAMING> {
  using RpcServiceMethod = RpcServiceMethodTraits<RpcServiceMethodConcept>;
  using RequestType = RpcServiceMethod::RequestType;
  using ResponseType = RpcServiceMethod::ResponseType;

 public:
  explicit Client(std::shared_ptr<::grpc::Channel> channel)
      : channel_(std::move(channel)),
        client_context_(std::make_unique<::grpc::ClientContext>()),
        rpc_method_name_(RpcServiceMethod::MethodName()),
        rpc_method_(rpc_method_name_.c_str(), RpcServiceMethod::StreamType,
                    channel_) {}

  [[nodiscard]] bool StreamRead(ResponseType* response) {
    InstantiateClientReaderWriterIfNeeded();
    return client_reader_writer_->Read(response);
  }

  [[nodiscard]] bool Write(const RequestType& request,
                           ::grpc::Status* status = nullptr) {
    ::grpc::Status internal_status;
    WriteImpl(request, &internal_status);
    if (status != nullptr) {
      *status = internal_status;
    }
    return internal_status.ok();
  }

  [[nodiscard]] bool StreamWritesDone() {
    InstantiateClientReaderWriterIfNeeded();
    return client_reader_writer_->WritesDone();
  }

  [[nodiscard]] ::grpc::Status StreamFinish() {
    InstantiateClientReaderWriterIfNeeded();
    return client_reader_writer_->Finish();
  }

 private:
  bool WriteImpl(const RequestType& request, ::grpc::Status* /* status */) {
    InstantiateClientReaderWriterIfNeeded();
    return client_reader_writer_->Write(request);
  }

  void InstantiateClientReaderWriterIfNeeded() {
    if (!client_reader_writer_) {
      client_reader_writer_.reset(
          ::grpc::internal::ClientReaderWriterFactory<
              RequestType, ResponseType>::Create(channel_.get(), rpc_method_,
                                                 client_context_.get()));
    }
  }

  std::shared_ptr<::grpc::Channel> channel_;
  std::unique_ptr<::grpc::ClientContext> client_context_;
  std::string rpc_method_name_;
  ::grpc::internal::RpcMethod rpc_method_;

  std::unique_ptr<::grpc::ClientReaderWriter<RequestType, ResponseType>>
      client_reader_writer_;
};

}  // namespace async_grpc
