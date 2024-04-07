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

  std::string parentHash();
  std::string createTree();

  bool createCommit();

  void execute();

private:
  std::string extractHead(const std::string &input);
  void getDataFromFile(const std::string &file, std::string &fileData);
};

} // namespace commit
} // namespace jh
