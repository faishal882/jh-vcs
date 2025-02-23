#pragma once

#include "../utils/fileutils.h"
#include "../utils/zlibutils.h"
#include <string.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>

namespace jh {
namespace commit {
class hashBlob {
public:
  std::string inputFile;

  hashBlob(const std::string inputFile);

  bool createBlob();

  void execute();
};
} // namespace commit
} // namespace jh
