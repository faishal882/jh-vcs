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

  bool deCompressBlob(const std::string &hash, const std::string &filename);

  std::string extractTreeHash(const std::string &commit);

  std::vector<std::vector<std::string>>
  getBlobsandTrees(const std::string &tree);

  void execute();
};
} // namespace commit
} // namespace jh
