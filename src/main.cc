#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "boost/algorithm/string.hpp"
#include "boost/algorithm/string/trim_all.hpp"

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cout << "must has two path args";
    return -1;
  }

  std::filesystem::path origin_file = argv[1];
  std::filesystem::path final_file = argv[2];

  if (std::filesystem::exists(origin_file) &&
      std::filesystem::exists(final_file)) {
    std::vector<std::string> origin_lines;
    std::vector<std::string> final_lines;

    std::ifstream origin(origin_file);
    if (origin.is_open()) {
      std::string line;
      while (std::getline(origin, line)) {
        origin_lines.push_back(line);
      }
      origin.close();
    } else {
      std::cerr << "Failed to open the file: " << origin_file << std::endl;
      return -1;
    }

    std::ifstream fi(final_file);
    if (fi.is_open()) {
      std::string line;
      while (std::getline(fi, line)) {
        final_lines.push_back(line);
      }
      fi.close();
    } else {
      std::cerr << "Failed to open the file: " << origin_file << std::endl;
      return -1;
    }

    if (origin_lines.size() != final_lines.size()) {
      std::cerr << "lines number not equal" << std::endl;
      return -1;
    }

    for (size_t i = 0; i < origin_lines.size(); ++i) {
      if (origin_lines[i].empty() || origin_lines[i][0] != '#') {
        continue;
      }
      boost::algorithm::replace_all(origin_lines[i], "\"", "\\\\\\\"");
      boost::algorithm::replace_all(final_lines[i], "\"", "\\\\\\\"");
      std::cout << "\"" << origin_lines[i] << "\""
                << ": "
                << "\"" << final_lines[i] << "\"," << std::endl;
    }

  } else if (!std::filesystem::exists(origin_file)) {
    std::cerr << "File does not exist: " << origin_file << std::endl;
    return -1;
  } else if (!std::filesystem::exists(final_file)) {
    std::cerr << "File does not exist: " << final_file << std::endl;
    return -1;
  }
  return 0;
}
