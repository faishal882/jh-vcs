#include "cat-file.h"

namespace jh {
namespace commit {
catFile::catFile(const std::string hash) { this->hash = hash; };

bool catFile::decompressFile() {
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
    // std::cout << data << std::endl;
    std::cout << "SUCCESS" << std::endl;

  return true;
}
} // namespace commit
} // namespace jh
