/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

// #include "gflags/gflags.h"
#include "glog/logging.h"

int main(int argc, char **argv) {
  google::InitGoogleLogging(argv[0]);
  google::SetStderrLogging(google::GLOG_INFO);
  LOG(INFO) << "TEST";
  // google::ParseCommandLineFlags(&argc, &argv, false);
}
