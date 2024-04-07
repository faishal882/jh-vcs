#include "commit.h"

namespace jh {
namespace commit {
Commit::Commit(const std::string &author, const std::string &msg) {
  this->author = author;
  this->message = msg;
  this->tree = createTree();
  this->parent = parentHash();
}

std::string Commit::extractHead(const std::string &input) {
  size_t lastSlashPos = input.find_last_of('/');
  if (lastSlashPos != std::string::npos && lastSlashPos < input.size() - 1) {
    return input.substr(lastSlashPos + 1);
  }
  return "";
}

void Commit::getDataFromFile(const std::string &file, std::string &fileData) {
  std::ifstream inFile(file, std::ios::binary);
  if (!inFile.is_open()) {
    std::cerr << "Failed to open input file: " << file << std::endl;
    return;
  }
  std::getline(inFile, fileData);
  inFile.close();
}

std::string Commit::parentHash() {
  std::string headFile = ".jh/HEAD";
  std::string headData;
  getDataFromFile(headFile, headData);

  std::string head = extractHead(headData);

  std::string parent = ".jh/refs/" + head;
  std::string parentHash;
  getDataFromFile(parent, parentHash);

  return parentHash;
}

std::string Commit::createTree() {
  Tree tree = Tree();
  std::cout << tree.tree << std::endl;
  std::stringstream ss(tree.tree);
  std::string data;

  if (Zlib::compress(ss, data)) {
    std::string sha;
    sha = Zlib::sha1(data);

    return sha;
  }

  return "";
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
    if (created) {
      std::string headFile = ".jh/HEAD";
      std::string headData;
      getDataFromFile(headFile, headData);

      std::string head = extractHead(headData);

      std::string parent = ".jh/refs/" + head;
      std::ofstream file(parent);
      if (!file.is_open()) {
        std::cerr << "Error opening file: " << parent << std::endl;
        return false;
      }

      file << sha;
      file.close();

      return true;
    }
  }

  return false;
}

void Commit::execute() { createCommit(); }
} // namespace commit
} // namespace jh
