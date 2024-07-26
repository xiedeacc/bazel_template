/*******************************************************************************
 * Copyright (c) 2022 Copyright 2022- xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef GRPC_FRAMEWORK_SERVER_GRPC_TAG_H
#define GRPC_FRAMEWORK_SERVER_GRPC_TAG_H

#include <grpcpp/support/status_code_enum.h>

#include "grpcpp/grpcpp.h"
#include "src/common/grpc_framework/server_impl.h"
#include "src/common/grpc_framework/server_grpc_reader.h"
#include "src/common/grpc_framework/server_grpc_writer.h"
#include "src/common/grpc_framework/tag_base.h"
#include "src/util/common_util.h"

namespace bazel_template {
namespace common {
namespace grpc_framework {

enum class ServerRPCStatus {
  CREATE,
  READ,
  WRITE,
  WORKING,
  FINISH,
  DESTORY,
  ERR
};

template <typename RequestType, typename ResponseType>
class ServerUnaryRpcTag : public TagBase {
 public:
  ServerUnaryRpcTag(grpc::ServerCompletionQueue* request_queue,
                    grpc::ServerCompletionQueue* response_queue)
      : responder_(&server_context_),
        request_queue_(request_queue),
        response_queue_(response_queue) {
    status_ = ServerRPCStatus::CREATE;
  }

  virtual ~ServerUnaryRpcTag() {}

  virtual std::string Name() override { return "ServerUnaryRpcTag"; }

  virtual void Process() override = 0;

  virtual void Finish() {
    status_ = ServerRPCStatus::FINISH;
    responder_.Finish(response_, rpc_status_, this);
  }

  virtual void OnError() override {
    status_ = ServerRPCStatus::FINISH;
    server_context_.TryCancel();
    Process();
  }

 protected:
  grpc::ServerContext server_context_;
  ServerRPCStatus status_;
  grpc::Status rpc_status_;

  typename grpc::ServerAsyncResponseWriter<ResponseType> responder_;
  RequestType request_;
  ResponseType response_;
  grpc::ServerCompletionQueue* request_queue_;
  grpc::ServerCompletionQueue* response_queue_;
};

template <typename RequestType, typename ResponseType>
class ServerClientStreamRpcTag : public TagBase, public ReaderCallback {
 public:
  ServerClientStreamRpcTag(grpc::ServerCompletionQueue* request_queue,
                           grpc::ServerCompletionQueue* response_queue)
      : responder_(&server_context_),
        request_queue_(request_queue),
        response_queue_(response_queue) {
    status_ = ServerRPCStatus::CREATE;
  }

  virtual ~ServerClientStreamRpcTag() {}

  virtual std::string Name() override { return "ServerClientStreamRpcTag"; }

  virtual void Process() override = 0;

  virtual void Finish() {
    status_ = bazel_template::common::grpc_framework::ServerRPCStatus::FINISH;
    responder_.Finish(response_, rpc_status_, this);
  }

  virtual void OnError() override {
    status_ = ServerRPCStatus::FINISH;
    server_context_.TryCancel();
    Process();
  }

  virtual void OnRead(void* req_ptr) override = 0;

  virtual void OnReadError() override {
    LOG(INFO) << "OnReadError";
    OnError();
  }

 protected:
  grpc::ServerContext server_context_;
  ServerRPCStatus status_;
  grpc::Status rpc_status_;

  typedef Reader<RequestType,
                 grpc::ServerAsyncReader<ResponseType, RequestType>>
      ReaderType;

  typename grpc::ServerAsyncReader<ResponseType, RequestType> responder_;
  std::unique_ptr<ReaderType> reader_;
  grpc::ServerCompletionQueue* request_queue_;
  grpc::ServerCompletionQueue* response_queue_;
  RequestType request_;
  ResponseType response_;
};

template <typename RequestType, typename ResponseType>
class ServerServerStreamRpcTag : public TagBase, public WriterCallback {
 public:
  ServerServerStreamRpcTag(grpc::ServerCompletionQueue* request_queue,
                           grpc::ServerCompletionQueue* response_queue)
      : responder_(&server_context_),
        request_queue_(request_queue),
        response_queue_(response_queue) {
    status_ = ServerRPCStatus::CREATE;
  }

  virtual ~ServerServerStreamRpcTag() {}

  virtual std::string Name() override { return "ServerServerStreamRpcTag"; }

  virtual void Process() override = 0;

  virtual void Finish() override {
    LOG(INFO) << "Finish";
    status_ = bazel_template::common::grpc_framework::ServerRPCStatus::FINISH;
    responder_.Finish(rpc_status_, this);
  }

  virtual void OnError() override {
    LOG(INFO) << "OnError";
    status_ = ServerRPCStatus::FINISH;
    server_context_.TryCancel();
    Process();
  }

  virtual void OnWrite(int write_id) override = 0;

  virtual void OnWriteError() override {
    LOG(INFO) << "OnWriteError";
    writer_->Stop();
    OnError();
  }

 protected:
  grpc::ServerContext server_context_;
  ServerRPCStatus status_;
  grpc::Status rpc_status_;

  typedef Writer<ResponseType, grpc::ServerAsyncWriter<ResponseType>>
      WriterType;
  typename grpc::ServerAsyncWriter<ResponseType> responder_;

  std::unique_ptr<WriterType> writer_;
  grpc::ServerCompletionQueue* request_queue_;
  grpc::ServerCompletionQueue* response_queue_;
  RequestType request_;
};

template <typename RequestType, typename ResponseType>
class ServerBiStreamRpcTag : public TagBase,
                             public ReaderCallback,
                             public WriterCallback {
 public:
  ServerBiStreamRpcTag(grpc::ServerCompletionQueue* request_queue,
                       grpc::ServerCompletionQueue* response_queue)
      : responder_(&server_context_),
        request_queue_(request_queue),
        response_queue_(response_queue) {
    status_ = ServerRPCStatus::CREATE;
  }

  virtual ~ServerBiStreamRpcTag() {}

  virtual std::string Name() override { return "ServerBiStreamRpcTag"; }

  virtual void Process() override = 0;

  virtual void Finish() override {
    status_ = bazel_template::common::grpc_framework::ServerRPCStatus::FINISH;
    responder_.Finish(rpc_status_, this);
  }

  virtual void OnError() override {
    status_ = ServerRPCStatus::FINISH;
    server_context_.TryCancel();
    Process();
  }

  virtual void OnRead(void* req_ptr) override = 0;

  virtual void OnReadError() override = 0;

  virtual void OnWrite(int write_id) override = 0;

  virtual void OnWriteError() override {
    writer_->Stop();
    OnError();
  }

 protected:
  grpc::ServerContext server_context_;
  ServerRPCStatus status_;
  grpc::Status rpc_status_;

  typedef Reader<RequestType,
                 grpc::ServerAsyncReaderWriter<ResponseType, RequestType>>
      ReaderType;

  typedef Writer<ResponseType,
                 grpc::ServerAsyncReaderWriter<ResponseType, RequestType>>
      WriterType;

  typename grpc::ServerAsyncReaderWriter<ResponseType, RequestType> responder_;
  std::unique_ptr<ReaderType> reader_;
  std::unique_ptr<WriterType> writer_;
  grpc::ServerCompletionQueue* request_queue_;
  grpc::ServerCompletionQueue* response_queue_;
};

}  // namespace grpc_framework
}  // namespace common
}  // namespace bazel_template

#endif  // GRPC_FRAMEWORK_SERVER_GRPC_TAG_H
