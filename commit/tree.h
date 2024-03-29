#pragma once

#include "../utils/fileutils.h"
#include "../utils/zlibutils.h"
#include <vector>

namespace jh {
namespace commit {

namespace fs = std::filesystem;
class Tree {
public:
  fs::path directoryPath;
  std::vector<std::pair<char, std::string>> files;

  Tree();
  Tree(const std::string path);

  // int createFolder(const char *folderpath, mode_t mode);
  // bool createFile(std::string &hash, std::string &data);

  std::vector<std::pair<char, std::string>>
  getFilesAndFolders(const fs::path &directory);

  bool createTree(std::stringstream &tree,
                  std::vector<std::pair<char, std::string>> &files);

  // private:
  void execute();
};
} // namespace commit
} // namespace jh
