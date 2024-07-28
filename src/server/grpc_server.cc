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

  bazel_template::common::InitAllModules(&argc, &argv);

  bazel_template::util::ConfigManager::Instance()->Init(
      "./conf/base_config.json");

  bazel_template::server::GrpcServer server(
      bazel_template::util::ConfigManager::Instance()->GrpcServerPort());
  server.WaitForShutdown();

  // ProfilerStop();
  return 0;
}
