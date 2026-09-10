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

#include <stop_token>
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

EventQueueThread::EventQueueThread() {
  event_queue_ = std::make_unique<EventQueue>();
}

EventQueue* EventQueueThread::event_queue() {
  return event_queue_.get();
}

void EventQueueThread::Start(const EventQueueRunner& runner) {
  CHECK(!thread_.joinable());
  EventQueue* event_queue = event_queue_.get();
  thread_ = std::jthread([event_queue, runner](std::stop_token stop_token) {
    runner(event_queue, std::move(stop_token));
  });
}

void EventQueueThread::RequestStop() {
  thread_.request_stop();
}

void EventQueueThread::Shutdown() {
  LOG(INFO) << "Shutting down event queue " << event_queue_.get();
  thread_.request_stop();
  if (thread_.joinable()) {
    thread_.join();
  }
}

}  // namespace async_grpc
