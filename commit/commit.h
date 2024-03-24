#pragma once

#include <iostream>

namespace jh {
namespace commit {

class Commit {
public:
  std::string author;
  std::string message;

  Commit() { std::cerr << "please provide author & message!"; };
  Commit(const std::string &author, const std::string &message);

  bool createCommit();

  void execut();
};

} // namespace commit
} // namespace jh
