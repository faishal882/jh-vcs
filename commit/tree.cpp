#include "../include/commit/tree.h"

namespace jh {
namespace commit {

Tree::Tree() {
  directoryPath = ".";
  files = getFilesAndFolders(directoryPath);
  tree = tree_str();
}

Tree::Tree(const std::string path) {
  directoryPath = path;
  files = getFilesAndFolders(directoryPath);
  tree = tree_str();
}

// check if given string is present in a string
bool Tree::containsStr(const std::string &str) {
  size_t found = str.find(".jh");
  return (found != std::string::npos);
}

std::vector<std::pair<char, std::string>>
Tree::getFilesAndFolders(const fs::path &directory) {
  std::vector<std::pair<char, std::string>> filesAndFolders;

  for (const auto &entry : fs::directory_iterator(directory)) {
    if (fs::is_directory(entry)) {
      if (!containsStr(entry.path().string())) {
        std::pair<char, std::string> p =
            std::make_pair('D', entry.path().string());
        filesAndFolders.push_back(p);
      }
    } else if (fs::is_regular_file(entry)) {
      std::pair<char, std::string> p =
          std::make_pair('F', entry.path().string());
      filesAndFolders.push_back(p);
    }
  }

  return filesAndFolders;
}

bool Tree::createTree(std::stringstream &tree,
                      std::vector<std::pair<char, std::string>> &files) {
  for (const auto &entry : files) {
    if (entry.first == 'F') {
      /* read file data, compress data, create hash of compressed data, store it
       in tree with metadata */
      std::stringstream ss;
      std::vector<char> data;

      std::ifstream inFile(entry.second, std::ios::binary);
      if (!inFile.is_open()) {
        std::cerr << "Failed to open input file: " << entry.second << std::endl;
        return false;
      }
      ss << inFile.rdbuf();
      inFile.close();

      if (Zlib::compress(ss, data)) {
        std::string sha;
        sha = Zlib::sha1(data);
        fileUtils::createFile(sha, data);
        tree << "10065"
             << "\t"
             << "blob"
             << "\t" << entry.second << "\t" << sha << std::endl;

        // std::string test_tree_str = tree.str();
        // std::cout << "test_tree_str" << test_tree_str << std::endl;
      }
    } else if (entry.first == 'D') {
      std::vector<std::pair<char, std::string>> subFiles =
          getFilesAndFolders(entry.second);
      std::stringstream subTree;
      createTree(subTree, subFiles);
      std::vector<char> data;
      bool compressed = Zlib::compress(subTree, data);
      if (compressed) {
        std::string sha = Zlib::sha1(data);
        fileUtils::createFile(sha, data);
        tree << "10075"
             << "\t"
             << "tree"
             << "\t" << entry.second << "\t" << sha << std::endl;
      }
    }
  }

  return true;
};

std::string Tree::tree_str() {
  std::stringstream tree;
  bool create_tree = createTree(tree, this->files);
  if (create_tree)
    return tree.str();
  return "";
}

// void Tree::execute() {
//   std::stringstream tree;
//   createTree(tree, this->files);
//   std::cout << tree.str() << std::endl;
// }
} // namespace commit
} // namespace jh
