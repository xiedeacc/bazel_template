/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "src/util/util.h"

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cout << "must has two path args";
    return -1;
  }

  std::filesystem::path origin_file = argv[1];
  std::filesystem::path final_file = argv[2];
  std::string out;

  bazel_template::util::Util::ConfReplace(origin_file, final_file, &out);

  std::cout << out << std::endl;
  return 0;
}
