/*******************************************************************************
 * Copyright (c) 2022 Copyright 2022- xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef GRPC_FRAMEWORK_GRPC_SERVER_IMPL_H
#define GRPC_FRAMEWORK_GRPC_SERVER_IMPL_H

#include <atomic>
#include <chrono>
#include <set>
#include <string>
#include <thread>

#include "absl/time/clock.h"
#include "boost/thread/thread_pool.hpp"
#include "glog/logging.h"
#include "grpc++/grpc++.h"
#include "grpc/support/log.h"
#include "grpc/support/time.h"
#include "src/common/grpc_framework/tag_base.h"

namespace bazel_template {
namespace common {
namespace grpc_framework {

template <typename ServiceType>
class ServerImpl {
 public:
  typedef ServerImpl<ServiceType> this_type;

  ServerImpl(int port) : port_(port) {}

  void Run() {
    grpc::ServerBuilder builder;
    const std::string addr = "[::]:" + std::to_string(port_);
    builder.AddListeningPort(addr, grpc::InsecureServerCredentials());
    builder.RegisterService(&service_);
    for (int i = 0; i < 6; ++i) {
      request_queues_.emplace_back(builder.AddCompletionQueue());
      response_queues_.emplace_back(builder.AddCompletionQueue());
    }
    server_ = builder.BuildAndStart();
    running_.store(true, std::memory_order_relaxed);
    LOG(INFO) << "server started on " << addr;

    handle_pool_ = new boost::executors::basic_thread_pool(6 * 2);
    for (int i = 0; i < 6; ++i) {
      handle_pool_->submit(
          boost::bind<void>(&this_type::HandleRequest, this, i));
      handle_pool_->submit(
          boost::bind<void>(&this_type::HandleResponse, this, i));
    }
    handle_pool_->interrupt_and_join();
  }

  virtual void OnRun(grpc::ServerCompletionQueue *request_queue,
                     grpc::ServerCompletionQueue *response_queue) = 0;

  void Exit() { running_.store(false, std::memory_order_relaxed); }

  virtual void OnExit() {}

 protected:
  void HandleRequest(int queue_id) {
    auto request_queue = request_queues_[queue_id].get();
    auto response_queue = response_queues_[queue_id].get();
    while (true) {
      if (!running_.load(std::memory_order_relaxed)) {
        request_queue->Shutdown();
        OnExit();
        return;
      }

      OnRun(request_queue, response_queue);
      void *got_tag;
      bool ok = false;
      while (request_queue->Next(&got_tag, &ok)) {
        TagBase *call = static_cast<TagBase *>(got_tag);
        LOG(INFO) << "tag is " << got_tag
                  << ", ok = " << (ok ? "true" : "false")
                  << ", name = " << call->Name();
        if (ok) {
          call->Process();
        } else {
          call->OnError();
        }
      }
      LOG(INFO) << "completion queue is shutting down. restart it...";
      std::this_thread::yield();
      std::this_thread::sleep_for(std::chrono::milliseconds(1000 * 10));
    }
  }

  void HandleResponse(int queue_id) {
    auto &response_queue = response_queues_[queue_id];
    while (true) {
      if (!running_.load(std::memory_order_relaxed)) {
        response_queue->Shutdown();
        OnExit();
        return;
      }

      void *got_tag;
      bool ok = false;
      while (response_queue->Next(&got_tag, &ok)) {
        TagBase *call = static_cast<TagBase *>(got_tag);
        LOG(INFO) << "tag is " << got_tag << ", ok = " << ok
                  << ", name = " << call->Name();
        if (ok) {
          call->Process();
        } else {
          call->OnError();
        }
      }
      LOG(INFO) << "completion queue is shutting down. restart it...";
      std::this_thread::yield();
      std::this_thread::sleep_for(std::chrono::milliseconds(1000 * 10));
    }
  }

 public:
  typename ServiceType::AsyncService service_;

 protected:
  std::unique_ptr<grpc::Server> server_;
  std::atomic<bool> running_;
  std::vector<std::unique_ptr<grpc::ServerCompletionQueue>> request_queues_;
  std::vector<std::unique_ptr<grpc::ServerCompletionQueue>> response_queues_;
  boost::executors::basic_thread_pool *handle_pool_;
  int port_;
};

}  // namespace grpc_framework
}  // namespace common
}  // namespace bazel_template

#endif  // GRPC_FRAMEWORK_GRPC_SERVER_IMPL_H
