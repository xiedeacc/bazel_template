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

#include <cstdlib>
#include <ranges>
#include <string_view>
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

Service::Service(std::string_view /* service_name */,
                 const std::map<std::string, RpcHandlerInfo>& rpc_handler_infos,
                 EventQueueSelector event_queue_selector)
    : rpc_handler_infos_(rpc_handler_infos),
      event_queue_selector_(std::move(event_queue_selector)) {
  for (const auto& handler_info : rpc_handler_infos_ | std::views::values) {
    // The 'handler' below is set to 'nullptr' indicating that we want to
    // handle this method asynchronously.
    this->AddMethod(new ::grpc::internal::RpcServiceMethod(
        handler_info.fully_qualified_name.c_str(), handler_info.rpc_type,
        nullptr /* handler */));
  }
}

void Service::StartServing(
    std::vector<CompletionQueueThread>& completion_queue_threads,
    ExecutionContext* execution_context) {
  // The method index is the position of the method in 'rpc_handler_infos_',
  // matching the order in which the constructor registered them.
  int method_index = 0;
  for (const auto& handler_info : rpc_handler_infos_ | std::views::values) {
    for (auto& completion_queue_thread : completion_queue_threads) {
      const std::shared_ptr<Rpc> rpc = active_rpcs_.Add(std::make_unique<Rpc>(
          method_index, completion_queue_thread.completion_queue(),
          event_queue_selector_(), execution_context, handler_info, this,
          active_rpcs_.GetWeakPtrFactory()));
      rpc->RequestNextMethodInvocation();
    }
    ++method_index;
  }
}

void Service::StopServing() {
  shutting_down_ = true;
}

void Service::HandleEvent(Rpc::Event event, Rpc* rpc, bool ok) {
  using enum Rpc::Event;
  switch (event) {
    case NEW_CONNECTION:
      HandleNewConnection(rpc, ok);
      break;
    case READ:
      HandleRead(rpc, ok);
      break;
    case WRITE_NEEDED:
    case WRITE:
      HandleWrite(rpc, ok);
      break;
    case FINISH:
      HandleFinish(rpc, ok);
      break;
    case DONE:
      HandleDone(rpc, ok);
      break;
  }
}

void Service::HandleNewConnection(Rpc* rpc, bool ok) {
  if (!ok) {
    // gRPC only reports a requested call as failed when the server is shutting
    // down: this listening RPC was never matched with a call. Because it was
    // never matched, the tag armed by AsyncNotifyWhenDone() will never be
    // delivered either, so drop that expectation before retiring the RPC --
    // otherwise it would look in-flight forever and stay in 'active_rpcs_'.
    // There is also nothing to listen for any more, so do not clone.
    rpc->DiscardNotifyWhenDone();
    RemoveIfNotPending(rpc);
    return;
  }

  if (shutting_down_) {
    LOG(WARNING) << "Server shutting down. Refusing to handle new RPCs.";
    // The call *was* matched, so gRPC still owes us its DONE tag and holds a
    // reference to this RPC's ServerContext: retiring the RPC here would free
    // memory that the completion queue is about to hand back. Reject the call
    // instead and let the regular FINISH/DONE path remove it. Rejecting also
    // unblocks ::grpc::Server::Shutdown(), which waits for in-flight calls.
    rpc->Finish(::grpc::Status(::grpc::StatusCode::UNAVAILABLE,
                               "Server is shutting down."));
    return;
  }

  rpc->OnConnection();

  // Create new active rpc to handle next connection and register it for the
  // incoming connection. Assign event queue in a round-robin fashion.
  std::unique_ptr<Rpc> new_rpc = rpc->Clone();
  new_rpc->SetEventQueue(event_queue_selector_());
  active_rpcs_.Add(std::move(new_rpc))->RequestNextMethodInvocation();
}

void Service::HandleRead(Rpc* rpc, bool ok) {
  if (ok) {
    rpc->OnRequest();
    rpc->RequestStreamingReadIfNeeded();
    return;
  }

  // Reads completed.
  rpc->OnReadsDone();

  RemoveIfNotPending(rpc);
}

void Service::HandleWrite(Rpc* rpc, bool ok) {
  if (!ok) {
    LOG(ERROR) << "Write failed";
  }

  // Send the next message or potentially finish the connection.
  rpc->HandleSendQueue();

  RemoveIfNotPending(rpc);
}

void Service::HandleFinish(Rpc* rpc, bool ok) {
  if (!ok) {
    LOG(ERROR) << "Finish failed";
  }

  rpc->OnFinish();

  RemoveIfNotPending(rpc);
}

void Service::HandleDone(Rpc* rpc, bool /*ok*/) {
  RemoveIfNotPending(rpc);
}

std::size_t Service::WaitUntilQuiescent(
    const std::chrono::milliseconds timeout) {
  return active_rpcs_.WaitUntilEmpty(timeout);
}

void Service::RemoveIfNotPending(Rpc* rpc) {
  if (!rpc->IsAnyEventPending()) {
    active_rpcs_.Remove(rpc);
  }
}

}  // namespace async_grpc
