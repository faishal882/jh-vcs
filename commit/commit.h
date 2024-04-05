#pragma once

#include "../utils/fileutils.h"
#include "../utils/zlibutils.h"
#include "./tree.h"
#include <iostream>
#include <sstream>

namespace jh {
namespace commit {

class Commit {
public:
  std::string tree;
  std::string parent;
  std::string author;
  std::string message;

  Commit() { std::cerr << "please provide author & message!"; };
  Commit(const std::string &author, const std::string &message);

  std::string createTree();

  bool createCommit();

  void execute();
};

} // namespace commit
} // namespace jh
