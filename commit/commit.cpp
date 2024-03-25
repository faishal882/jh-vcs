#include "commit.h"

namespace jh {
namespace commit {
Commit::Commit(const std::string &author, const std::string &msg) {
  this->author = author;
  this->message = msg;
  this->tree = "";
  this->parent = "";
}

bool Commit::createCommit() {
  std::cout << author << " " << message << std::endl;
  std::stringstream commit;
  std::string compressedData;

  commit << "Tree: "
         << "\t" << tree << std::endl
         << "Parent: "
         << "\t" << parent << std::endl
         << "Author: "
         << "\t" << author << std::endl
         << "Message: "
         << "\t" << message << std::endl;

  bool compressed = Zlib::compress(commit, compressedData);

  if (compressed) {
    std::string sha = Zlib::sha1(compressedData);
    bool created = fileUtils::createFile(sha, compressedData);
    std::cout << sha << std::endl;
    if (created)
      return true;
  }

  return false;
}

void Commit::execute() { createCommit(); }
} // namespace commit
} // namespace jh
