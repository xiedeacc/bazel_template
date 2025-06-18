#include "src/client/handler/load_folder_handler.h"

#include "gtest/gtest.h"

namespace bazel_template {
namespace client {
namespace handler {

TEST(FolderLoader, Load) { FolderLoader::Instance()->Load("./large.json"); }

}  // namespace handler
}  // namespace client
}  // namespace bazel_template
