#include "../include/commit/reset.h"

namespace jh {
namespace commit {
ResetCommit::ResetCommit(const std::string &hash) { this->hash = hash; }

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
ResetCommit::getBlobsandTrees(const std::string &treeHash) {
  std::string tree = deCompressTree(treeHash);

  std::vector<std::vector<std::string>> blobsAndTrees;

  std::istringstream ss(tree);
  std::string line;

  while (std::getline(ss, line)) {
    std::istringstream iss(line);

    std::string permission, type, name, sha1;
    if (iss >> permission >> type >> name >> sha1) {
      if (type == "blob") {
        std::vector<std::string> temp{permission, type, name, sha1};
        blobsAndTrees.push_back(temp);
      } else if (type == "tree") {
        std::vector<std::vector<std::string>> subBlobsAndTrees =
            getBlobsandTrees(sha1);
        blobsAndTrees.insert(blobsAndTrees.end(), subBlobsAndTrees.begin(),
                             subBlobsAndTrees.end());
      }
    } else {
      // Handle extraction failure
      std::cerr << "Failed to extract data from line: " << line << std::endl;
    }
  }

  return blobsAndTrees;
}

/* function which takes hash of blob and replaces the file contents of
 * corresponding file */
bool ResetCommit::deCompressBlob(const std::string &hash,
                                 const std::string &filename) {
  std::string file = fileUtils::resolveFilePath(hash);

  std::ifstream inFile(file, std::ios::binary);
  if (!inFile.is_open()) {
    std::cerr << "Failed to open input file: " << file << std::endl;
    return false;
  }

  std::stringstream ss;
  ss << inFile.rdbuf();
  inFile.close();

  std::string decompressedData;

  bool decompressed = Zlib::decompress(ss, decompressedData);
  if (!decompressed or decompressedData == "")
    return false;

  std::ofstream outFile(filename, std::ios::binary);
  if (!outFile.is_open()) {
    std::cerr << "Failed to open outFile file: " << filename << std::endl;
    return false;
  }

  outFile.write(decompressedData.c_str(), decompressedData.length());

  outFile.close();

  return true;
}

void ResetCommit::execute() {
  std::string commit = deCompressCommit(this->hash);

  if (commit != "") {
    std::string treeHash = extractTreeHash(commit);

    if (treeHash != "") {
      std::vector<std::vector<std::string>> allBlobsandTrees =
          getBlobsandTrees(treeHash);

      for (auto const &i : allBlobsandTrees) {

        bool contents = deCompressBlob(i[3], i[2]);
        if (contents)
          std::cout << "SUCCEED"
                    << "  " << i[2] << std::endl;
      }
    }
  }
}
} // namespace commit
} // namespace jh
