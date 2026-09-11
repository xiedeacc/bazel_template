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

// Module interface for the async_grpc server side (and the completion queue
// based asynchronous client). google::protobuf::Message, Descriptor and
// DescriptorPool are only forward-declared here: MSVC cannot write a BMI
// whose declarations reach protobuf's Message (see docs/cpp20_modules.md).
// Everything that needs the complete types lives in the implementation units
// or is a template instantiated by the importer, which includes the generated
// proto headers before importing this module.
module;

#include <chrono>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <queue>
#include <stop_token>
#include <string>
#include <string_view>
#include <thread>
#include <tuple>
#include <utility>
#include <vector>

#include "grpc++/grpc++.h"
#include "grpc++/impl/codegen/async_stream.h"
#include "grpc++/impl/codegen/async_unary_call.h"
#include "grpc++/impl/codegen/service_type.h"
#include "src/async_grpc/common/blocking_queue.h"
#include "src/async_grpc/common/mutex.h"
#include "src/async_grpc/common/time.h"
#include "src/async_grpc/rpc_service_method_traits.h"
#include "src/async_grpc/type_traits.h"
#include "src/common/logging.h"
#include "src/util/util_fwd.h"

#if BUILD_TRACING
#include "opencensus/trace/span.h"
#endif

export module bazel_template.async_grpc;

// ---- from execution_context.h
export namespace async_grpc {

// Implementations of this class allow RPC handlers to share state among one
// another. Using Server::SetExecutionContext(...) a server-wide
// 'ExecutionContext' can be specified. This 'ExecutionContext' can be retrieved
// by all implementations of 'RpcHandler' by calling
// 'RpcHandler::GetContext<MyContext>()'.
class ExecutionContext {
 public:
  // Automatically locks an ExecutionContext for shared use by RPC handlers.
  // This non-movable, non-copyable class is used to broker access from various
  // RPC handlers to the shared 'ExecutionContext'.
  template <typename ContextType>
  class Synchronized {
   public:
    [[nodiscard]] ContextType* operator->() {
      return static_cast<ContextType*>(execution_context_);
    }
    Synchronized(common::Mutex* lock, ExecutionContext* execution_context)
        : locker_(lock), execution_context_(execution_context) {}
    Synchronized(const Synchronized&) = delete;
    Synchronized(Synchronized&&) = delete;
    Synchronized& operator=(const Synchronized&) = delete;
    Synchronized& operator=(Synchronized&&) = delete;
    ~Synchronized() = default;

   private:
    common::MutexLocker locker_;
    ExecutionContext* execution_context_;
  };
  ExecutionContext() = default;
  virtual ~ExecutionContext() = default;

  // Rule of five: a virtual destructor suppresses the implicit move
  // operations, and copying through a base reference would slice.
  ExecutionContext(const ExecutionContext&) = delete;
  ExecutionContext& operator=(const ExecutionContext&) = delete;
  ExecutionContext(ExecutionContext&&) = delete;
  ExecutionContext& operator=(ExecutionContext&&) = delete;
  [[nodiscard]] common::Mutex* lock() { return &lock_; }

 private:
  common::Mutex lock_;
};

}  // namespace async_grpc

// ---- from span.h
export namespace async_grpc {

// Span represents a trace span. All implementations of the Span interface
// must be thread-safe.
class Span {
 public:
  Span() = default;
  virtual ~Span() = default;

  // Rule of five: a virtual destructor suppresses the implicit move
  // operations, and copying through a base reference would slice.
  Span(const Span&) = delete;
  Span& operator=(const Span&) = delete;
  Span(Span&&) = delete;
  Span& operator=(Span&&) = delete;

  // Creates a new child span with this span as the parent.
  [[nodiscard]] virtual std::unique_ptr<Span> CreateChildSpan(
      std::string_view name) = 0;

  // Sets the status of the Span. See status_code.h for canonical codes.
  virtual void SetStatus(const ::grpc::Status& status) = 0;

  // Marks the end of a Span. No further changes can be made to the Span after
  // End is called.
  virtual void End() = 0;
};

}  // namespace async_grpc

// ---- from opencensus_span.h
#if BUILD_TRACING

export namespace async_grpc {

// An implementation of the Span interface backed by Opencensus.
class OpencensusSpan : public Span {
 public:
  [[nodiscard]] static std::unique_ptr<Span> StartSpan(
      std::string_view name, const OpencensusSpan* parent = nullptr);

  [[nodiscard]] std::unique_ptr<Span> CreateChildSpan(
      std::string_view name) override;
  void SetStatus(const ::grpc::Status& status) override;
  void End() override;

 private:
  explicit OpencensusSpan(std::string_view name,
                          const OpencensusSpan* parent = nullptr);

  opencensus::trace::Span span_;
};

}  // namespace async_grpc

