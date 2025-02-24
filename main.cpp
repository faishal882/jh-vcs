#include "./include/commit/cat-file.h"
#include "./include/commit/commit-logs.h"
#include "./include/commit/commit.h"
#include "./include/commit/hash-blob.h"
#include "./include/commit/ls-tree.h"
#include "./include/commit/reset.h"
#include "./include/commit/tree.h"
#include "./include/init/init.h"
#include "./include/utils/remoteutils.h"
#include "./include/utils/res-args.h"

#include <iostream>

int main(int argc, char *argv[]) {
  using namespace jh;
  using namespace commit;
  using namespace init;

  auto [command, arg1, flag] = resolveCommands(argc, argv);

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

  else if (command == "remote") {
    if (arg1 == "add") {
      std::string remoteName = argv[3];
      std::string remoteUrl = argv[4];
      jh::remoteUtils::setRemoteUrl(remoteName, remoteUrl);
    }

    else if (arg1 == "get-url") {
      std::string remoteName = argv[3];
      std::cout << jh::remoteUtils::getRemoteUrl(remoteName) << std::endl;
    }
  }

  return 0;
}
