#include "../include/commit/hash-blob.h"

namespace jh {
namespace commit {

hashBlob::hashBlob(std::string inputFile) { this->inputFile = inputFile; }

bool hashBlob::createBlob() {
  std::stringstream fileData;
  std::vector<char> compressedData;

  std::ifstream inFile(inputFile, std::ios::binary);
  if (!inFile.is_open()) {
    std::cerr << "Failed to open input file: " << inputFile << std::endl;
  }
  fileData << inFile.rdbuf();

  if (Zlib::compress(fileData, compressedData)) {
    std::string sha;
    sha = Zlib::sha1(compressedData);
    bool created = fileUtils::createFile(sha, compressedData);
    if (created) {
      std::cout << sha << std::endl;
      return true;
    }
  }

  return false;
}

void hashBlob::execute() { createBlob(); }
} // namespace commit
} // namespace jh