#endif  // BUILD_TRACING

// ---- from rpc_handler_interface.h
export namespace async_grpc {

class Rpc;
class RpcHandlerInterface {
 public:
  RpcHandlerInterface() = default;
  virtual ~RpcHandlerInterface() = default;

  // Rule of five: a virtual destructor suppresses the implicit move
  // operations, and copying through a base reference would slice.
  RpcHandlerInterface(const RpcHandlerInterface&) = delete;
  RpcHandlerInterface& operator=(const RpcHandlerInterface&) = delete;
  RpcHandlerInterface(RpcHandlerInterface&&) = delete;
  RpcHandlerInterface& operator=(RpcHandlerInterface&&) = delete;
  virtual void SetExecutionContext(ExecutionContext* execution_context) = 0;
  virtual void SetRpc(Rpc* rpc) = 0;
  virtual void Initialize() {};
  virtual void OnRequestInternal(
      const ::google::protobuf::Message* request) = 0;
  virtual void OnReadsDone() {};
  virtual void OnFinish() {};
  [[nodiscard]] virtual Span* trace_span() = 0;
  template <std::derived_from<RpcHandlerInterface> RpcHandlerType>
  [[nodiscard]] static std::unique_ptr<RpcHandlerType> Instantiate() {
    return std::make_unique<RpcHandlerType>();
  }
};

using RpcHandlerFactory = std::function<std::unique_ptr<RpcHandlerInterface>(
    Rpc*, ExecutionContext*)>;

struct RpcHandlerInfo {
  const google::protobuf::Descriptor* request_descriptor;
  const google::protobuf::Descriptor* response_descriptor;
  RpcHandlerFactory rpc_handler_factory;
  ::grpc::internal::RpcMethod::RpcType rpc_type;
  std::string fully_qualified_name;
};

}  // namespace async_grpc

// ---- from rpc.h
export namespace async_grpc {

class Service;
// TODO(cschuet): Add a unittest that tests the logic of this class.
class Rpc {
 public:
  using WeakPtrFactory = std::function<std::weak_ptr<Rpc>(Rpc*)>;
  enum class Event : std::uint8_t {
    NEW_CONNECTION = 0,
    READ,
    WRITE_NEEDED,
    WRITE,
    FINISH,
    DONE
  };

  struct EventBase {
    explicit EventBase(Event event) : event(event) {}
    virtual ~EventBase() = default;

    // Rule of five: the virtual destructor suppresses the implicit move
    // operations, and copying through a base reference would slice.
    EventBase(const EventBase&) = delete;
    EventBase& operator=(const EventBase&) = delete;
    EventBase(EventBase&&) = delete;
    EventBase& operator=(EventBase&&) = delete;
    virtual void Handle() = 0;

    Event event;
  };

  class EventDeleter {
   public:
    enum class Action : std::uint8_t { DEL = 0, DO_NOT_DELETE };

    // 'Action::DEL' is the default, so a default-constructed UniqueEventPtr
    // owns its event. The member is set in the constructor rather than with a
    // default member initializer: a nested class's default member
    // initializers are only parsed once the enclosing 'Rpc' is complete, so
    // an initializer here would leave EventDeleter, and therefore
    // UniqueEventPtr, non-default-constructible at the point 'EventQueue' is
    // declared below.
    EventDeleter() : action_(Action::DEL) {}
    explicit EventDeleter(Action action) : action_(action) {}
    void operator()(EventBase* e) {
      if (e != nullptr && action_ == Action::DEL) {
        delete e;
      }
    }

   private:
    Action action_;
  };

  using UniqueEventPtr = std::unique_ptr<EventBase, EventDeleter>;
  using EventQueue = common::BlockingQueue<UniqueEventPtr>;

  // Flows through gRPC's CompletionQueue and then our EventQueue.
  struct CompletionQueueRpcEvent : public EventBase {
    CompletionQueueRpcEvent(Event event, Rpc* rpc)
        : EventBase(event), rpc_ptr(rpc) {}
    void PushToEventQueue() {
      rpc_ptr->event_queue()->Push(UniqueEventPtr(
          this, EventDeleter(EventDeleter::Action::DO_NOT_DELETE)));
    }
    void Handle() override;

    Rpc* rpc_ptr;
    bool ok{false};
    bool pending{false};
  };

  // Flows only through our EventQueue.
  struct InternalRpcEvent : public EventBase {
    InternalRpcEvent(Event event, std::weak_ptr<Rpc> rpc)
        : EventBase(event), rpc(std::move(rpc)) {}
    void Handle() override;

    std::weak_ptr<Rpc> rpc;
  };

