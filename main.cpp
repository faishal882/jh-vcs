#include "./commit/commit-logs.h"
#include "commit/cat-file.h"
#include "commit/commit.h"
#include "commit/hash-blob.h"
#include "commit/ls-tree.h"
#include "commit/reset.h"
#include "commit/tree.h"
#include "init/init.h"
#include "utils/res-args.h"

#include <iostream>

int main(int argc, char *argv[]) {
  using namespace jh;
  using namespace commit;
  using namespace init;

  auto [command, arg1, flag] = resoleCommands(argc, argv);

  if (command == "cat-file") {
    catFile cat_file(arg1);
    cat_file.execute();
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

  else if (command == "reset") {
    ResetCommit reset(arg1);
    reset.execute();
  }

  else if (command == "log") {
    commitLog logs;
    logs.execute();
  }

  return 0;
}
