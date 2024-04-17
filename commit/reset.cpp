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

  std::ofstream outFile(filename, std::ios::binary);
  if (!outFile.is_open()) {
    std::cerr << "Failed to open outFile file: " << filename << std::endl;
    return false;
  }

  // Set up zlib structures
  z_stream stream;
  stream.zalloc = Z_NULL;
  stream.zfree = Z_NULL;
  stream.opaque = Z_NULL;
  stream.avail_in = 0;
  stream.next_in = Z_NULL;

  // Initialize zlib for decompression
  if (inflateInit(&stream) != Z_OK) {
    std::cerr << "Failed to initialize zlib for decompression" << std::endl;
    return false;
  }

  const int CHUNK_SIZE = 1024; // Size of the buffer for reading input
  unsigned char inBuffer[CHUNK_SIZE];
  unsigned char outBuffer[CHUNK_SIZE];

  do {
    inFile.read(reinterpret_cast<char *>(inBuffer), CHUNK_SIZE);
    stream.avail_in = inFile.gcount();

    if (stream.avail_in == 0) {
      break; // No more input data
    }
    stream.next_in = inBuffer;

    do {
      stream.avail_out = CHUNK_SIZE;
      stream.next_out = outBuffer;
      int ret = inflate(&stream, Z_NO_FLUSH);

      if (ret == Z_STREAM_ERROR) {
        std::cerr << "Error during decompression: " << stream.msg << std::endl;
        inflateEnd(&stream); // Clean up zlib resources
        return false;
      }

      size_t have = CHUNK_SIZE - stream.avail_out;
      outFile.write(reinterpret_cast<const char *>(outBuffer), have);

    } while (stream.avail_out == 0);
  } while (true);

  inflateEnd(&stream);

  inFile.close();
  outFile.close();

  return true;
}

// bool ResetCommit::replaceFileContents(const std::string &filename,
// const std::string &content) {
// return true;
// }

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
              bool contents = deCompressBlob(i[2], i[1]);
              if (contents)
                std::cout << "SUCCEED" << i[1] << std::endl;
            }
          }
        }
      }
    }
  }
}
} // namespace commit
} // namespace jh