  Rpc(int method_index, ::grpc::ServerCompletionQueue* server_completion_queue,
      EventQueue* event_queue, ExecutionContext* execution_context,
      const RpcHandlerInfo& rpc_handler_info, Service* service,
      WeakPtrFactory weak_ptr_factory);
  [[nodiscard]] std::unique_ptr<Rpc> Clone();
  void OnConnection();
  void OnRequest();
  void OnReadsDone();
  void OnFinish();
  void RequestNextMethodInvocation();
  void RequestStreamingReadIfNeeded();
  void HandleSendQueue();
  void Write(std::unique_ptr<::google::protobuf::Message> message);
  void Finish(::grpc::Status status);
  [[nodiscard]] Service* service() { return service_; }
  [[nodiscard]] bool IsRpcEventPending(Event event);
  [[nodiscard]] bool IsAnyEventPending();
  // Forgets the DONE tag armed by RequestNextMethodInvocation(). Call this
  // when gRPC reports the requested call as failed: the call was never
  // matched, so that tag will never be delivered. See the comment in
  // Rpc::RequestNextMethodInvocation().
  void DiscardNotifyWhenDone();
  void SetEventQueue(EventQueue* event_queue) { event_queue_ = event_queue; }
  [[nodiscard]] EventQueue* event_queue() { return event_queue_; }
  [[nodiscard]] std::weak_ptr<Rpc> GetWeakPtr();
  [[nodiscard]] RpcHandlerInterface* handler() { return handler_.get(); }
  [[nodiscard]] ::grpc::ServerContext* server_context() {
    return &server_context_;
  }

  Rpc(const Rpc&) = delete;
  Rpc& operator=(const Rpc&) = delete;
  Rpc(Rpc&&) = delete;
  Rpc& operator=(Rpc&&) = delete;
  // Out of line: destroying the readers and writers below instantiates
  // grpc's templates on google::protobuf::Message, which this interface
  // only forward-declares. rpc.cc has the full headers.
  ~Rpc();

 private:
  struct SendItem {
    std::unique_ptr<google::protobuf::Message> msg;
    ::grpc::Status status;
  };

  void InitializeReadersAndWriters(
      ::grpc::internal::RpcMethod::RpcType rpc_type);
  [[nodiscard]] CompletionQueueRpcEvent* GetRpcEvent(Event event);
  [[nodiscard]] bool* GetRpcEventState(Event event);
  void SetRpcEventState(Event event, bool pending);
  void EnqueueMessage(SendItem&& send_item);
  void PerformFinish(std::unique_ptr<::google::protobuf::Message> message,
                     const ::grpc::Status& status);
  void PerformWrite(std::unique_ptr<::google::protobuf::Message> message,
                    const ::grpc::Status& status);

  [[nodiscard]] ::grpc::internal::AsyncReaderInterface<
      ::google::protobuf::Message>*
  async_reader_interface();
  [[nodiscard]] ::grpc::internal::AsyncWriterInterface<
      ::google::protobuf::Message>*
  async_writer_interface();

  [[nodiscard]] ::grpc::internal::ServerAsyncStreamingInterface*
  streaming_interface();

  int method_index_;
  ::grpc::ServerCompletionQueue* server_completion_queue_;
  EventQueue* event_queue_;
  ExecutionContext* execution_context_;
  RpcHandlerInfo rpc_handler_info_;
  Service* service_;
  WeakPtrFactory weak_ptr_factory_;
  ::grpc::ServerContext server_context_;

  CompletionQueueRpcEvent new_connection_event_;
  CompletionQueueRpcEvent read_event_;
  CompletionQueueRpcEvent write_event_;
  CompletionQueueRpcEvent finish_event_;
  CompletionQueueRpcEvent done_event_;

  std::unique_ptr<google::protobuf::Message> request_;
  std::unique_ptr<google::protobuf::Message> response_;

  std::unique_ptr<RpcHandlerInterface> handler_;

  std::unique_ptr<::grpc::ServerAsyncResponseWriter<google::protobuf::Message>>
      server_async_response_writer_;
  std::unique_ptr<::grpc::ServerAsyncReader<google::protobuf::Message,
                                            google::protobuf::Message>>
      server_async_reader_;
  std::unique_ptr<::grpc::ServerAsyncReaderWriter<google::protobuf::Message,
                                                  google::protobuf::Message>>
      server_async_reader_writer_;
  std::unique_ptr<::grpc::ServerAsyncWriter<google::protobuf::Message>>
      server_async_writer_;

  common::Mutex send_queue_lock_;
  std::queue<SendItem> send_queue_;
};

using EventQueue = Rpc::EventQueue;

// This class keeps track of all in-flight RPCs for a 'Service'. Make sure that
// all RPCs have been terminated and removed from this object before it goes out
// of scope.
class ActiveRpcs {
 public:
  ActiveRpcs() = default;
  ~ActiveRpcs() EXCLUDES(lock_);

