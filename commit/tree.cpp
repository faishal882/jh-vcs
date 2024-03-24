#include "tree.h"

namespace jh {
namespace commit {

Tree::Tree() {
  std::cout << "Please pass directoryPath, default cwd; " << std::endl;
  directoryPath = ".";
}

Tree::Tree(const std::string path) {
  directoryPath = path;
  files = getFilesAndFolders(path);
}

std::string Tree::sha1(const std::string &data) {
  unsigned char hash[SHA_DIGEST_LENGTH];
  SHA1(reinterpret_cast<const unsigned char *>(data.c_str()), data.length(),
       hash);

  std::stringstream ss;
  ss << std::hex;
  for (unsigned char byte : hash) {
    ss << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
  }
  return ss.str();
}

bool Tree::compress(std::stringstream &ss, std::string &data) {
  // Set up zlib structures
  z_stream stream;
  stream.zalloc = Z_NULL;
  stream.zfree = Z_NULL;
  stream.opaque = Z_NULL;
  if (deflateInit(&stream, Z_BEST_COMPRESSION) != Z_OK) {
    std::cerr << "Failed to initialize zlib for compression" << std::endl;
    return false;
  }

  // Compress the data
  const int CHUNK_SIZE = 1024; // Size of the buffer for reading input
  unsigned char inBuffer[CHUNK_SIZE];
  unsigned char outBuffer[CHUNK_SIZE];
  std::stringstream compressedStream;

  do {
    ss.read(reinterpret_cast<char *>(inBuffer), CHUNK_SIZE);
    stream.avail_in = ss.gcount();
    if (stream.avail_in == 0) {
      break; // No more input data
    }
    stream.next_in = inBuffer;

    do {
      stream.avail_out = CHUNK_SIZE;
      stream.next_out = outBuffer;
      if (deflate(&stream, Z_FINISH) == Z_STREAM_ERROR) {
        std::cerr << "Error during compression" << std::endl;
        deflateEnd(&stream);
        return false;
      }
      size_t have = CHUNK_SIZE - stream.avail_out;
      compressedStream.write(reinterpret_cast<const char *>(outBuffer), have);
    } while (stream.avail_out == 0);
  } while (true);

  // Clean up zlib resources
  (void)deflateEnd(&stream);

  // Close input and output files
  data = compressedStream.str();

  return true;
}

// creates folders in provided filepath if success return 0 else return -1
int Tree::createFolder(const char *folderpath, mode_t mode) {
  if (mkdir(folderpath, mode) == -1) {
    if (errno != EEXIST) {
      std::cerr << "Error creating folder: " << folderpath << std::endl;
      return -1;
    }
  }

  return 0;
}

bool Tree::createFile(std::string &hash, std::string &data) {
  std::string folder = hash.substr(0, 2);
  std::string file_name = hash.substr(2, 38);
  std::string folder_location = ".jh/objects/" + folder;
  std::string file_location = folder_location + "/" + file_name;
  mode_t mode = S_IRWXU;
  if (createFolder(folder_location.c_str(), mode) != -1) {
    std::ofstream outFile(file_location, std::ios::binary);
    if (!outFile.is_open()) {
      std::cerr << "Failed to open output file: " << file_location << std::endl;
      return false;
    }
    outFile.write(data.c_str(), data.length());
    outFile.close();

    return true;
  }

  return false;
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

      if (compress(ss, data)) {
        std::string sha;
        sha = sha1(data);
        tree << "10065"
             << "\t"
             << "blob"
             << "\t" << i.second << "\t" << sha << std::endl;
      }
    } else {
      // TODO {recurively}
      std::stringstream subTree;
      std::string data;
      auto subFiles = getFilesAndFolders(i.second);
      createTree(subTree, subFiles);
      if (compress(subTree, data)) {
        std::string sha = sha1(data);
        tree << "10075"
             << "\t"
             << "tree"
             << "\t" << i.second << "\t" << sha << std::endl;
      }
    }
  }

  return true;
};

void Tree::execute() {
  std::stringstream tree;
  createTree(tree, this->files);
  std::cout << tree.str() << std::endl;
}
} // namespace commit
} // namespace jh
