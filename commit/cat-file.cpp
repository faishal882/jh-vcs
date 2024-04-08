#include "cat-file.h"

namespace jh {
namespace commit {
catFile::catFile(const std::string hash) { this->hash = hash; };

std::string catFile::resolveFilePath() {
  // find file
  std::string folder = hash.substr(0, 2);
  std::string file_name = hash.substr(2, 38);
  std::string filelocation = ".jh/objects/" + folder + "/" + file_name;
  return filelocation;
}

bool catFile::decompressFile() {
  std::string file = resolveFilePath();
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
} // namespace commit
} // namespace jh
