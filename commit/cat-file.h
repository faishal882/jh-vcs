#include "../utils/fileutils.h"
#include "../utils/zlibutils.h"
#include <fstream>
#include <string>

namespace jh {
namespace commit {

class catFile {
private:
  std::string hash;

public:
  catFile(const std::string hash);

  bool decompressFile();
  void execute();
};
} // namespace commit
} // namespace jh
