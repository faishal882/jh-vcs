#include "tree.h"

namespace jh {
namespace commit {

Tree::Tree() {
  std::cout << "Please pass directoryPath, default cwd; " << std::endl;
  directoryPath = ".";
  files = getFilesAndFolders(directoryPath);
  tree = tree_str();
}

Tree::Tree(const std::string path) {
  directoryPath = path;
  files = getFilesAndFolders(directoryPath);
  tree = tree_str();
}

std::vector<std::pair<char, std::string>>
Tree::getFilesAndFolders(const fs::path &directory) {
  std::vector<std::pair<char, std::string>> filesAndFolders;

  for (const auto &entry : fs::directory_iterator(directory)) {
    if (fs::is_directory(entry)) {
      std::pair<char, std::string> p =
          std::make_pair('D', entry.path().string());
      filesAndFolders.push_back(p);
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
  for (auto i : files) {
    if (i.first == 'F') {
      // TODO {store compress file, create hash, get hash, get filename}
      std::stringstream ss;
      std::string data;

      std::ifstream inFile(i.second, std::ios::binary);
      if (!inFile.is_open()) {
        std::cerr << "Failed to open input file: " << i.second << std::endl;
      }
      ss << inFile.rdbuf();

      std::string test_ss_str = ss.str();

      if (Zlib::compress(ss, data)) {
        std::string sha;
        sha = Zlib::sha1(data);
        fileUtils::createFile(sha, data);
        tree << "10065"
             << "\t"
             << "blob"
             << "\t" << i.second << "\t" << sha << std::endl;

        std::string test_tree_str = tree.str();
      }
    } else {
      // TODO {recurively}
      std::stringstream subTree;
      std::string data;
      auto subFiles = getFilesAndFolders(i.second);
      createTree(subTree, subFiles);
      std::string test_subtree_str = tree.str();
      bool compressed = Zlib::compress(subTree, data);
      if (compressed) {
        std::string sha = Zlib::sha1(data);
        tree << "10075"
             << "\t"
             << "tree"
             << "\t" << i.second << "\t" << sha << std::endl;
        std::string test_tree_str = tree.str();
      }
    }
  }

  std::string data;
  bool compressed = false;
  if (tree.str() != "")
    compressed = Zlib::compress(tree, data);
  if (compressed) {
    std::string sha = Zlib::sha1(data);
    std::string test_tree_str = tree.str();
    // std::cout << "SHA: " << sha << std::endl
    //           << "TREE: " << tree.str() << std::endl;
    // std::cout << "D: ";
    // for (auto i : files) {
    //   std::cout << i.first << " " << i.second << ",";
    // }
    // std::cout << std::endl;
    fileUtils::createFile(sha, data);
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
