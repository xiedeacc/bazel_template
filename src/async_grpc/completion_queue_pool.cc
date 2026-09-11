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

module;

#include <cstdlib>
#include <random>

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

void CompletionQueue::Start() {
  CHECK(!thread_.joinable()) << "CompletionQueue already started.";
  thread_ = std::jthread([this]() { RunCompletionQueue(); });
}

void CompletionQueue::Shutdown() {
  CHECK(thread_.joinable()) << "CompletionQueue not yet started.";
  LOG(INFO) << "Shutting down client completion queue " << this;
  // The queue's own Shutdown() is what makes Next() return false and ends the
  // thread; a stop token would not be observed inside Next().
  completion_queue_->Shutdown();
  thread_.join();
}

void CompletionQueue::RunCompletionQueue() {
  bool ok = false;
  void* tag = nullptr;
  while (completion_queue_->Next(&tag, &ok)) {
    auto* client_event = static_cast<ClientEvent*>(tag);
    client_event->ok = ok;
    client_event->async_client->HandleEvent(*client_event);
  }
}

CompletionQueuePool* CompletionQueuePool::completion_queue_pool() {
  static auto* const kInstance = new CompletionQueuePool();
  return kInstance;
}

void CompletionQueuePool::SetNumberCompletionQueues(
    std::size_t number_completion_queues) {
  CompletionQueuePool* pool = completion_queue_pool();
  CHECK(!pool->initialized_)
      << "Can't change number of completion queues after initialization.";
  CHECK_GT(number_completion_queues, 0U);
  pool->number_completion_queues_ = number_completion_queues;
}

::grpc::CompletionQueue* CompletionQueuePool::GetCompletionQueue() {
  CompletionQueuePool* pool = completion_queue_pool();
  pool->Initialize();
  // rand() is neither well distributed nor thread-safe; this picks a queue
  // per thread from a properly seeded engine.
  static thread_local std::mt19937 engine{std::random_device{}()};
  const std::size_t qid = std::uniform_int_distribution<std::size_t>{
      0, pool->completion_queues_.size() - 1}(engine);
  return pool->completion_queues_.at(qid).completion_queue();
}

void CompletionQueuePool::Start() {
  CompletionQueuePool* pool = completion_queue_pool();
  pool->Initialize();
}

void CompletionQueuePool::Shutdown() {
  LOG(INFO) << "Shutting down CompletionQueuePool";
  CompletionQueuePool* pool = completion_queue_pool();
  common::MutexLocker locker(&pool->mutex_);
  for (auto& completion_queue : pool->completion_queues_) {
    completion_queue.Shutdown();
  }
  pool->completion_queues_.clear();
  pool->initialized_ = false;
}

CompletionQueuePool::CompletionQueuePool() = default;

CompletionQueuePool::~CompletionQueuePool() {
  // Logging allocates, so it can throw, and a destructor is implicitly
  // noexcept -- an escaping exception would terminate the process.
  try {
    LOG(INFO) << "~CompletionQueuePool";
  } catch (...) {  // NOLINT(bugprone-empty-catch)
  }
}

void CompletionQueuePool::Initialize() {
  common::MutexLocker locker(&mutex_);
  if (initialized_) {
    return;
  }
  completion_queues_.resize(number_completion_queues_);
  for (auto& completion_queue : completion_queues_) {
    completion_queue.Start();
  }
  initialized_ = true;
}

}  // namespace async_grpc
