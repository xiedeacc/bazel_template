#include "src/util/util.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cout << "must has one conf path args";
    return -1;
  }

  std::filesystem::path file_path = argv[1];
  std::ifstream file(file_path);

  if (!file.is_open()) {
    std::cerr << "Failed to open the file: " << file_path.string() << std::endl;
    return -1;
  }

  std::string content;
  std::string line;
  while (std::getline(file, line)) {
    boost::algorithm::trim(line);
    if (line.empty()) {
      continue;
    }
    std::vector<std::string> result;
    std::string rule;
    boost::split(result, line, boost::is_any_of(":"));
    if (result.size() == 3) {
      boost::algorithm::trim(result[0]);
      boost::algorithm::trim(result[1]);
      boost::algorithm::trim(result[2]);
      if (result[0] == "replace") {
        std::filesystem::path origin_file = result[1];
        std::filesystem::path final_file = result[2];
        Util::ConfReplace(origin_file, final_file, &rule);
      } else {
        std::cerr << "unsupport type: " << line << std::endl;
        continue;
      }
    } else if (result.size() == 2) {
      boost::algorithm::trim(result[0]);
      boost::algorithm::trim(result[1]);
      if (result[0] == "gen") {
        std::filesystem::path origin_file = result[1];
        Util::ConfGen(origin_file, &rule);
      } else {
        std::cerr << "unsupport type: " << line << std::endl;
        continue;
      }
    } else {
      std::cerr << "conf line should split by :, content: " << line
                << std::endl;
      continue;
    }
    content.append(rule);
    content.append("\n");
  }

  std::ofstream build_file("BUILD.tmp");
  if (!build_file) {
    std::cerr << "open BUILD.tmp failed" << std::endl;
  }
  build_file << content << std::endl;

  build_file.close();
  file.close();
  return 0;
}
