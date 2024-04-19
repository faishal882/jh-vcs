#pragma once

#include <cassert>
#include <iomanip>
#include <iostream>
#include <openssl/sha.h>
#include <sstream>
#include <vector>
#include <zlib.h>

namespace jh {

#define CHUNK_SIZE 2048

class Zlib {
public:
  static bool compress(std::stringstream &ss, std::vector<char> &data) {
    int ret, flush;
    unsigned have;
    z_stream strm;

    unsigned char in[CHUNK_SIZE];
    unsigned char out[CHUNK_SIZE];

    /* allocate deflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    ret = deflateInit(&strm, Z_BEST_COMPRESSION);

    if (ret != Z_OK)
      return false;

    /* compress until end of file */
    do {
      ss.read(reinterpret_cast<char *>(in), CHUNK_SIZE);
      strm.avail_in = ss.gcount();

      if (ss.fail() && !ss.eof()) {
        (void)deflateEnd(&strm);
        return false;
      }

      flush = ss.eof() ? Z_FINISH : Z_NO_FLUSH;
      strm.next_in = in;

      /* run deflate() on input until output buffer not full, finish
            compression if all of source has been read in */
      do {
        strm.avail_out = CHUNK_SIZE;
        strm.next_out = out;
        ret = deflate(&strm, flush);   /* no bad return value */
        assert(ret != Z_STREAM_ERROR); /* state not clobbered */
        have = CHUNK_SIZE - strm.avail_out;

        data.insert(data.end(), out, out + have);
      } while (strm.avail_out == 0);
      assert(strm.avail_in == 0); /* all input will be used */

      /* done when last data in file processed */
    } while (flush != Z_FINISH);
    assert(ret == Z_STREAM_END); /* stream will be complete */

    (void)deflateEnd(&strm);

    return true;
  }

  static std::string sha1(const std::vector<char> &data) {
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char *>(data.data()), data.size(),
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
        std::cout << outBuffer;
      } while (stream.avail_out == 0);
    } while (true);

    inflateEnd(&stream);

    data = deCompressedStream.str();

    return true;
  }
};
} // namespace jh
