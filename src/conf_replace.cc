#include "src/util.h"

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cout << "must has two path args";
    return -1;
  }

  std::filesystem::path origin_file = argv[1];
  std::filesystem::path final_file = argv[2];
  std::string out;

  Util::ConfReplace(origin_file, final_file, &out);

  std::cout << out << std::endl;
  return 0;
}
