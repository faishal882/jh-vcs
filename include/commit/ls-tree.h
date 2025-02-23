#pragma once

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <zlib.h>

namespace jh {
namespace commit {

struct TreeEntry {
  std::string mode;
  std::string name;
  std::string sha;
};

class lsTree {
  std::string hash;

public:
  lsTree(std::string hash) { this->hash = hash; };
  std::string resolveFile();
  std::vector<TreeEntry> parse_tree_object(const std::string &tree_object);
  bool decompressFile(const std::string &inputFile, std::string &data);
  bool decompressFolder();
};
} // namespace commit
} // namespace jh
