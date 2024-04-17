#pragma once

#include <iomanip>
#include <iostream>
#include <openssl/sha.h>
#include <sstream>
#include <zlib.h>

namespace jh {

class Zlib {
public:
  static bool compress(std::stringstream &ss, std::string &data) {
    // Set up zlib structures
    z_stream stream;
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;

    if (deflateInit(&stream, Z_BEST_COMPRESSION) != Z_OK) {
      std::cerr << "Failed to initialize zlib for compression" << std::endl;
      return false;
    }

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

    data = compressedStream.str();

    return true;
  }

  static std::string sha1(const std::string &data) {
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

  static bool decompress(std::stringstream &ss, std::string &data) {
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

    std::stringstream deCompressedStream;

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
        int ret = inflate(&stream, Z_NO_FLUSH);
        if (ret == Z_STREAM_ERROR) {
          std::cerr << "Error during decompression: " << stream.msg
                    << std::endl;
          inflateEnd(&stream); // Clean up zlib resources
          return false;
        }
        size_t have = CHUNK_SIZE - stream.avail_out;
        deCompressedStream.write(reinterpret_cast<const char *>(outBuffer),
                                 have);
      } while (stream.avail_out == 0);
    } while (true);

    inflateEnd(&stream);

    data = deCompressedStream.str();

    return true;
  }

  static bool decompressBlob(std::stringstream &ss, std::string &data) {
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

    std::stringstream deCompressedStream;

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
        int ret = inflate(&stream, Z_NO_FLUSH);
        if (ret == Z_STREAM_ERROR) {
          std::cerr << "Error during decompression: " << stream.msg
                    << std::endl;
          inflateEnd(&stream); // Clean up zlib resources
          return false;
        }
        size_t have = CHUNK_SIZE - stream.avail_out;
        deCompressedStream.write(reinterpret_cast<const char *>(outBuffer),
                                 have);
      } while (stream.avail_out == 0);
    } while (true);

    inflateEnd(&stream);

    data = deCompressedStream.str();

    return true;
  }
};
} // namespace jh
