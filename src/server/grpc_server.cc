#include "gflags/gflags.h"
#include "glog/logging.h"
// #include "gperftools/profiler.h"
#include "src/common/module.h"
#include "src/server/server_impl.h"
#include "src/util/config_manager.h"

int main(int argc, char **argv) {
  // ProfilerStart("bazel_template_profile");

  // google::InitGoogleLogging(argv[0]);
  // gflags::ParseCommandLineFlags(&argc, &argv, true);

  bazel_template::common::InitAllModules(&argc, &argv);

  bazel_template::util::ConfigUtil::Instance().Init();

  bazel_template::server::GrpcServer server(
      bazel_template::util::ConfigUtil::Instance()->GrpcPort());
  server.Run();

  // ProfilerStop();
  return 0;
}
