#include <fstream>
#include <iostream>
#include <string>
#include <zlib.h>

namespace jh {
namespace commit {

class catFile {
private:
  std::string hash;

public:
  catFile(const std::string hash);
  std::string resolveFilePath();
  bool decompressFile();
  void execute();
};
} // namespace commit
} // namespace jh
