#include "commit/cat-file.h"
#include "commit/commit.h"
#include "commit/hash-blob.h"
#include "commit/ls-tree.h"
#include "commit/tree.h"
#include "init/init.h"
#include <iostream>

int main(int argc, char *argv[]) {
  using namespace jh;
  using namespace commit;
  using namespace init;

  std::string command = argv[1];
  std::string arg1 = argv[2];
  std::string flag = argv[3];

  if (command == "cat-file") {
    catFile(arg1).decompressFile();
  }

  else if (command == "init") {
    Init init(arg1.c_str());
    init.execute();
  }

  else if (command == "hash-object") {
    hashBlob blob(arg1);
    blob.execute();
  }

  else if (command == "ls-tree") {
    if (flag == "-w") {
      Tree tree(arg1);
      std::cout << tree.tree << std::endl;
      // tree.execute();
    } else {
      lsTree tree(arg1);
      tree.decompressFolder();
    }
  }

  else if (command == "commit") {
    Commit commit("faishal", arg1);
    commit.execute();
  }

  return 0;
}
