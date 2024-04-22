#pragma once

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

namespace jh {
namespace commit {
class commitLog {
public:
  std::stringstream logs;

  commitLog();

  std::stringstream getLogs();

  std::vector<std::vector<std::string>> extractInfo();

  std::string representTime(const std::string &timestamp);

  void execute();
};
} // namespace commit
} // namespace jh
