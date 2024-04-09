#pragma once

#include "../utils/fileutils.h"
#include "../utils/zlibutils.h"

namespace jh {
namespace commit {

class ResetCommit {
public:
  std::string hash;

  ResetCommit(const std::string &hash);

  bool deCompressCommit(const std::string &hash);
  bool deCompressTree(const std::string &hash);
  bool deCompressBlob(const std::string &hash);

  bool replaceFileContent(const char *hash, const char *filename);
  bool createFile(const char *hash, const char *filename);
};
} // namespace commit
} // namespace jh
