/*******************************************************************************
 * Copyright (c) 2023  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/
#pragma once

#include <thread>

#include "brpc/server.h"
#include "src/proto/service.pb.h"
#include "src/util/config_util.h"

namespace bazel_template {
namespace server {

enum ServerStatus {
  STATUS_INIT = 0,
  STATUS_SERVING,
  STATUS_SHUTDOWNING,
  STATUS_STOPPED
};

class ServiceImpl final : public bazel_template::proto::Service {
 public:
  ServiceImpl() : server_status_(STATUS_INIT) {}

  virtual ~ServiceImpl() {
    if (shutdown_task_.joinable()) {
      shutdown_task_.join();
    }
    LOG(ERROR) << "~ServiceImpl";
  }

  virtual void Handle(google::protobuf::RpcController *cntl_base,
                      const bazel_template::proto::Request *req,
                      bazel_template::proto::Response *res,
                      google::protobuf::Closure *done);

  virtual void HttpHandle(google::protobuf::RpcController *cntl_base,
                          const bazel_template::proto::HttpRequest *req,
                          bazel_template::proto::HttpResponse *res,
                          google::protobuf::Closure *done);

  bool Init();

  void Run() {
    if (brpc_server_.AddService(this, brpc::SERVER_DOESNT_OWN_SERVICE)) {
      LOG(FATAL) << "fail add brpc service!";
      return;
    }
    if (brpc_http_server_.AddService(this, brpc::SERVER_OWNS_SERVICE,
                                     "/handle => HttpHandle")) {
      LOG(ERROR) << "fail add http service!";
      return;
    }

    brpc::ServerOptions s_options;
    s_options.max_concurrency =
        util::ConfigUtil::Instance().BrpcMaxConcurrency();
    s_options.num_threads = util::ConfigUtil::Instance().BrpcNumThreads();

    if (brpc_server_.Start(util::ConfigUtil::Instance().BrpcServerPort(),
                           &s_options)) {
      LOG(FATAL) << "fail to start brpc server!";
      return;
    }
    LOG(INFO) << "started brpc server, port: "
              << util::ConfigUtil::Instance().BrpcServerPort()
              << ", max_concurrency: "
              << util::ConfigUtil::Instance().BrpcMaxConcurrency()
              << ", num_threads: "
              << util::ConfigUtil::Instance().BrpcNumThreads();

    if (brpc_http_server_.Start(
            util::ConfigUtil::Instance().BrpcHttpServerPort(), &s_options)) {
      LOG(ERROR) << "fail to start http server!";
      return;
    }
    LOG(INFO) << "started http server, port: "
              << util::ConfigUtil::Instance().BrpcHttpServerPort()
              << ", max_concurrency: "
              << util::ConfigUtil::Instance().BrpcMaxConcurrency()
              << ", num_threads: "
              << util::ConfigUtil::Instance().BrpcNumThreads();

    nacos_properties_[nacos::PropertyKeyConst::SERVER_ADDR] =
        util::ConfigUtil::Instance().BaseConfig()->config.nacos_sdk_addr();
    nacos_properties_[nacos::PropertyKeyConst::NAMESPACE] =
        util::ConfigUtil::Instance().BaseConfig()->config.nacos_namespace();
    nacos_properties_[nacos::PropertyKeyConst::UDP_RECEIVER_PORT] =
        util::ConfigUtil::Instance()
            .BaseConfig()
            ->config.nacos_sdk_rcv_udp_port();

    service_factory_.reset(new nacos::NacosServiceFactory(nacos_properties_));
    naming_srv_.reset(service_factory_->CreateNamingService());
    instance_.groupName =
        util::ConfigUtil::Instance().BaseConfig()->config.nacos_group_name();
    instance_.clusterName =
        util::ConfigUtil::Instance().BaseConfig()->config.nacos_cluster_name();
    instance_.ip = util::Util::GetServerIp();
    instance_.instanceId = util::Util::GetServerIp();
    instance_.ephemeral = true;
    const std::string &service_name =
        util::ConfigUtil::Instance().BaseConfig()->config.nacos_service_name();
    instance_.port = util::ConfigUtil::Instance().BrpcServerPort();

    try {
      naming_srv_->registerInstance(service_name, instance_);
      LOG(INFO) << "registed nacos, service_name: " << service_name
                << ", ip: " << instance_.ip << ", port: " << instance_.port;
    } catch (nacos::NacosException &e) {
      LOG(ERROR) << "register nacos exception: " << e.what();
      return;
    }

    int bazel_templateect_status = STATUS_INIT;
    int next_status = STATUS_SERVING;
    server_status_.compare_exchange_strong(bazel_templateect_status, next_status);

    brpc_server_.Join();
    brpc_http_server_.Join();
    LOG(INFO) << "server stopped";
  }

  static void SignalHandler(int signal) {
    LOG(INFO) << "got signal: " << signal << ", ready to stop";
    terminated.store(true);
  }

 private:
  void ShutdownTask() {
    while (true) {
      if (terminated.load()) {
        int bazel_templateect_status = STATUS_SERVING;
        int next_status = STATUS_SHUTDOWNING;
        if (server_status_.compare_exchange_strong(bazel_templateect_status,
                                                   next_status)) {
          try {
            naming_srv_->deregisterInstance(util::ConfigUtil::Instance()
                                                .BaseConfig()
                                                ->config.nacos_service_name(),
                                            util::ConfigUtil::Instance()
                                                .BaseConfig()
                                                ->config.nacos_group_name(),
                                            instance_);
            LOG(INFO) << "unregisted nacos, service_name: "
                      << util::ConfigUtil::Instance()
                             .BaseConfig()
                             ->config.nacos_service_name()
                      << ", ip: " << instance_.ip
                      << ", port: " << instance_.port;
          } catch (nacos::NacosException &e) {
            LOG(ERROR) << "unregister nacos exception: " << e.what();
            return;
          }

          LOG(INFO) << "shutdown server in "
                    << util::ConfigUtil::Instance().DelayShutdownSec()
                    << " seconds";
          std::thread(
              std::bind<void>(
                  [](std::atomic<int> *server_status, brpc::Server *brpc_server,
                     brpc::Server *http_server) {
                    absl::SleepFor(absl::Seconds(
                        util::ConfigUtil::Instance().DelayShutdownSec()));
                    absl::SleepFor(absl::Seconds(1));
                    int expect_status = STATUS_SHUTDOWNING;
                    int next_status = STATUS_STOPPED;
                    if (server_status->compare_exchange_strong(expect_status,
                                                               next_status)) {
                      brpc_server->Stop(3);
                      http_server->Stop(3);
                      LOG(INFO) << "brpc server stopped, status: "
                                << brpc_server->status();
                      LOG(INFO) << "http server stopped, status: "
                                << http_server->status();
                    }
                  },
                  &server_status_, &brpc_server_, &brpc_http_server_))
              .detach();
        }
      }
      if (server_status_.load() == STATUS_STOPPED) {
        LOG(INFO) << "server already stopped";
        break;
      }
      util::Util::Sleep(100);
    }
    LOG(WARNING) << "ShutdownTask existed";
  }

  void BuildHttpDefaultResponse(const bazel_template::proto::Request *req,
                                bazel_template::proto::Response *res,
                                brpc::Controller *cntl) {
    res->set_request_id(req->request_id());
    if (!cntl) {
      return;
    }
    butil::IOBufAsZeroCopyOutputStream wrapper(&(cntl->response_attachment()));
    if (!res->SerializeToZeroCopyStream(&wrapper)) {
      LOG(ERROR) << "SerializeToZeroCopyStream error!";
    }
  }

  void BuildDefaultResponse(const bazel_template::proto::Request *req,
                            bazel_template::proto::Response *res,
                            brpc::Controller *cntl) {
    res->set_request_id(req->request_id());
  }

 public:
  bool DoHandle(const bazel_template::proto::Request &req,
                bazel_template::proto::Response *res);

 public:
  static std::atomic<bool> terminated;

 private:
  std::atomic<int> server_status_;
  std::thread shutdown_task_;
  nacos::Properties nacos_properties_;
  std::shared_ptr<nacos::NacosServiceFactory> service_factory_;
  std::shared_ptr<nacos::NamingService> naming_srv_;
  nacos::Instance instance_;
  brpc::Server brpc_server_;
  brpc::Server brpc_http_server_;
};  // class ServiceImpl

}  // namespace server
}  // namespace bazel_template
