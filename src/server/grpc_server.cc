#include "gflags/gflags.h"
#include "glog/logging.h"
// #include "gperftools/profiler.h"
#include "src/common/module.h"
#include "src/server/grpc_server_impl.h"
#include "src/util/config_manager.h"

int main(int argc, char **argv) {
  // ProfilerStart("bazel_template_profile");

  // google::InitGoogleLogging(argv[0]);
  // gflags::ParseCommandLineFlags(&argc, &argv, true);

  common::InitAllModules(&argc, &argv);

  util::ConfigUtil::Instance().Init();

  server::GrpcServer server(util::ConfigUtil::Instance()->GrpcPort());
  server.WaitForShutdown();

  // ProfilerStop();
  return 0;
}