  // Holds a mutex and the live RPC set; neither may be duplicated.
  ActiveRpcs(const ActiveRpcs&) = delete;
  ActiveRpcs& operator=(const ActiveRpcs&) = delete;
  ActiveRpcs(ActiveRpcs&&) = delete;
  ActiveRpcs& operator=(ActiveRpcs&&) = delete;

  [[nodiscard]] std::shared_ptr<Rpc> Add(std::unique_ptr<Rpc> rpc)
      EXCLUDES(lock_);
  // Returns whether the RPC was still registered. Callers that just want the
  // RPC gone may ignore the result.
  bool Remove(Rpc* rpc) EXCLUDES(lock_);
  // Blocks until no RPC is registered any more, or until 'timeout' elapses.
  // Returns the number of RPCs still registered, i.e. 0 on success.
  [[nodiscard]] std::size_t WaitUntilEmpty(std::chrono::milliseconds timeout)
      EXCLUDES(lock_);
  [[nodiscard]] Rpc::WeakPtrFactory GetWeakPtrFactory();

 private:
  [[nodiscard]] std::weak_ptr<Rpc> GetWeakPtr(Rpc* rpc);

  common::Mutex lock_;
  std::map<Rpc*, std::shared_ptr<Rpc>> rpcs_;
};

}  // namespace async_grpc

// ---- from rpc_handler.h
export namespace async_grpc {

template <RpcServiceMethodSpec RpcServiceMethodConcept>
class RpcHandler : public RpcHandlerInterface {
 public:
  using RpcServiceMethod = RpcServiceMethodTraits<RpcServiceMethodConcept>;
  using RequestType = RpcServiceMethod::RequestType;
  using ResponseType = RpcServiceMethod::ResponseType;

  class Writer {
   public:
    explicit Writer(std::weak_ptr<Rpc> rpc) : rpc_(std::move(rpc)) {}
    [[nodiscard]] bool Write(std::unique_ptr<ResponseType> message) const {
      if (auto rpc = rpc_.lock()) {
        rpc->Write(std::move(message));
        return true;
      }
      return false;
    }
    [[nodiscard]] bool WritesDone() const {
      if (auto rpc = rpc_.lock()) {
        rpc->Finish(::grpc::Status::OK);
        return true;
      }
      return false;
    }
    [[nodiscard]] bool Finish(const ::grpc::Status& status) const {
      if (auto rpc = rpc_.lock()) {
        rpc->Finish(status);
        auto* span = rpc->handler()->trace_span();
        if (span != nullptr) {
          span->SetStatus(status);
        }
        return true;
      }
      return false;
    }

   private:
    std::weak_ptr<Rpc> rpc_;
  };

#if BUILD_TRACING
  RpcHandler()
      : span_(
            OpencensusSpan::StartSpan(RpcServiceMethodConcept::MethodName())) {}
  virtual ~RpcHandler() { span_->End(); }
#endif

  [[nodiscard]] Span* trace_span() override { return span_.get(); }
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
    rpc_->Finish(std::move(status));
#if BUILD_TRACING
    span_->SetStatus(status);
#endif
  }
  void Send(std::unique_ptr<ResponseType> response) {
    rpc_->Write(std::move(response));
  }
  template <std::derived_from<ExecutionContext> T>
  [[nodiscard]] ExecutionContext::Synchronized<T> GetContext() {
    return {execution_context_->lock(), execution_context_};
  }
  template <std::derived_from<ExecutionContext> T>
  [[nodiscard]] T* GetUnsynchronizedContext() {
    return dynamic_cast<T*>(execution_context_);
  }
  [[nodiscard]] Writer GetWriter() { return Writer(rpc_->GetWeakPtr()); }

 protected:
  // Handlers reach the Rpc through this; the pointer itself stays
  // private so only this class can rebind it.
  [[nodiscard]] Rpc* GetRpc() { return rpc_; }

 private:
  ExecutionContext* execution_context_{};
  Rpc* rpc_{};
  std::unique_ptr<Span> span_;
};

}  // namespace async_grpc

// ---- from completion_queue_thread.h
export namespace async_grpc {

class CompletionQueueThread {
 public:
  using CompletionQueueRunner =
      std::function<void(::grpc::ServerCompletionQueue*)>;

  explicit CompletionQueueThread(
      std::unique_ptr<::grpc::ServerCompletionQueue> completion_queue);

  [[nodiscard]] ::grpc::ServerCompletionQueue* completion_queue();

  void Start(const CompletionQueueRunner& runner);
  void Shutdown();

 private:
  std::unique_ptr<::grpc::ServerCompletionQueue> completion_queue_;
  // Owned by us, so it is a std::jthread: it joins on destruction even if
  // Shutdown() is never reached.
  std::jthread worker_thread_;
};

}  // namespace async_grpc

