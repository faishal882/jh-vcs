#pragma once

#include "hash-blob.h"
#include "tree.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace jh {
namespace commit {

class Commit : public Tree {
public:
  std::string author;
  std::string message;

  Commit(std::string author) {
    std::cout << "Commited " << author << std::endl;
  }

  // bool compress(std::stringstream &ss, std::string &data);

  // int createFolder(const char *folderpath, mode_t mode);
  // bool createFile(std::string &hash, std::string &data);
};

} // namespace commit
} // namespace jh
