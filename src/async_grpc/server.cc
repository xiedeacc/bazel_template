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

#include <chrono>
#include <cstddef>
#include <exception>
#include <ranges>
#include <stop_token>
#include <string_view>
#include <tuple>
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

#if BUILD_TRACING
#include "opencensus/exporters/trace/stackdriver/stackdriver_exporter.h"
#include "opencensus/trace/trace_config.h"
#endif

module bazel_template.async_grpc;

namespace async_grpc {

void Server::Builder::CheckHandlerCompatibility(
    const std::string& service_full_name, const std::string& method_name,
    const google::protobuf::Descriptor* request_type,
    const google::protobuf::Descriptor* response_type,
    ::grpc::internal::RpcMethod::RpcType stream_type) {
  const auto* pool = google::protobuf::DescriptorPool::generated_pool();
  const auto* service = pool->FindServiceByName(service_full_name);
  CHECK(service) << "Unknown service " << service_full_name;
  const auto* method_descriptor = service->FindMethodByName(method_name);
  CHECK(method_descriptor) << "Unknown method " << method_name << " in service "
                           << service_full_name;
  CHECK_EQ(request_type, method_descriptor->input_type());
  CHECK_EQ(response_type, method_descriptor->output_type());
  using enum ::grpc::internal::RpcMethod::RpcType;
  switch (stream_type) {
    case NORMAL_RPC:
      CHECK(!method_descriptor->client_streaming());
      CHECK(!method_descriptor->server_streaming());
      break;
    case CLIENT_STREAMING:
      CHECK(method_descriptor->client_streaming());
      CHECK(!method_descriptor->server_streaming());
      break;
    case SERVER_STREAMING:
      CHECK(!method_descriptor->client_streaming());
      CHECK(method_descriptor->server_streaming());
      break;
    case BIDI_STREAMING:
      CHECK(method_descriptor->client_streaming());
      CHECK(method_descriptor->server_streaming());
      break;
  }
}
namespace {

constexpr std::chrono::milliseconds kPopEventTimeout{100};
// How long Shutdown() lets in-flight RPCs run before gRPC cancels them.
constexpr std::chrono::seconds kShutdownGracePeriod{2};
// How long Shutdown() then waits for those cancelled RPCs to retire.
constexpr std::chrono::milliseconds kQuiesceTimeout{5000};
constexpr unsigned int kDefaultTracingMaxAttributes = 128;
constexpr unsigned int kDefaultTracingMaxAnnotations = 128;
constexpr unsigned int kDefaultTracingMaxMessageEvents = 128;
constexpr unsigned int kDefaultTracingMaxLinks = 128;

}  // namespace

void Server::Builder::SetNumGrpcThreads(const size_t num_grpc_threads) {
  options_.num_grpc_threads = num_grpc_threads;
}

void Server::Builder::SetNumEventThreads(const std::size_t num_event_threads) {
  options_.num_event_threads = num_event_threads;
}

void Server::Builder::SetServerAddress(std::string_view server_address) {
  options_.server_address = server_address;
}

void Server::Builder::SetMaxReceiveMessageSize(int max_receive_message_size) {
  CHECK_GT(max_receive_message_size, 0)
      << "max_receive_message_size must be larger than 0.";
  options_.max_receive_message_size = max_receive_message_size;
}

void Server::Builder::SetMaxSendMessageSize(int max_send_message_size) {
  CHECK_GT(max_send_message_size, 0)
      << "max_send_message_size must be larger than 0.";
  options_.max_send_message_size = max_send_message_size;
}

void Server::Builder::EnableTracing() {
#if BUILD_TRACING
  options_.enable_tracing = true;
#else
  LOG(FATAL) << "Enable tracing support by compiling with -DBUILD_TRACING=1.";
#endif
}

void Server::Builder::DisableTracing() {
  options_.enable_tracing = false;
}

void Server::Builder::SetTracingSamplerProbability(
    double tracing_sampler_probability) {
  options_.tracing_sampler_probability = tracing_sampler_probability;
}

void Server::Builder::SetTracingTaskName(std::string_view tracing_task_name) {
  options_.tracing_task_name = tracing_task_name;
}

void Server::Builder::SetTracingGcpProjectId(
    std::string_view tracing_gcp_project_id) {
  options_.tracing_gcp_project_id = tracing_gcp_project_id;
}

std::tuple<std::string, std::string> Server::Builder::ParseMethodFullName(
    std::string_view method_full_name) {
  CHECK(method_full_name.starts_with('/')) << "Invalid method name.";
  const std::string_view rest = method_full_name.substr(1);
  const auto service_end = rest.find('/');
  CHECK_NE(service_end, std::string_view::npos) << "Invalid method name.";
  const std::string_view service_full_name = rest.substr(0, service_end);
  std::string_view method_name = rest.substr(service_end + 1);
  if (const auto method_end = method_name.find('/');
      method_end != std::string_view::npos) {
    method_name = method_name.substr(0, method_end);
  }
  CHECK(!service_full_name.empty() && !method_name.empty());
  return {std::string(service_full_name), std::string(method_name)};
}

std::unique_ptr<Server> Server::Builder::Build() {
  std::unique_ptr<Server> server(new Server(options_));
  for (const auto& [service_name, handlers] : rpc_handlers_) {
    server->AddService(service_name, handlers);
  }
  return server;
}

Server::Server(Options options) : options_(std::move(options)) {
  server_builder_.AddListeningPort(options_.server_address,
                                   ::grpc::InsecureServerCredentials());

  // Set max message sizes.
  server_builder_.SetMaxReceiveMessageSize(options_.max_receive_message_size);
  server_builder_.SetMaxSendMessageSize(options_.max_send_message_size);

  // Set up event queue threads.
  event_queue_threads_ =
      std::vector<EventQueueThread>(options_.num_event_threads);

  // Set up completion queues threads.
  completion_queue_threads_.reserve(options_.num_grpc_threads);
  for ([[maybe_unused]] const std::size_t i :
       std::views::iota(std::size_t{0}, options_.num_grpc_threads)) {
    completion_queue_threads_.emplace_back(
        server_builder_.AddCompletionQueue());
  }
}

void Server::AddService(
    const std::string& service_name,
    const std::map<std::string, RpcHandlerInfo>& rpc_handler_infos) {
  // Instantiate and register service.
  const auto result = services_.emplace(
      std::piecewise_construct, std::make_tuple(service_name),
      std::make_tuple(service_name, rpc_handler_infos,
                      [this]() { return SelectNextEventQueueRoundRobin(); }));
  CHECK(result.second) << "A service named " << service_name
                       << " already exists.";
  server_builder_.RegisterService(&result.first->second);
}

void Server::RunCompletionQueue(
    ::grpc::ServerCompletionQueue* completion_queue) {
  bool ok = false;
  void* tag = nullptr;
  while (completion_queue->Next(&tag, &ok)) {
    auto* rpc_event = static_cast<Rpc::CompletionQueueRpcEvent*>(tag);
    rpc_event->ok = ok;
    rpc_event->PushToEventQueue();
  }
}

EventQueue* Server::SelectNextEventQueueRoundRobin() {
  common::MutexLocker locker(&current_event_queue_id_lock_);
  current_event_queue_id_ =
      (current_event_queue_id_ + 1) % options_.num_event_threads;
  return event_queue_threads_.at(current_event_queue_id_).event_queue();
}

void Server::RunEventQueue(EventQueue* event_queue,
                           const std::stop_token& stop_token) {
  while (!stop_token.stop_requested()) {
    Rpc::UniqueEventPtr rpc_event =
        event_queue->PopWithTimeout(kPopEventTimeout);
    if (rpc_event) {
      rpc_event->Handle();
    }
  }

  // Finish processing the rest of the items.
  while (Rpc::UniqueEventPtr rpc_event =
             event_queue->PopWithTimeout(kPopEventTimeout)) {
    rpc_event->Handle();
  }
}

void Server::Start() {
#if BUILD_TRACING
  if (options_.enable_tracing) {
    opencensus::exporters::trace::StackdriverExporter::Register(
        options_.tracing_gcp_project_id);
    opencensus::trace::TraceConfig::SetCurrentTraceParams(
        {kDefaultTracingMaxAttributes, kDefaultTracingMaxAnnotations,
         kDefaultTracingMaxMessageEvents, kDefaultTracingMaxLinks,
         opencensus::trace::ProbabilitySampler(
             options_.tracing_sampler_probability)});
  }
#endif

  // Start the gRPC server process.
  server_ = server_builder_.BuildAndStart();
  CHECK(server_) << "Failed to start a gRPC server on "
                 << options_.server_address;

  // Start serving all services on all completion queues.
  for (auto& service : services_ | std::views::values) {
    service.StartServing(completion_queue_threads_, execution_context_.get());
  }

  // Start threads to process all event queues.
  for (auto& event_queue_thread : event_queue_threads_) {
    event_queue_thread.Start(
        [](EventQueue* event_queue, const std::stop_token& stop_token) {
          RunEventQueue(event_queue, stop_token);
        });
  }

  // Start threads to process all completion queues.
  for (auto& completion_queue_threads : completion_queue_threads_) {
    completion_queue_threads.Start(
        [this](::grpc::ServerCompletionQueue* completion_queue) {
          RunCompletionQueue(completion_queue);
        });
  }
}

void Server::WaitForShutdown() {
  if (!server_) {
    return;
  }

  server_->Wait();
}

void Server::Shutdown() {
  if (is_shut_down_ || !server_) {
    // Never started, or shut down already. Shutdown() is idempotent because
    // the destructor calls it.
    return;
  }
  is_shut_down_ = true;
  LOG(INFO) << "Shutting down server.";

  // The order below matters, and the guiding rule is that a thread may only be
  // stopped once nothing can feed it any more. Events flow
  //   gRPC -> completion queue thread -> event queue -> event queue thread,
  // and handling an event can put new work back onto a completion queue, so
  // the event queue threads have to outlive the completion queue threads,
  // which in turn have to outlive the gRPC server.

  // 1. Tell the services to stop serving RPCs.
  for (auto& service : services_ | std::views::values) {
    service.StopServing();
  }

  // 2. Shut down the gRPC server. This fails every requested-but-unmatched
  //    call right away (their tags come back with 'ok == false') and waits for
  //    the in-flight ones until the hard deadline, after which it cancels
  //    them. The event queue threads are deliberately still running: they are
  //    what finishes those in-flight calls, and without them this call would
  //    block forever.
  server_->Shutdown(std::chrono::system_clock::now() + kShutdownGracePeriod);

  // 3. Let every RPC retire itself now that the completion queues are still
  //    being polled. After this, no handler is going to enqueue new work on a
  //    completion queue.
  for (auto& [service_name, service] : services_) {
    if (const std::size_t remaining =
            service.WaitUntilQuiescent(kQuiesceTimeout);
        remaining > 0) {
      LOG(ERROR) << "Service " << service_name << " still has " << remaining
                 << " RPC(s) in flight after shutdown.";
    }
  }

  // 4. Shut down the server completion queues and wait for the processing
  //    threads to join. Every remaining gRPC tag is delivered before
  //    'Next()' returns false, so this pushes the last events onto the event
  //    queues.
  for (auto& completion_queue_threads : completion_queue_threads_) {
    completion_queue_threads.Shutdown();
  }

  // 5. Only now stop the event queue threads. RunEventQueue() drains whatever
  //    is left in its queue before returning, and nothing can push any more.
  for (auto& event_queue_thread : event_queue_threads_) {
    event_queue_thread.Shutdown();
  }

  LOG(INFO) << "Shutdown complete.";
}

Server::~Server() {
  // A Server that goes out of scope without an explicit Shutdown() would
  // otherwise hang joining its completion queue threads, which sit in
  // 'ServerCompletionQueue::Next()' until the queue is shut down. Shutdown()
  // logs and waits on condition variables, either of which can throw, and a
  // destructor must not let that out.
  try {
    Shutdown();
  } catch (const std::exception& e) {
    bazel_template::logging::ReportException("async_grpc::Server::~Server",
                                             e.what());
  } catch (...) {
    bazel_template::logging::ReportException("async_grpc::Server::~Server",
                                             nullptr);
  }
}

void Server::SetExecutionContext(
    std::shared_ptr<ExecutionContext> execution_context) {
  // After the server has been started the 'ExecutionHandle' cannot be changed
  // anymore.
  CHECK(!server_);
  execution_context_ = std::move(execution_context);
}

}  // namespace async_grpc