// ---- from event_queue_thread.h
export namespace async_grpc {

class EventQueueThread {
 public:
  // The runner loops until the stop token is signalled by RequestStop() or by
  // the owning std::jthread going away.
  using EventQueueRunner = std::function<void(EventQueue*, std::stop_token)>;

  EventQueueThread();

  [[nodiscard]] EventQueue* event_queue();

  void Start(const EventQueueRunner& runner);

  // Asks the runner to wind down without waiting for it. Idempotent, and safe
  // to call when the thread was never started.
  void RequestStop();

  // Asks the runner to wind down and joins the thread. Idempotent.
  void Shutdown();

 private:
  std::unique_ptr<EventQueue> event_queue_;
  // Owned by us, so it is a std::jthread: it requests a stop and joins on
  // destruction even if Shutdown() is never reached.
  std::jthread thread_;
};

}  // namespace async_grpc

// ---- from service.h
export namespace async_grpc {

// A 'Service' represents a generic service for gRPC asynchronous methods and is
// responsible for managing the lifetime of active RPCs issued against methods
// of the service and distributing incoming gRPC events to their respective
// 'Rpc' handler objects.
class Service : public ::grpc::Service {
 public:
  using EventQueueSelector = std::function<EventQueue*()>;
  friend class Rpc;

  Service(std::string_view service_name,
          const std::map<std::string, RpcHandlerInfo>& rpc_handler_infos,
          EventQueueSelector event_queue_selector);
  void StartServing(
      std::vector<CompletionQueueThread>& completion_queue_threads,
      ExecutionContext* execution_context);
  void HandleEvent(Rpc::Event event, Rpc* rpc, bool ok);
  void StopServing();

  // Blocks until no RPC of this service is in flight any more, or until
  // 'timeout' elapses; returns the number of RPCs still in flight (0 if the
  // service went quiet). Call it during shutdown, after the gRPC server has
  // been shut down but while the completion queues are still being polled:
  // that is the point at which every RPC can still retire itself.
  [[nodiscard]] std::size_t WaitUntilQuiescent(
      std::chrono::milliseconds timeout);

 private:
  void HandleNewConnection(Rpc* rpc, bool ok);
  void HandleRead(Rpc* rpc, bool ok);
  void HandleWrite(Rpc* rpc, bool ok);
  void HandleFinish(Rpc* rpc, bool ok);
  void HandleDone(Rpc* rpc, bool ok);

  void RemoveIfNotPending(Rpc* rpc);

  std::map<std::string, RpcHandlerInfo> rpc_handler_infos_;
  EventQueueSelector event_queue_selector_;
  ActiveRpcs active_rpcs_;
  bool shutting_down_ = false;
};

}  // namespace async_grpc

// ---- from server.h
export namespace async_grpc {

inline constexpr std::int64_t kMaxGrpcMsgSize =
    std::int64_t{2} * 64 * 1024 * 1024 * 8;
inline constexpr double kTracingSamplerProbability = 0.01;  // 1 Percent

// A handler registered with Server::Builder::RegisterHandler(): an
// RpcHandlerInterface implementation that names the service method it serves.
// See async_grpc::RpcHandler.
template <typename T>
concept RpcHandlerImpl = std::derived_from<T, RpcHandlerInterface> &&
                         requires { typename T::RpcServiceMethod; };

class Server {
 protected:
  // All options that configure server behaviour such as number of threads,
  // ports etc.
  // An aggregate: build one with designated initializers.
  struct Options {
    std::size_t num_grpc_threads{};
    std::size_t num_event_threads{};
    std::string server_address;
    int max_receive_message_size = kMaxGrpcMsgSize;
    int max_send_message_size = kMaxGrpcMsgSize;
    bool enable_tracing = false;
    double tracing_sampler_probability = kTracingSamplerProbability;
    std::string tracing_task_name;
    std::string tracing_gcp_project_id;
  };

 public:
  // This 'Builder' is the only way to construct a 'Server'.
  class Builder {
   public:
    Builder() = default;

    [[nodiscard]] std::unique_ptr<Server> Build();
    void SetNumGrpcThreads(std::size_t num_grpc_threads);
    void SetNumEventThreads(std::size_t num_event_threads);
    void SetServerAddress(std::string_view server_address);
    void SetMaxReceiveMessageSize(int max_receive_message_size);
    void SetMaxSendMessageSize(int max_send_message_size);
    static void EnableTracing();
    void DisableTracing();
    void SetTracingSamplerProbability(double tracing_sampler_probability);
    void SetTracingTaskName(std::string_view tracing_task_name);
    void SetTracingGcpProjectId(std::string_view tracing_gcp_project_id);

