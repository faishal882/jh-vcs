#include "reset.h"

namespace jh {
namespace commit {
ResetCommit::ResetCommit(const std::string &hash) {
  this->hash = hash;
  // std::cout << hash << std::endl;
}

std::string ResetCommit::deCompressCommit(const std::string &hash) {
  std::string file = fileUtils::resolveFilePath(hash);
  std::ifstream inFile(file, std::ios::binary);
  if (!inFile.is_open()) {
    std::cerr << "Failed to open input file: " << file << std::endl;
    return "";
  }

  std::stringstream ss;
  ss << inFile.rdbuf();
  inFile.close();

  std::string data;

  bool decompressed = Zlib::decompress(ss, data);

  if (decompressed)
    return data;

  return "";
}

std::string ResetCommit::extractTreeHash(const std::string &commit) {
  // Regex pattern to match the Tree key and its associated SHA hash
  std::regex pattern(R"(Tree:\s+(\w{40}))");

  // Search for matches in the input string
  std::smatch match;
  if (std::regex_search(commit, match, pattern)) {
    // Extract the SHA hash captured by the first capturing group
    return match.str(1);
  }

  return "";
}

std::string ResetCommit::deCompressTree(const std::string &hash) {
  std::string file = fileUtils::resolveFilePath(hash);
  std::ifstream inFile(file, std::ios::binary);
  if (!inFile.is_open()) {
    std::cerr << "Failed to open input file: " << file << std::endl;
    return "";
  }

  std::stringstream ss;
  ss << inFile.rdbuf();
  inFile.close();

  std::string data;

  bool decompressed = Zlib::decompress(ss, data);
  if (decompressed)
    return data;

  return "";
}

std::vector<std::vector<std::string>>
ResetCommit::getBlobsandTrees(std::string &tree) {
  std::vector<std::vector<std::string>> blobsandTrees;

  std::string permission;
  std::string type;
  std::string name;
  std::string sha1;

  std::stringstream ss(tree);
  std::string line;

  while (std::getline(ss, line)) {
    std::istringstream iss(line);

    iss >> permission;
    iss >> type;
    iss >> name;
    iss >> sha1;

    std::vector<std::string> temp;
    temp.push_back(type);
    temp.push_back(name);
    temp.push_back(sha1);

    blobsandTrees.push_back(temp);
  }

  return blobsandTrees;
}

std::string ResetCommit::deCompressBlob(const std::string &hash) {
  std::string file = fileUtils::resolveFilePath(hash);
  std::ifstream inFile(file, std::ios::binary);
  if (!inFile.is_open()) {
    std::cerr << "Failed to open input file: " << file << std::endl;
    return "";
  }

  std::stringstream ss;
  ss << inFile.rdbuf();
  inFile.close();

  std::string data;

  bool decompressed = Zlib::decompress(ss, data);
  if (decompressed)
    return data;

  return "";
}

bool ResetCommit::replaceFileContents(const std::string &filename,
                                      const std::string &content) {
  std::ofstream file(filename);
  if (!file.is_open()) {
    return false;
  }

  file << content;
  file.close();
  return true;
}

bool createFile(const char *hash, const char *filename);

void ResetCommit::execute() {
  std::string commit = deCompressCommit(this->hash);

  if (commit != "") {
    std::string treeHash = extractTreeHash(commit);
    std::cout << treeHash << std::endl;

    if (treeHash != "") {
      std::string tree = deCompressTree(treeHash);

      if (tree != "") {
        std::vector<std::vector<std::string>> blobsandTrees =
            getBlobsandTrees(tree);

        if (!blobsandTrees.empty()) {
          for (auto const &i : blobsandTrees) {

            if (i[0] == "blob") {
              std::string contents = deCompressBlob(i[2]);

              if (contents != "")
                replaceFileContents(i[1], contents);
            }
          }
        }
      }
    }
  }
}
} // namespace commit
} // namespace jh
