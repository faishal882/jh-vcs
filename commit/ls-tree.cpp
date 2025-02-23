#include "../include/commit/ls-tree.h"
#include <cstring>

namespace jh {
namespace commit {

std::string decompressString(const std::string &compressedData) {
  std::string decompressedData;

  z_stream zs;
  memset(&zs, 0, sizeof(zs));

  if (inflateInit(&zs) != Z_OK) {
    throw std::runtime_error("inflateInit failed while decompressing.");
  }

  zs.next_in = (Bytef *)compressedData.data();
  zs.avail_in = compressedData.size();

  int ret;
  char outbuffer[32768];
  do {
    zs.next_out = reinterpret_cast<Bytef *>(outbuffer);
    zs.avail_out = sizeof(outbuffer);

    ret = inflate(&zs, 0);

    if (decompressedData.size() < zs.total_out) {
      decompressedData.append(outbuffer,
                              zs.total_out - decompressedData.size());
    }
  } while (ret == Z_OK);

  inflateEnd(&zs);

  if (ret != Z_STREAM_END) {
    std::cerr << "Error during zlib decompression: (" << ret << ") " << zs.msg
              << std::endl;
    throw std::runtime_error("Error during zlib decompression");
  }

  return decompressedData;
}

std::vector<TreeEntry> lsTree::parse_tree_object(const std::string &input) {
  std::vector<TreeEntry> result;

  size_t pos = 0;
  while (pos < input.size()) {
    TreeEntry entry;

    // Extract mode
    entry.mode = input.substr(pos, 6); // Assuming mode is always 6 characters
    pos += 6;

    // Extract name
    size_t nameStart = pos;
    size_t nameEnd = input.find_first_of('\0', nameStart);
    entry.name = input.substr(nameStart, nameEnd - nameStart);
    pos = nameEnd + 1;

    // Extract compressed hash
    // entry.sha = decompressString(input.substr(pos, 20));
    entry.sha = input.substr(pos, 20);
    pos += 20;

    // Skip the rest until the next entry
    pos = input.find("100644", pos);

    // Add the entry to the result
    result.push_back(entry);
  }

  return result;
}

bool lsTree::decompressFile(const std::string &inputFile, std::string &data) {
  std::ifstream inFile(inputFile, std::ios::binary);
  if (!inFile.is_open()) {
    std::cerr << "Failed to open input file: " << inputFile << std::endl;
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
    inFile.close(); // Close the input file
    return false;
  }

  const int CHUNK_SIZE = 1024; // Size of the buffer for reading input
  unsigned char inBuffer[CHUNK_SIZE];
  unsigned char outBuffer[CHUNK_SIZE];
  std::stringstream decompressedStream;

  // Decompress the file
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
        inFile.close();      // Close the input file
        return false;
      }
      size_t have = CHUNK_SIZE - stream.avail_out;
      decompressedStream.write(reinterpret_cast<char *>(outBuffer), have);
    } while (stream.avail_out == 0);
  } while (true);

  // Clean up zlib resources
  inflateEnd(&stream);

  // Close input and output files
  inFile.close();
  data = decompressedStream.str();
  return true;
}

std::string lsTree::resolveFile() {
  // find file
  std::string folder = hash.substr(0, 2);
  std::string file_name = hash.substr(2, 38);
  std::string filelocation = ".git/objects/" + folder + "/" + file_name;
  return filelocation;
}

bool lsTree::decompressFolder() {
  std::string data;
  bool _decompreesed = decompressFile(resolveFile(), data);
  std::cout << "_decompreesed: " << data << std::endl;
  std::vector<TreeEntry> tree = parse_tree_object(data);
  for (TreeEntry i : tree) {
    std::cout << i.mode << " " << i.sha << " " << i.name << std::endl;
  }
  return true; // All files decompressed successfully
}

} // namespace commit
} // namespace jh
