#include "init/init.h"
#include <iostream>

int main(int argc, char *argv[]) {
  std::string arg = argv[1];
  char *filepath = argv[2];
  jh::init::Init init(filepath);
  init.execute();
  return 0;
}
