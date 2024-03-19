#include "hash-blob.h"

namespace jh {
namespace commit {

hashBlob::hashBlob(std::string inputFile) { this->inputFile = inputFile; }

std::string hashBlob::sha1() {
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

bool hashBlob::compressFile() {
  // Open the input file for reading
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
  if (deflateInit(&stream, Z_BEST_COMPRESSION) != Z_OK) {
    std::cerr << "Failed to initialize zlib for compression" << std::endl;
    inFile.close();
    return false;
  }

  // Compress the data
  const int CHUNK_SIZE = 1024; // Size of the buffer for reading input
  unsigned char inBuffer[CHUNK_SIZE];
  unsigned char outBuffer[CHUNK_SIZE];
  std::stringstream compressedStream;

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
      if (deflate(&stream, Z_FINISH) == Z_STREAM_ERROR) {
        std::cerr << "Error during compression" << std::endl;
        deflateEnd(&stream);
        inFile.close();
        return false;
      }
      size_t have = CHUNK_SIZE - stream.avail_out;
      compressedStream.write(reinterpret_cast<const char *>(outBuffer), have);
    } while (stream.avail_out == 0);
  } while (true);

  // Clean up zlib resources
  (void)deflateEnd(&stream);

  // Close input and output files
  inFile.close();

  data = compressedStream.str();

  return true;
}

bool hashBlob::createFile(std::string &hash) {
  std::string folder = hash.substr(0, 2);
  std::string file_name = hash.substr(2, 38);

  std::string filelocation = ".jh/objects/" + folder + "/" + file_name;

  std::ofstream outFile(filelocation, std::ios::binary);
  if (!outFile.is_open()) {
    std::cerr << "Failed to open output file: " << filelocation << std::endl;
    return false;
  }
  outFile.write(data.c_str(), data.length());
  outFile.close();

  return false;
}

void hashBlob::execute() {
  compressFile();
  std::string hash = sha1();
  createFile(hash);
}
} // namespace commit
} // namespace jh
