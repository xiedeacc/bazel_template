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

#include "src/async_grpc/event_queue_thread.h"

#include "glog/logging.h"

namespace async_grpc {

EventQueueThread::EventQueueThread() {
  event_queue_ = std::make_unique<EventQueue>();
}

EventQueue* EventQueueThread::event_queue() { return event_queue_.get(); }

void EventQueueThread::Start(EventQueueRunner runner) {
  CHECK(!thread_);
  EventQueue* event_queue = event_queue_.get();
  thread_ = std::make_unique<std::thread>(
      [event_queue, runner]() { runner(event_queue); });
}

void EventQueueThread::Shutdown() {
  LOG(INFO) << "Shutting down event queue " << event_queue_.get();
  thread_->join();
}

}  // namespace async_grpc