    template <RpcHandlerImpl RpcHandlerType>
    void RegisterHandler() {
      using RpcServiceMethod = RpcHandlerType::RpcServiceMethod;
      using RequestType = RpcServiceMethod::RequestType;
      using ResponseType = RpcServiceMethod::ResponseType;

      std::string method_full_name = RpcServiceMethod::MethodName();
      auto [service_full_name, method_name] =
          ParseMethodFullName(method_full_name);
      CheckHandlerCompatibility(
          service_full_name, method_name,
          RequestType::default_instance().GetDescriptor(),
          ResponseType::default_instance().GetDescriptor(),
          RpcServiceMethod::StreamType);
      rpc_handlers_[service_full_name].emplace(
          method_name,
          RpcHandlerInfo{
              RequestType::default_instance().GetDescriptor(),
              ResponseType::default_instance().GetDescriptor(),
              [](Rpc* const rpc, ExecutionContext* const execution_context) {
                std::unique_ptr<RpcHandlerInterface> rpc_handler =
                    std::make_unique<RpcHandlerType>();
                rpc_handler->SetRpc(rpc);
                rpc_handler->SetExecutionContext(execution_context);
                rpc_handler->Initialize();
                return rpc_handler;
              },
              RpcServiceMethod::StreamType, std::move(method_full_name)});
    }
    [[nodiscard]] static std::tuple<std::string /* service_full_name */,
                                    std::string /* method_name */>
    ParseMethodFullName(std::string_view method_full_name);

   private:
    using ServiceInfo = std::map<std::string, RpcHandlerInfo>;

    // Not a template: it names google::protobuf::DescriptorPool, which is
    // only forward-declared in this interface, so the body lives in
    // server.cc. The templates above pass in what depends on the handler.
    static void CheckHandlerCompatibility(
        const std::string& service_full_name, const std::string& method_name,
        const google::protobuf::Descriptor* request_type,
        const google::protobuf::Descriptor* response_type,
        ::grpc::internal::RpcMethod::RpcType stream_type);

    Options options_;
    std::map<std::string, ServiceInfo> rpc_handlers_;
  };
  friend class Builder;
  // Calls Shutdown(), which is idempotent.
  virtual ~Server();

  Server(const Server&) = delete;
  Server& operator=(const Server&) = delete;
  Server(Server&&) = delete;
  Server& operator=(Server&&) = delete;

  // Starts a server starts serving the registered services.
  void Start();

  // Waits for the server to shut down. Note: The server must be either shutting
  // down or some other thread must call 'Shutdown()' for this function to ever
  // return.
  void WaitForShutdown();

  // Shuts down the server and all of its services.
  void Shutdown();

  // Sets the server-wide context object shared between RPC handlers.
  void SetExecutionContext(std::shared_ptr<ExecutionContext> execution_context);

  template <std::derived_from<ExecutionContext> T>
  [[nodiscard]] ExecutionContext::Synchronized<T> GetContext() {
    return {execution_context_->lock(), execution_context_.get()};
  }

  template <std::derived_from<ExecutionContext> T>
  [[nodiscard]] T* GetUnsynchronizedContext() {
    return dynamic_cast<T*>(execution_context_.get());
  }

 protected:
  explicit Server(Options options);
  void AddService(
      const std::string& service_name,
      const std::map<std::string, RpcHandlerInfo>& rpc_handler_infos);

 private:
  static void RunCompletionQueue(
      ::grpc::ServerCompletionQueue* completion_queue);
  static void RunEventQueue(Rpc::EventQueue* event_queue,
                            const std::stop_token& stop_token);
  [[nodiscard]] Rpc::EventQueue* SelectNextEventQueueRoundRobin();

  Options options_;

  // gRPC objects needed to build a server.
  ::grpc::ServerBuilder server_builder_;
  std::unique_ptr<::grpc::Server> server_;
  // Guards against shutting down twice; Shutdown() is not meant to be called
  // concurrently with itself.
  bool is_shut_down_ = false;

  // Threads processing the completion queues.
  std::vector<CompletionQueueThread> completion_queue_threads_;

  // Threads processing RPC events.
  std::vector<EventQueueThread> event_queue_threads_;
  common::Mutex current_event_queue_id_lock_;
  std::size_t current_event_queue_id_ = 0;

  // Map of service names to services.
  std::map<std::string, Service> services_;

  // A context object that is shared between all implementations of
  // 'RpcHandler'.
  std::shared_ptr<ExecutionContext> execution_context_;
};

}  // namespace async_grpc

// ---- from completion_queue_pool.h
export namespace async_grpc {

class AsyncClientInterface;

class CompletionQueue {
 public:
  struct ClientEvent {
    enum class Event : std::uint8_t { FINISH = 0, READ = 1, WRITE = 2 };
    ClientEvent(Event event, AsyncClientInterface* async_client)
        : event(event), async_client(async_client) {}
    Event event;
    AsyncClientInterface* async_client;
    bool ok = false;
  };

