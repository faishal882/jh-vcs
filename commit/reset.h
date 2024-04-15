#pragma once

#include "../utils/fileutils.h"
#include "../utils/zlibutils.h"
#include <regex>

namespace jh {
namespace commit {

class ResetCommit {
public:
  std::string hash;

  ResetCommit(const std::string &hash);

  bool decompress(const std::string &hash);
  std::string deCompressCommit(const std::string &hash);
  std::string deCompressTree(const std::string &hash);
  std::string deCompressBlob(const std::string &hash);

  std::string extractTreeHash(const std::string &commit);
  std::vector<std::vector<std::string>> getBlobsandTrees(std::string &tree);

  bool replaceFileContents(const std::string &file, const std::string &content);
  bool createFile(const char *hash, const char *filename);

  void execute();
};
} // namespace commit
} // namespace jh
