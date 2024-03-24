#include "commit.h"

namespace jh {
namespace commit {
Commit::Commit(const std::string &author, const std::string &msg) {
  this->author = author;
  this->message = msg;
}

bool Commit::createCommit() {
  std::cout << author << " " << message << std::endl;
  return true;
}
} // namespace commit
} // namespace jh