  CompletionQueue()
      : completion_queue_(std::make_shared<grpc::CompletionQueue>()) {}

  void Start();
  void Shutdown();

  [[nodiscard]] ::grpc::CompletionQueue* completion_queue() {
    return completion_queue_.get();
  }

 private:
  void RunCompletionQueue();

  std::shared_ptr<grpc::CompletionQueue> completion_queue_;
  // Owned by us, so it is a std::jthread: it joins on destruction even if
  // Shutdown() is never reached.
  std::jthread thread_;
};

// TODO(cschuet): Add unit test for CompletionQueuePool.
class CompletionQueuePool {
 public:
  static void SetNumberCompletionQueues(std::size_t number_completion_queues);
  static void Start();
  static void Shutdown();

  // Returns a random completion queue.
  [[nodiscard]] static ::grpc::CompletionQueue* GetCompletionQueue();

  // Owns the completion-queue threads; there is exactly one pool.
  CompletionQueuePool(const CompletionQueuePool&) = delete;
  CompletionQueuePool& operator=(const CompletionQueuePool&) = delete;
  CompletionQueuePool(CompletionQueuePool&&) = delete;
  CompletionQueuePool& operator=(CompletionQueuePool&&) = delete;

 private:
  static constexpr std::size_t kDefaultNumberCompletionQueues = 2;

  CompletionQueuePool();
  ~CompletionQueuePool();

  void Initialize();
  [[nodiscard]] static CompletionQueuePool* completion_queue_pool();

  common::Mutex mutex_;
  bool initialized_ = false;
  std::size_t number_completion_queues_ = kDefaultNumberCompletionQueues;
  std::vector<CompletionQueue> completion_queues_;
};

}  // namespace async_grpc

// ---- from async_client.h
export namespace async_grpc {

class ClientEvent;

class AsyncClientInterface {
  friend class CompletionQueue;

 public:
  AsyncClientInterface() = default;
  virtual ~AsyncClientInterface() = default;

  // Rule of five: a virtual destructor suppresses the implicit move
  // operations, and copying through a base reference would slice.
  AsyncClientInterface(const AsyncClientInterface&) = delete;
  AsyncClientInterface& operator=(const AsyncClientInterface&) = delete;
  AsyncClientInterface(AsyncClientInterface&&) = delete;
  AsyncClientInterface& operator=(AsyncClientInterface&&) = delete;

