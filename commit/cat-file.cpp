#include "cat-file.h"

namespace jh {
namespace commit {
catFile::catFile(const std::string hash) { this->hash = hash; };

std::string catFile::resolveFilePath() {
  // find file
  std::string folder = hash.substr(0, 2);
  std::string file_name = hash.substr(2, 38);
  std::string filelocation = ".git/objects/" + folder + "/" + file_name;
  return filelocation;
}

bool catFile::decompressFile() {
  std::string file = resolveFilePath();
  std::ifstream inFile(file, std::ios::binary);
  if (!inFile.is_open()) {
    std::cerr << "Failed to open input file: " << file << std::endl;
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
      std::cout.write(reinterpret_cast<char *>(outBuffer), have);
    } while (stream.avail_out == 0);
  } while (true);

  // Clean up zlib resources
  inflateEnd(&stream);

  // Close input files
  inFile.close();
  return true;
}
} // namespace commit
} // namespace jh
