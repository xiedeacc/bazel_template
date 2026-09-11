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

CompletionQueueThread::CompletionQueueThread(
    std::unique_ptr<::grpc::ServerCompletionQueue> completion_queue)
    : completion_queue_(std::move(completion_queue)) {}

::grpc::ServerCompletionQueue* CompletionQueueThread::completion_queue() {
  return completion_queue_.get();
}

void CompletionQueueThread::Start(const CompletionQueueRunner& runner) {
  CHECK(!worker_thread_.joinable());
  worker_thread_ =
      std::jthread([this, runner]() { runner(completion_queue_.get()); });
}

void CompletionQueueThread::Shutdown() {
  LOG(INFO) << "Shutting down completion queue " << completion_queue_.get();
  // The queue's own Shutdown() is what makes Next() return false and ends the
  // thread; a stop token would not be observed inside Next().
  completion_queue_->Shutdown();
  if (worker_thread_.joinable()) {
    worker_thread_.join();
  }
}

}  // namespace async_grpc
