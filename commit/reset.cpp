#include "reset.h"

namespace jh {
namespace commit {
ResetCommit::ResetCommit(const std::string &hash) {
  this->hash = hash;
  std::cout << hash << std::endl;
}

bool ResetCommit::deCompressCommit(const std::string &hash) {
  std::string file = fileUtils::resolveFilePath(hash);
  std::ifstream inFile(file, std::ios::binary);
  if (!inFile.is_open()) {
    std::cerr << "Failed to open input file: " << file << std::endl;
    return false;
  }

  std::stringstream ss;
  ss << inFile.rdbuf();
  inFile.close();

  std::string data;

  bool decompressed = Zlib::decompress(ss, data);

  if (decompressed)
    std::cout << data << std::endl;

  return true;
}
bool ResetCommit::deCompressTree(const std::string &hash) { return true; }
bool ResetCommit::deCompressBlob(const std::string &hash) { return true; }

bool replaceFileContent(const char *hash, const char *filename);
bool createFile(const char *hash, const char *filename);
} // namespace commit
} // namespace jh