 private:
  virtual void HandleEvent(
      const CompletionQueue::ClientEvent& client_event) = 0;
};

template <RpcServiceMethodSpec RpcServiceMethodConcept,
          ::grpc::internal::RpcMethod::RpcType StreamType =
              RpcServiceMethodTraits<RpcServiceMethodConcept>::StreamType>
class AsyncClient {};

template <RpcServiceMethodSpec RpcServiceMethodConcept>
class AsyncClient<RpcServiceMethodConcept,
                  ::grpc::internal::RpcMethod::NORMAL_RPC>
    : public AsyncClientInterface {
  using RpcServiceMethod = RpcServiceMethodTraits<RpcServiceMethodConcept>;
  using RequestType = RpcServiceMethod::RequestType;
  using ResponseType = RpcServiceMethod::ResponseType;
  using CallbackType =
      std::function<void(const ::grpc::Status&, const ResponseType*)>;

 public:
  AsyncClient(std::shared_ptr<::grpc::Channel> channel, CallbackType callback)
      : channel_(std::move(channel)),
        callback_(std::move(callback)),
        completion_queue_(CompletionQueuePool::GetCompletionQueue()),
        rpc_method_name_(RpcServiceMethod::MethodName()),
        rpc_method_(rpc_method_name_.c_str(), RpcServiceMethod::StreamType,
                    channel_),
        finish_event_(CompletionQueue::ClientEvent::Event::FINISH, this) {}

  void WriteAsync(const RequestType& request) {
    response_reader_ =
        std::unique_ptr<::grpc::ClientAsyncResponseReader<ResponseType>>(
            ::grpc::internal::ClientAsyncResponseReaderFactory<
                ResponseType>::Create(channel_.get(), completion_queue_,
                                      rpc_method_, &client_context_, request,
                                      /*start=*/false));
    response_reader_->StartCall();
    response_reader_->Finish(&response_, &status_,
                             static_cast<void*>(&finish_event_));
  }

  void HandleEvent(const CompletionQueue::ClientEvent& client_event) override {
    using enum CompletionQueue::ClientEvent::Event;
    switch (client_event.event) {
      case FINISH:
        HandleFinishEvent();
        break;
      default:
        LOG(FATAL) << "Unhandled event type: "
                   << static_cast<int>(client_event.event);
    }
  }

  void HandleFinishEvent() {
    if (callback_) {
      callback_(status_, status_.ok() ? &response_ : nullptr);
    }
  }

 private:
  ::grpc::ClientContext client_context_;
  std::shared_ptr<::grpc::Channel> channel_;
  CallbackType callback_;
  ::grpc::CompletionQueue* completion_queue_;
  std::string rpc_method_name_;
  ::grpc::internal::RpcMethod rpc_method_;
  std::unique_ptr<::grpc::ClientAsyncResponseReader<ResponseType>>
      response_reader_;
  CompletionQueue::ClientEvent finish_event_;
  ::grpc::Status status_;
  ResponseType response_;
};

template <RpcServiceMethodSpec RpcServiceMethodConcept>
class AsyncClient<RpcServiceMethodConcept,
                  ::grpc::internal::RpcMethod::SERVER_STREAMING>
    : public AsyncClientInterface {
  using RpcServiceMethod = RpcServiceMethodTraits<RpcServiceMethodConcept>;
  using RequestType = RpcServiceMethod::RequestType;
  using ResponseType = RpcServiceMethod::ResponseType;
  using CallbackType =
      std::function<void(const ::grpc::Status&, const ResponseType*)>;

 public:
  AsyncClient(std::shared_ptr<::grpc::Channel> channel, CallbackType callback)
      : channel_(std::move(channel)),
        callback_(std::move(callback)),
        completion_queue_(CompletionQueuePool::GetCompletionQueue()),
        rpc_method_name_(RpcServiceMethod::MethodName()),
        rpc_method_(rpc_method_name_.c_str(), RpcServiceMethod::StreamType,
                    channel_),
        write_event_(CompletionQueue::ClientEvent::Event::WRITE, this),
        read_event_(CompletionQueue::ClientEvent::Event::READ, this),
        finish_event_(CompletionQueue::ClientEvent::Event::FINISH, this) {}

  void WriteAsync(const RequestType& request) {
    // Start the call.
    response_reader_ = std::unique_ptr<::grpc::ClientAsyncReader<ResponseType>>(
        ::grpc::internal::ClientAsyncReaderFactory<ResponseType>::Create(
            channel_.get(), completion_queue_, rpc_method_, &client_context_,
            request,
            /*start=*/true, static_cast<void*>(&write_event_)));
  }

  void HandleEvent(const CompletionQueue::ClientEvent& client_event) override {
    using enum CompletionQueue::ClientEvent::Event;
    switch (client_event.event) {
      case WRITE:
        HandleWriteEvent(client_event);
        break;
      case READ:
        HandleReadEvent(client_event);
        break;
      case FINISH:
        HandleFinishEvent(client_event);
        break;
      default:
        LOG(FATAL) << "Unhandled event type: "
                   << static_cast<int>(client_event.event);
    }
  }

  void HandleWriteEvent(const CompletionQueue::ClientEvent& client_event) {
    if (!client_event.ok) {
      LOG(ERROR) << "Write failed in async server streaming.";
      ::grpc::Status status(::grpc::INTERNAL,
                            "Write failed in async server streaming.");
      if (callback_) {
        callback_(status, nullptr);
        callback_ = nullptr;
      }
      finish_status_ = status;
      response_reader_->Finish(&finish_status_,
                               static_cast<void*>(&finish_event_));
      return;
    }

    response_reader_->Read(&response_, static_cast<void*>(&read_event_));
  }

  void HandleReadEvent(const CompletionQueue::ClientEvent& client_event) {
    if (client_event.ok) {
      if (callback_) {
        callback_(::grpc::Status(), &response_);
      }
      response_reader_->Read(&response_, static_cast<void*>(&read_event_));
    } else {
      finish_status_ = ::grpc::Status();
      response_reader_->Finish(&finish_status_,
                               static_cast<void*>(&finish_event_));
    }
  }

  void HandleFinishEvent(const CompletionQueue::ClientEvent& client_event) {
    if (callback_) {
      if (!client_event.ok) {
        LOG(ERROR) << "Finish failed in async server streaming.";
      }
      callback_(
          client_event.ok
              ? ::grpc::Status()
              : ::grpc::Status(::grpc::INTERNAL,
                               "Finish failed in async server streaming."),
          nullptr);
      callback_ = nullptr;
    }
  }

 private:
  ::grpc::ClientContext client_context_;
  std::shared_ptr<::grpc::Channel> channel_;
  CallbackType callback_;
  ::grpc::CompletionQueue* completion_queue_;
  std::string rpc_method_name_;
  ::grpc::internal::RpcMethod rpc_method_;
  std::unique_ptr<::grpc::ClientAsyncReader<ResponseType>> response_reader_;
  CompletionQueue::ClientEvent write_event_;
  CompletionQueue::ClientEvent read_event_;
  CompletionQueue::ClientEvent finish_event_;
  ::grpc::Status status_;
  ResponseType response_;
  ::grpc::Status finish_status_;
};

}  // namespace async_grpc
