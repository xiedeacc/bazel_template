
/*******************************************************************************
 * Copyright (c) 2022 Copyright 2022- xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "src/server/grpc_server_impl.h"

namespace bazel_template {
namespace server {

// using context::GrpcContext;
// using context::ScopeDecrementContextCounter;
// using common::GCEntry;
using grpc::Status;
using util::ConfigManager;
using util::Util;

class WorkingThreadInPoolDecrementCounter {
 public:
  explicit WorkingThreadInPoolDecrementCounter(GrpcServer* server)
      : server_(server) {}

  ~WorkingThreadInPoolDecrementCounter() {
    absl::MutexLock l(&server_->working_thread_mu_);
    server_->working_thread_in_pool--;
  }

 private:
  GrpcServer* server_;
};

bool WorkingIdle(GrpcServer* server) {
  return server->working_thread_in_pool <= server->working_thread_limit;
}

void GrpcServer::ScanList() {
  // LOG(INFO) << "ScanList";
  // context::GrpcContext* grpc_context = nullptr;
  // context::GrpcContext* fast_fail_grpc_context = nullptr;
  // absl::Condition working_idle(WorkingIdle, this);

  // while (!terminated) {
  // TboxConfigPtr config = ConfigUtil::Instance().GetTboxConfig();
  // int fast_fail_request_tot = 0;
  // int fast_fail_request_by_count = 0;
  // request_pending_queue_.wait_until_not_empty();
  //// LOG(INFO) << "request_pending_queue_ size: "
  //// << request_pending_queue_.size();
  // while (working_thread_in_pool < working_thread_limit) {
  // grpc_context = request_pending_queue_.pop_back();
  //++working_thread_in_pool;
  // thread_pool_->submit(boost::move(grpc_context->task));
  //}

  //// drop request for count
  // while (fast_fail_request_tot < fast_fail_one_scope_limit_ &&
  // request_pending_queue_.size() > config->fast_fail_max_queue_size) {
  // bool pop_status =
  // request_pending_queue_.try_pop_front(&fast_fail_grpc_context);
  // if (!pop_status) {
  // break;
  //}
  // RequestFastFail(fast_fail_grpc_context);
  //++fast_fail_request_by_count;
  //++fast_fail_request_tot;
  //}

  //// drop request for timeout
  // while (fast_fail_request_tot < fast_fail_one_scope_limit_ &&
  // request_pending_queue_.size() > 3) {
  // bool peek_status =
  // request_pending_queue_.try_peek_front(&fast_fail_grpc_context);
  // if (!peek_status) {
  // break;
  //}
  // if ((absl::Now() - fast_fail_grpc_context->created_time) >
  // absl::Milliseconds(config->fast_fail_wait_ms)) {
  // bool pop_status =
  // request_pending_queue_.try_pop_front(&fast_fail_grpc_context);
  // if (!pop_status) {
  // break;
  //}
  // RequestFastFail(fast_fail_grpc_context);
  //++fast_fail_request_tot;
  //} else {
  // break;
  //}
  //}

  // if (fast_fail_request_tot < fast_fail_one_scope_limit_) {
  // absl::MutexLock l(&working_thread_mu_);
  // working_thread_mu_.AwaitWithTimeout(working_idle, absl::Milliseconds(5));
  //}

  // if (fast_fail_request_tot > 0) {
  // if (fast_fail_request_by_count > 0) {
  //// report failed by count
  //}
  // if (fast_fail_request_tot - fast_fail_request_by_count > 0) {
  //// report failed by time
  //}
  //}
  //}

  //// terminated
  // int64_t left_request_count = request_pending_queue_.size();
  // for (int i = 0; i < left_request_count; ++i) {
  // bool peek_status =
  // request_pending_queue_.try_peek_front(&fast_fail_grpc_context);
  // if (peek_status) {
  // RequestFastFail(fast_fail_grpc_context);
  //}
  //}
}

// grpc::Status GrpcServer::ServerStatus(const ServerStatusRequest* request,
// ServerStatusResponse* response) {
// LOG(INFO) << "ServerStatus";
// auto start_time = CommonUtil::CurrentTimeMillis();

// TboxConfigPtr config = ConfigUtil::Instance().GetTboxConfig();
// GCEntry<GrpcContext> gc_entry(
// new GrpcContext(request, response, &request->config(), 1));
// GrpcContext& grpc_context = *gc_entry.value;

// if (!grpc_context.is_built_succ || grpc_context.is_skip) {
// BuildDefaultResponse(request, response);
// return Status::OK;
//}

// grpc_context.task = std::bind(&GrpcServer::DoServerStatus, this, request,
// response, &grpc_context);
// request_pending_queue_.push_back(&grpc_context);
// grpc_context.blocking_counter.Wait();

// if (grpc_context.is_fast_fail) {
// BuildDefaultResponse(request, response);
// return Status::OK;
//}

//// log
// int64_t end_time = CommonUtil::CurrentTimeMillis();
// int64_t cost_time = end_time - start_time;
// if (grpc_context.is_debug || cost_time >= 200 ||
// end_time >= next_log_info_time_.load()) {
// next_log_info_time_.store(end_time + 200);
// grpc_context.log_info.SetTotalCostTimeMs(cost_time);
// grpc_context.log_info.SetSuccessNum(1);
// grpc_context.log_info.SetIsDebug(grpc_context.is_debug);
// LOG(INFO) << "log_info: " << grpc_context.log_info.ToJsonString();
//}

// return Status::OK;
//}

// void GrpcServer::DoServerStatus(const server::ServerStatusRequest* request,
// server::ServerStatusResponse* response,
// context::GrpcContext* grpc_context) {
// LOG(INFO) << "DoServerStatus";
// ScopeDecrementContextCounter context_dec_counter(grpc_context);
// WorkingThreadInPoolDecrementCounter working_thread_dec_counter(this);
// response->set_ret_msg("ok");
// response->set_ret_code(error::ErrorCode::SUCCESS);
// response->set_server_status("server running");
//}

// void GrpcServer::RequestFastFail(GrpcContext* grpc_context) {
// grpc_context->is_fast_fail = true;
// grpc_context->blocking_counter.DecrementCount();
//}

// void GrpcServer::BuildDefaultResponse(const ServerStatusRequest* request,
// ServerStatusResponse* response) {
// LOG(INFO) << "BuildDefaultResponse";
//}

}  // namespace server
}  // namespace bazel_template
