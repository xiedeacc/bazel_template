#ifndef UTIL_H
#define UTIL_H

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "boost/algorithm/string.hpp"
#include "boost/algorithm/string/trim_all.hpp"

class Util {
public:
  static bool ConfGen(const std::filesystem::path &origin_file,
                      std::string *out) {
    if (!std::filesystem::exists(origin_file)) {
      *out = origin_file.string() + " gen error, file not exists";
      return false;
    }

    std::vector<std::string> origin_lines;
    std::ifstream origin(origin_file);
    if (origin.is_open()) {
      std::string line;
      while (std::getline(origin, line)) {
        origin_lines.push_back(line);
      }
      origin.close();
    } else {
      *out = origin_file.string() + " gen error, open failed";
      return false;
    }

    std::string name = origin_file.filename();
    boost::algorithm::replace_all(name, ".", "_");
    std::stringstream buff;

    buff << "genrule(" << std::endl;
    buff << "    name = \"" << name << "\"," << std::endl;
    buff << "outs = [\"" << origin_file.string() << "\"]," << std::endl;
    buff << "    cmd = \"\\n\".join([" << std::endl;
    buff << "\"cat <<'EOF' >$@\"," << std::endl;

    for (size_t i = 0; i < origin_lines.size(); ++i) {
      boost::algorithm::replace_all(origin_lines[i], "\\", "\\\\");
      boost::algorithm::replace_all(origin_lines[i], "\"", "\\\"");
      buff << "\"" << origin_lines[i] << "\"," << std::endl;
    }

    buff << "\"EOF\"," << std::endl;
    buff << "    ])," << std::endl;
    buff << ")" << std::endl;

    *out = buff.str();
    return true;
  }

  static bool ConfReplace(const std::filesystem::path &origin_file,
                          const std::filesystem::path &final_file,
                          std::string *out) {
    if (!std::filesystem::exists(origin_file)) {
      *out = origin_file.string() + " replace error, origin file not exists";
      return false;
    }

    if (!std::filesystem::exists(final_file)) {
      *out = final_file.string() + " replace error, final file not exists";
      return false;
    }

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
      *out = origin_file.string() + " replace error, origin file open failed";
      return false;
    }

    std::ifstream fi(final_file);
    if (fi.is_open()) {
      std::string line;
      while (std::getline(fi, line)) {
        final_lines.push_back(line);
      }
      fi.close();
    } else {
      *out = origin_file.string() + " replace error, final file open failed";
      return false;
    }

    if (origin_lines.size() != final_lines.size()) {
      *out = origin_file.string() + " replace error, line num not equal";
      return false;
    }

    std::string name = final_file.filename();
    boost::algorithm::replace_all(name, ".", "_");
    std::stringstream buff;

    buff << "template_rule(" << std::endl;
    buff << "    name = \"" << name << "\"," << std::endl;
    buff << "src= \"" << origin_file.string() << "\"," << std::endl;
    buff << "out = \"" << final_file.string() << "\"," << std::endl;
    buff << "    substitutions = {" << std::endl;

    for (size_t i = 0; i < origin_lines.size(); ++i) {
      if (origin_lines[i].empty() || origin_lines[i][0] != '#') {
        continue;
      }
      boost::algorithm::replace_all(origin_lines[i], "\\", "\\\\");
      boost::algorithm::replace_all(final_lines[i], "\\", "\\\\");

      boost::algorithm::replace_all(origin_lines[i], "\"", "\\\"");
      boost::algorithm::replace_all(final_lines[i], "\"", "\\\"");
      buff << "\"" << origin_lines[i] << "\""
           << ": "
           << "\"" << final_lines[i] << "\"," << std::endl;
    }

    buff << "}," << std::endl;
    buff << ")" << std::endl;

    *out = buff.str();
    return true;
  }
};

#endif
