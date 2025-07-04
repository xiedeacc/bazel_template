/*
 * Copyright 2017 The Cartographer Authors
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

#ifndef CPP_GRPC_RPC_HANDLER_H
#define CPP_GRPC_RPC_HANDLER_H

#include "glog/logging.h"
#include "google/protobuf/message.h"
#include "src/async_grpc/execution_context.h"
#include "src/async_grpc/rpc.h"
#include "src/async_grpc/rpc_handler_interface.h"
#include "src/async_grpc/rpc_service_method_traits.h"
#include "src/async_grpc/span.h"
#if BUILD_TRACING
#include "src/async_grpc/opencensus_span.h"
#endif

namespace async_grpc {

template <typename RpcServiceMethodConcept>
class RpcHandler : public RpcHandlerInterface {
 public:
  using RpcServiceMethod = RpcServiceMethodTraits<RpcServiceMethodConcept>;
  using RequestType = typename RpcServiceMethod::RequestType;
  using ResponseType = typename RpcServiceMethod::ResponseType;

  class Writer {
   public:
    explicit Writer(std::weak_ptr<Rpc> rpc) : rpc_(std::move(rpc)) {}
    bool Write(std::unique_ptr<ResponseType> message) const {
      if (auto rpc = rpc_.lock()) {
        rpc->Write(std::move(message));
        return true;
      }
      return false;
    }
    bool WritesDone() const {
      if (auto rpc = rpc_.lock()) {
        rpc->Finish(::grpc::Status::OK);
        return true;
      }
      return false;
    }
    bool Finish(const ::grpc::Status& status) const {
      if (auto rpc = rpc_.lock()) {
        rpc->Finish(status);
        auto* span = rpc->handler()->trace_span();
        if (span) {
          span->SetStatus(status);
        }
        return true;
      }
      return false;
    }

   private:
    const std::weak_ptr<Rpc> rpc_;
  };

#if BUILD_TRACING
  RpcHandler()
      : span_(
            OpencensusSpan::StartSpan(RpcServiceMethodConcept::MethodName())) {}
  virtual ~RpcHandler() { span_->End(); }
#endif

  Span* trace_span() override { return span_.get(); }
  void SetExecutionContext(ExecutionContext* execution_context) override {
    execution_context_ = execution_context;
  }
  void SetRpc(Rpc* rpc) override { rpc_ = rpc; }
  void OnRequestInternal(const ::google::protobuf::Message* request) override {
    DCHECK(dynamic_cast<const RequestType*>(request));
    OnRequest(static_cast<const RequestType&>(*request));
  }
  virtual void OnRequest(const RequestType& request) = 0;
  void Finish(::grpc::Status status) {
    rpc_->Finish(status);
#if BUILD_TRACING
    span_->SetStatus(status);
#endif
  }
  void Send(std::unique_ptr<ResponseType> response) {
    rpc_->Write(std::move(response));
  }
  template <typename T>
  ExecutionContext::Synchronized<T> GetContext() {
    return {execution_context_->lock(), execution_context_};
  }
  template <typename T>
  T* GetUnsynchronizedContext() {
    return dynamic_cast<T*>(execution_context_);
  }
  Writer GetWriter() { return Writer(rpc_->GetWeakPtr()); }

 protected:
  ExecutionContext* execution_context_;

 private:
  Rpc* rpc_;
  std::unique_ptr<Span> span_;
};

}  // namespace async_grpc

#endif  // CPP_GRPC_RPC_HANDLER_H
