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

module;

#include <memory>
#include <utility>

#include "google/protobuf/descriptor.h"
#include "google/protobuf/message.h"
#include "grpc++/grpc++.h"
#include "grpc++/impl/codegen/async_stream.h"
#include "grpc++/impl/codegen/async_unary_call.h"
#include "grpc++/impl/codegen/proto_utils.h"
#include "grpc++/impl/codegen/service_type.h"
#include "src/async_grpc/common/blocking_queue.h"
#include "src/async_grpc/common/mutex.h"
#include "src/async_grpc/common/time.h"
#include "src/async_grpc/rpc_service_method_traits.h"
#include "src/async_grpc/type_traits.h"
#include "src/common/logging.h"
#include "src/util/util_fwd.h"

module bazel_template.async_grpc;

namespace async_grpc {

Rpc::~Rpc() = default;
namespace {

// Finishes the gRPC for non-streaming response RPCs, i.e. NORMAL_RPC and
// CLIENT_STREAMING. If no 'msg' is passed, we signal an error to the client as
// the server is not honoring the gRPC call signature.
template <typename ReaderWriter>
void SendUnaryFinish(ReaderWriter* reader_writer, const ::grpc::Status& status,
                     const google::protobuf::Message* msg,
                     Rpc::EventBase* rpc_event) {
  if (msg) {
    reader_writer->Finish(*msg, status, rpc_event);
  } else {
    reader_writer->FinishWithError(status, rpc_event);
  }
}

}  // namespace

void Rpc::CompletionQueueRpcEvent::Handle() {
  pending = false;
  rpc_ptr->service()->HandleEvent(event, rpc_ptr, ok);
}

void Rpc::InternalRpcEvent::Handle() {
  if (auto rpc_shared = rpc.lock()) {
    rpc_shared->service()->HandleEvent(event, rpc_shared.get(), true);
  } else {
    LOG(WARNING) << "Ignoring stale event.";
  }
}

Rpc::Rpc(int method_index,
         ::grpc::ServerCompletionQueue* server_completion_queue,
         EventQueue* event_queue, ExecutionContext* execution_context,
         const RpcHandlerInfo& rpc_handler_info, Service* service,
         WeakPtrFactory weak_ptr_factory)
    : method_index_(method_index),
      server_completion_queue_(server_completion_queue),
      event_queue_(event_queue),
      execution_context_(execution_context),
      rpc_handler_info_(rpc_handler_info),
      service_(service),
      weak_ptr_factory_(std::move(weak_ptr_factory)),
      new_connection_event_(Event::NEW_CONNECTION, this),
      read_event_(Event::READ, this),
      write_event_(Event::WRITE, this),
      finish_event_(Event::FINISH, this),
      done_event_(Event::DONE, this) {
  InitializeReadersAndWriters(rpc_handler_info_.rpc_type);

  // Initialize the prototypical request and response messages.
  request_.reset(::google::protobuf::MessageFactory::generated_factory()
                     ->GetPrototype(rpc_handler_info_.request_descriptor)
                     ->New());
  response_.reset(::google::protobuf::MessageFactory::generated_factory()
                      ->GetPrototype(rpc_handler_info_.response_descriptor)
                      ->New());
}

std::unique_ptr<Rpc> Rpc::Clone() {
  return std::make_unique<Rpc>(method_index_, server_completion_queue_,
                               event_queue_, execution_context_,
                               rpc_handler_info_, service_, weak_ptr_factory_);
}

void Rpc::OnConnection() {
  if (!handler_) {
    // Instantiate the handler.
    handler_ = rpc_handler_info_.rpc_handler_factory(this, execution_context_);
  }

  // For request-streaming RPCs ask the client to start sending requests.
  RequestStreamingReadIfNeeded();
}

void Rpc::OnRequest() {
  if (!handler_) {
    return;
  }
  handler_->OnRequestInternal(request_.get());
}

void Rpc::OnReadsDone() {
  if (!handler_) {
    return;
  }
  handler_->OnReadsDone();
}

void Rpc::OnFinish() {
  // An RPC that is finished before it was ever connected -- the server was
  // already shutting down when gRPC matched the call -- has no handler.
  if (!handler_) {
    return;
  }
  handler_->OnFinish();
}

void Rpc::RequestNextMethodInvocation() {
  // Ask gRPC to notify us when the connection terminates. gRPC only ever
  // delivers this tag for a call it actually matched: AsyncNotifyWhenDone()
  // just stores the tag, and the completion op that eventually posts it is
  // created when the request below is fulfilled. If instead the request comes
  // back with 'ok == false' -- which is how gRPC reports that the server is
  // shutting down -- the tag is dropped, and Service::HandleNewConnection()
  // has to call DiscardNotifyWhenDone() so this Rpc does not stay marked as
  // in-flight (and hence registered in ActiveRpcs) forever.
  SetRpcEventState(Event::DONE, true);
  server_context_.AsyncNotifyWhenDone(GetRpcEvent(Event::DONE));

  // Make sure after terminating the connection, gRPC notifies us with this
  // event.
  SetRpcEventState(Event::NEW_CONNECTION, true);
  using enum ::grpc::internal::RpcMethod::RpcType;
  switch (rpc_handler_info_.rpc_type) {
    case BIDI_STREAMING:
      service_->RequestAsyncBidiStreaming(
          method_index_, &server_context_, streaming_interface(),
          server_completion_queue_, server_completion_queue_,
          GetRpcEvent(Event::NEW_CONNECTION));
      break;
    case CLIENT_STREAMING:
      service_->RequestAsyncClientStreaming(
          method_index_, &server_context_, streaming_interface(),
          server_completion_queue_, server_completion_queue_,
          GetRpcEvent(Event::NEW_CONNECTION));
      break;
    case NORMAL_RPC:
      service_->RequestAsyncUnary(
          method_index_, &server_context_, request_.get(),
          streaming_interface(), server_completion_queue_,
          server_completion_queue_, GetRpcEvent(Event::NEW_CONNECTION));
      break;
    case SERVER_STREAMING:
      service_->RequestAsyncServerStreaming(
          method_index_, &server_context_, request_.get(),
          streaming_interface(), server_completion_queue_,
          server_completion_queue_, GetRpcEvent(Event::NEW_CONNECTION));
      break;
  }
}

void Rpc::RequestStreamingReadIfNeeded() {
  // For request-streaming RPCs ask the client to start sending requests.
  using enum ::grpc::internal::RpcMethod::RpcType;
  switch (rpc_handler_info_.rpc_type) {
    case BIDI_STREAMING:
    case CLIENT_STREAMING:
      SetRpcEventState(Event::READ, true);
      async_reader_interface()->Read(request_.get(), GetRpcEvent(Event::READ));
      break;
    case NORMAL_RPC:
    case SERVER_STREAMING:
      // For NORMAL_RPC and SERVER_STREAMING we don't need to queue an event,
      // since gRPC automatically issues a READ request and places the request
      // into the 'Message' we provided to 'RequestAsyncUnary' above.
      OnRequest();
      OnReadsDone();
      break;
  }
}

void Rpc::Write(std::unique_ptr<::google::protobuf::Message> message) {
  EnqueueMessage(
      SendItem{.msg = std::move(message), .status = ::grpc::Status::OK});
  event_queue_->Push(UniqueEventPtr(
      new InternalRpcEvent(Event::WRITE_NEEDED, weak_ptr_factory_(this)),
      EventDeleter(EventDeleter::Action::DEL)));
}

void Rpc::Finish(::grpc::Status status) {
  EnqueueMessage(
      SendItem{.msg = nullptr /* message */, .status = std::move(status)});
  event_queue_->Push(UniqueEventPtr(
      new InternalRpcEvent(Event::WRITE_NEEDED, weak_ptr_factory_(this)),
      EventDeleter(EventDeleter::Action::DEL)));
}

void Rpc::HandleSendQueue() {
  SendItem send_item;
  {
    common::MutexLocker locker(&send_queue_lock_);
    if (send_queue_.empty() || IsRpcEventPending(Event::WRITE) ||
        IsRpcEventPending(Event::FINISH)) {
      return;
    }

    send_item = std::move(send_queue_.front());
    send_queue_.pop();
  }
  using enum ::grpc::internal::RpcMethod::RpcType;
  if (!send_item.msg || rpc_handler_info_.rpc_type == NORMAL_RPC ||
      rpc_handler_info_.rpc_type == CLIENT_STREAMING) {
    PerformFinish(std::move(send_item.msg), send_item.status);
    return;
  }
  PerformWrite(std::move(send_item.msg), send_item.status);
}

::grpc::internal::ServerAsyncStreamingInterface* Rpc::streaming_interface() {
  using enum ::grpc::internal::RpcMethod::RpcType;
  switch (rpc_handler_info_.rpc_type) {
    case BIDI_STREAMING:
      return server_async_reader_writer_.get();
    case CLIENT_STREAMING:
      return server_async_reader_.get();
    case NORMAL_RPC:
      return server_async_response_writer_.get();
    case SERVER_STREAMING:
      return server_async_writer_.get();
  }
  LOG(FATAL) << "Never reached.";
}

::grpc::internal::AsyncReaderInterface<::google::protobuf::Message>*
Rpc::async_reader_interface() {
  using enum ::grpc::internal::RpcMethod::RpcType;
  switch (rpc_handler_info_.rpc_type) {
    case BIDI_STREAMING:
      return server_async_reader_writer_.get();
    case CLIENT_STREAMING:
      return server_async_reader_.get();
    case NORMAL_RPC:
      LOG(FATAL) << "For NORMAL_RPC no streaming reader interface exists.";
    case SERVER_STREAMING:
      LOG(FATAL)
          << "For SERVER_STREAMING no streaming reader interface exists.";
  }
  LOG(FATAL) << "Never reached.";
}

::grpc::internal::AsyncWriterInterface<::google::protobuf::Message>*
Rpc::async_writer_interface() {
  using enum ::grpc::internal::RpcMethod::RpcType;
  switch (rpc_handler_info_.rpc_type) {
    case BIDI_STREAMING:
      return server_async_reader_writer_.get();
    case CLIENT_STREAMING:
    case NORMAL_RPC:
      LOG(FATAL) << "For NORMAL_RPC and CLIENT_STREAMING no streaming writer "
                    "interface exists.";
      break;
    case SERVER_STREAMING:
      return server_async_writer_.get();
  }
  LOG(FATAL) << "Never reached.";
}

Rpc::CompletionQueueRpcEvent* Rpc::GetRpcEvent(Event event) {
  using enum Event;
  switch (event) {
    case NEW_CONNECTION:
      return &new_connection_event_;
    case READ:
      return &read_event_;
    case WRITE_NEEDED:
      LOG(FATAL) << "Rpc does not store Event::WRITE_NEEDED.";
      break;
    case WRITE:
      return &write_event_;
    case FINISH:
      return &finish_event_;
    case DONE:
      return &done_event_;
  }
  LOG(FATAL) << "Never reached.";
}

bool* Rpc::GetRpcEventState(Event event) {
  return &GetRpcEvent(event)->pending;
}

void Rpc::EnqueueMessage(SendItem&& send_item) {
  common::MutexLocker locker(&send_queue_lock_);
  send_queue_.emplace(std::move(send_item));
}

void Rpc::PerformFinish(std::unique_ptr<::google::protobuf::Message> message,
                        const ::grpc::Status& status) {
  SetRpcEventState(Event::FINISH, true);
  using enum ::grpc::internal::RpcMethod::RpcType;
  switch (rpc_handler_info_.rpc_type) {
    case BIDI_STREAMING:
      CHECK(!message);
      server_async_reader_writer_->Finish(status, GetRpcEvent(Event::FINISH));
      break;
    case CLIENT_STREAMING:
      response_ = std::move(message);
      SendUnaryFinish(server_async_reader_.get(), status, response_.get(),
                      GetRpcEvent(Event::FINISH));
      break;
    case NORMAL_RPC:
      response_ = std::move(message);
      SendUnaryFinish(server_async_response_writer_.get(), status,
                      response_.get(), GetRpcEvent(Event::FINISH));
      break;
    case SERVER_STREAMING:
      CHECK(!message);
      server_async_writer_->Finish(status, GetRpcEvent(Event::FINISH));
      break;
  }
}

void Rpc::PerformWrite(std::unique_ptr<::google::protobuf::Message> message,
                       const ::grpc::Status& /* status */) {
  CHECK(message) << "PerformWrite must be called with a non-null message";
  CHECK_NE(rpc_handler_info_.rpc_type, ::grpc::internal::RpcMethod::NORMAL_RPC);
  CHECK_NE(rpc_handler_info_.rpc_type,
           ::grpc::internal::RpcMethod::CLIENT_STREAMING);
  SetRpcEventState(Event::WRITE, true);
  response_ = std::move(message);
  async_writer_interface()->Write(*response_, GetRpcEvent(Event::WRITE));
}

void Rpc::SetRpcEventState(Event event, bool pending) {
  // TODO(gaschler): Since the only usage is setting this true at creation,
  // consider removing this method.
  *GetRpcEventState(event) = pending;
}

bool Rpc::IsRpcEventPending(Event event) {
  return *GetRpcEventState(event);
}

void Rpc::DiscardNotifyWhenDone() {
  SetRpcEventState(Event::DONE, false);
}

bool Rpc::IsAnyEventPending() {
  using enum Event;
  return IsRpcEventPending(DONE) || IsRpcEventPending(READ) ||
         IsRpcEventPending(WRITE) || IsRpcEventPending(FINISH);
}

std::weak_ptr<Rpc> Rpc::GetWeakPtr() {
  return weak_ptr_factory_(this);
}

void Rpc::InitializeReadersAndWriters(
    ::grpc::internal::RpcMethod::RpcType rpc_type) {
  using enum ::grpc::internal::RpcMethod::RpcType;
  switch (rpc_type) {
    case BIDI_STREAMING:
      server_async_reader_writer_ =
          std::make_unique<::grpc::ServerAsyncReaderWriter<
              google::protobuf::Message, google::protobuf::Message>>(
              &server_context_);
      break;
    case CLIENT_STREAMING:
      server_async_reader_ = std::make_unique<::grpc::ServerAsyncReader<
          google::protobuf::Message, google::protobuf::Message>>(
          &server_context_);
      break;
    case NORMAL_RPC:
      server_async_response_writer_ = std::make_unique<
          ::grpc::ServerAsyncResponseWriter<google::protobuf::Message>>(
          &server_context_);
      break;
    case SERVER_STREAMING:
      server_async_writer_ = std::make_unique<
          ::grpc::ServerAsyncWriter<google::protobuf::Message>>(
          &server_context_);
      break;
  }
}

ActiveRpcs::~ActiveRpcs() {
  // Taking the lock and logging can both throw, and a destructor is
  // implicitly noexcept; what must not happen is an exception unwinding out
  // of this.
  //
  // An orderly Server::Shutdown() leaves this empty: every RPC retires itself
  // once its last gRPC tag has been handled. Anything left here means an RPC
  // was still waiting for a completion queue tag that never came, which is a
  // bug worth reporting -- but not worth aborting a process that is already on
  // its way out, so this logs instead of ending the program.
  try {
    common::MutexLocker locker(&lock_);
    if (!rpcs_.empty()) {
      LOG(ERROR) << "Destroying ActiveRpcs with " << rpcs_.size()
                 << " RPC(s) still in flight.";
    }
  } catch (...) {  // NOLINT(bugprone-empty-catch)
  }
}

std::shared_ptr<Rpc> ActiveRpcs::Add(std::unique_ptr<Rpc> rpc) {
  common::MutexLocker locker(&lock_);
  std::shared_ptr<Rpc> shared_ptr_rpc = std::move(rpc);
  const auto result = rpcs_.emplace(shared_ptr_rpc.get(), shared_ptr_rpc);
  CHECK(result.second) << "RPC already active.";
  return shared_ptr_rpc;
}

bool ActiveRpcs::Remove(Rpc* rpc) {
  common::MutexLocker locker(&lock_);
  return rpcs_.erase(rpc) > 0;
}

std::size_t ActiveRpcs::WaitUntilEmpty(
    const std::chrono::milliseconds timeout) {
  common::MutexLocker locker(&lock_);
  // Every Locker notifies the condition variable when it releases the lock, so
  // Remove() wakes this up.
  if (!locker.AwaitWithTimeout(
          [this]() REQUIRES(lock_) { return rpcs_.empty(); }, timeout)) {
    return rpcs_.size();
  }
  return 0;
}

Rpc::WeakPtrFactory ActiveRpcs::GetWeakPtrFactory() {
  return [this](Rpc* rpc) { return GetWeakPtr(rpc); };
}

std::weak_ptr<Rpc> ActiveRpcs::GetWeakPtr(Rpc* rpc) {
  common::MutexLocker locker(&lock_);
  auto it = rpcs_.find(rpc);
  CHECK(it != rpcs_.end());
  return it->second;
}

}  // namespace async_grpc
