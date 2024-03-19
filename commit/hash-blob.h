#pragma once

#include <fstream>
#include <iomanip>
#include <iostream>
#include <openssl/sha.h>
#include <sstream>
#include <string>
#include <zlib.h>

namespace jh {
namespace commit {
class hashBlob {
public:
  std::string data;
  std::string inputFile;

  hashBlob(const std::string inputFile);
  std::string sha1();
  bool compressFile();
  bool createFile(std::string &hash);

  void execute();
};
} // namespace commit
} // namespace jh
