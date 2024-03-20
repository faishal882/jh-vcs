#pragma once

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <openssl/sha.h>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>
#include <zlib.h>

namespace jh {
namespace commit {

namespace fs = std::filesystem;
class Tree {
public:
  fs::path directoryPath;
  std::vector<std::pair<char, std::string>> files;

  Tree(const std::string path);

  std::string sha1(const std::string &data);
  bool compress(std::stringstream &ss, std::string &data);

  int createFolder(const char *folderpath, mode_t mode);
  bool createFile(std::string &hash, std::string &data);

  std::vector<std::pair<char, std::string>>
  getFilesAndFolders(const fs::path &directory);

  bool createTree(std::stringstream &tree);

  // private:
  void execute();
};
} // namespace commit
} // namespace jh
