#include "commit/cat-file.h"
#include "commit/commit.h"
// #include "init/init.h"
#include <iostream>

int main(int argc, char *argv[]) {
  using namespace jh;
  using namespace commit;
  std::string command = argv[1];
  std::string arg1 = argv[2];
  if (command == "cat-file") {
    std::cout << command << arg1 << std::endl;
    catFile(arg1).decompressFile();
  }

  return 0;
}
