#include "commit/cat-file.h"
#include "commit/commit.h"
#include "commit/hash-blob.h"
#include "commit/ls-tree.h"
#include "init/init.h"
#include <iostream>

int main(int argc, char *argv[]) {
  using namespace jh;
  using namespace commit;
  using namespace init;
  std::string command = argv[1];
  std::string arg1 = argv[2];
  if (command == "cat-file") {
    catFile(arg1).decompressFile();
  } else if (command == "init") {
    Init init(arg1.c_str());
    init.execute();
  } else if (command == "hash-object") {
    hashBlob blob(arg1);
    blob.execute();
  } else if (command == "ls-tree") {
    lsTree tree(arg1);
    tree.decompressFolder();
  }

  return 0;
}
