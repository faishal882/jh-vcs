#pragma once

#include <iostream>

namespace jh {
namespace commit {

class ResetCommit {
public:
  std::string hash;

  ResetCommit(const char *hash);

  bool deCompressTree(const char *hash);
  bool deCompressBlob(const char *hash);

  bool replaceFileContent(const char *hash, const char *filename);
  bool createFile(const char *hash, const char *filename);
};
} // namespace commit
} // namespace jh
