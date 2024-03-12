#include "commit/commit.h"
// #include "init/init.h"
#include <iostream>

int main(int argc, char *argv[]) {
  std::string arg = argv[1];
  char *filepath = argv[2];
  // jh::init::Init init(filepath);
  jh::commit::Commit commit;
  commit.execute();
  // init.execute();
  return 0;
}
