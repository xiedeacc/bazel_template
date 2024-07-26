/*******************************************************************************
 * Copyright (c) 2022 Copyright 2022- xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef GRPC_FRAMEWORK_CLIENT_GRPC_TAG_H
#define GRPC_FRAMEWORK_CLIENT_GRPC_TAG_H

#include <grpcpp/support/async_stream.h>

#include "grpcpp/grpcpp.h"
#include "src/common/grpc_framework/client_grpc_reader.h"
#include "src/common/grpc_framework/client_grpc_writer.h"
#include "src/common/grpc_framework/grpc_client_impl.h"
#include "src/common/grpc_framework/tag_base.h"
#include "src/util/common_util.h"

namespace bazel_template {
namespace common {
namespace grpc_framework {

enum class ClientRPCStatus {
  CREATE,
  READ,
  WRITE,
  WORKING,
  FINISH,
  DESTORY,
  ERR
};

template <typename RequestType, typename ResponseType>
class ClientUnaryRpcTag : public TagBase {
 public:
  ClientUnaryRpcTag(RequestType &request) : request_(request) {
    status_ = ClientRPCStatus::CREATE;
  }

  virtual ~ClientUnaryRpcTag() {}

  virtual std::string Name() override { return "ClientUnaryRpcTag"; }

  virtual void Process() override = 0;

  virtual void Finish() {
    status_ = bazel_template::common::grpc_framework::ClientRPCStatus::FINISH;
    rpc_->Finish(&response_, &rpc_status_, this);
  }

  virtual void OnError() override {
    status_ = ClientRPCStatus::FINISH;
    client_context_.TryCancel();
    Process();
  }

 protected:
  grpc::ClientContext client_context_;
  ClientRPCStatus status_;
  grpc::Status rpc_status_;

  std::unique_ptr<grpc::ClientAsyncResponseReader<ResponseType>> rpc_;
  RequestType request_;
  ResponseType response_;
};

template <typename RequestType, typename ResponseType>
class ClientClientStreamRpcTag : public TagBase, public WriterCallback {
 public:
  ClientClientStreamRpcTag() : can_call_(false) {
    status_ = ClientRPCStatus::CREATE;
  };

  virtual ~ClientClientStreamRpcTag() {}

  virtual std::string Name() override { return "ClientClientStreamRpcTag"; }

  virtual void Process() override = 0;

  virtual void Finish() override {
    status_ = bazel_template::common::grpc_framework::ClientRPCStatus::FINISH;
    rpc_->Finish(&rpc_status_, this);
  }

  virtual void OnError() override {
    status_ = ClientRPCStatus::FINISH;
    client_context_.TryCancel();
    Process();
  }

  virtual bool Call(const RequestType *req) {
    LOG(INFO) << "Call";
    if (!can_call_) {
      return false;
    }
    if (req == nullptr) {
      writer_->Write(nullptr);
      return true;
    }
    RequestType *request = writer_->NewRequest();
    request->CopyFrom(*req);
    // LOG(INFO) << "request: \n"
    //<< bazel_template::util::CommonUtil::MessageToJsonString(*request);
    writer_->Write(request);
    return true;
  }

  virtual void OnWrite(int write_id) override = 0;

  virtual void OnWriteError() override {
    writer_->Stop();
    OnError();
  }

 protected:
  grpc::ClientContext client_context_;
  ClientRPCStatus status_;
  grpc::Status rpc_status_;

  std::unique_ptr<grpc::ClientAsyncWriter<RequestType>> rpc_;
  typedef Writer<RequestType, grpc::ClientAsyncWriter<RequestType>> WriterType;
  std::unique_ptr<WriterType> writer_;
  ResponseType response_;
  bool can_call_;
};

template <typename RequestType, typename ResponseType>
class ClientServerStreamRpcTag : public TagBase, public ReaderCallback {
 public:
  ClientServerStreamRpcTag(RequestType &request) : request_(request) {
    status_ = ClientRPCStatus::CREATE;
  }

  virtual ~ClientServerStreamRpcTag() {}

  virtual std::string Name() override { return "ClientServerStreamRpcTag"; }

  virtual void Process() override = 0;

  virtual void Finish() {
    status_ = bazel_template::common::grpc_framework::ClientRPCStatus::FINISH;
    rpc_->Finish(&rpc_status_, this);
  }

  virtual void OnError() override {
    status_ = ClientRPCStatus::FINISH;
    client_context_.TryCancel();
    Process();
  }

  virtual void OnRead(void *req_ptr) override = 0;

  virtual void OnReadError() override = 0;

 protected:
  grpc::ClientContext client_context_;
  ClientRPCStatus status_;
  grpc::Status rpc_status_;

  std::unique_ptr<grpc::ClientAsyncReader<ResponseType>> rpc_;
  typedef Reader<ResponseType, grpc::ClientAsyncReader<ResponseType>>
      ReaderType;
  std::unique_ptr<ReaderType> reader_;
  RequestType request_;
};

template <typename RequestType, typename ResponseType>
class ClientBiStreamRpcTag : public TagBase,
                             public ReaderCallback,
                             public WriterCallback {
 public:
  ClientBiStreamRpcTag() : can_call_(false) {
    status_ = ClientRPCStatus::CREATE;
  };

  virtual ~ClientBiStreamRpcTag() {}

  virtual std::string Name() override { return "ClientBiStreamRpcTag"; }

  virtual void Process() override = 0;

  virtual void Finish() override {
    status_ = bazel_template::common::grpc_framework::ClientRPCStatus::FINISH;
    rpc_->Finish(&response_, &rpc_status_, this);
  }

  virtual void OnError() override {
    status_ = ClientRPCStatus::FINISH;
    client_context_.TryCancel();
    Process();
  }

  virtual bool Call(const RequestType *req) {
    LOG(INFO) << "Call";
    if (!can_call_) {
      return false;
    }
    if (req == nullptr) {
      writer_->Write(nullptr);
      return true;
    }
    RequestType *request = writer_->NewRequest();
    request->CopyFrom(*req);
    // LOG(INFO) << "request: \n"
    //<< bazel_template::util::CommonUtil::MessageToJsonString(*request);
    writer_->Write(request);
    return true;
  }

  virtual void OnRead(void *req_ptr) override = 0;

  virtual void OnReadError() override = 0;

  virtual void OnWrite(int write_id) override = 0;

  virtual void OnWriteError() override {
    writer_->Stop();
    OnError();
  }

 protected:
  grpc::ClientContext client_context_;
  ClientRPCStatus status_;
  grpc::Status rpc_status_;

  std::unique_ptr<grpc::ClientAsyncReaderWriter<RequestType, ResponseType>>
      rpc_;
  typedef Reader<ResponseType,
                 grpc::ClientAsyncReaderWriter<RequestType, ResponseType>>
      ReaderType;

  typedef Writer<RequestType,
                 grpc::ClientAsyncReaderWriter<RequestType, ResponseType>>
      WriterType;
  std::unique_ptr<ReaderType> reader_;
  std::unique_ptr<WriterType> writer_;
  ResponseType response_;
  bool can_call_;
};

}  // namespace grpc_framework
}  // namespace common
}  // namespace bazel_template

#endif  // GRPC_FRAMEWORK_CLIENT_GRPC_TAG_H
