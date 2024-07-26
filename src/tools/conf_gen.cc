#include "src/util/util.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cout << "must has one path args";
    return -1;
  }

  std::filesystem::path origin_file = argv[1];
  std::string out;

  bazel_template::util::Util::ConfGen(origin_file, &out);

  std::cout << out << std::endl;
  return 0;
}
