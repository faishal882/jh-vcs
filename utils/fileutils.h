#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>

namespace jh {
class fileUtils {
public:
  static std::string resolveFilePath(const std::string hash) {
    // find file
    std::string folder = hash.substr(0, 2);
    std::string file_name = hash.substr(2, 38);
    std::string filelocation = ".jh/objects/" + folder + "/" + file_name;
    return filelocation;
  }

  static int createFolder(const char *folderpath, mode_t mode) {
    if (mkdir(folderpath, mode) == -1) {
      if (errno != EEXIST) {
        std::cerr << "Error creating folder: " << folderpath << std::endl;
        return -1;
      }
    }

    return 0;
  }

  static bool createFile(std::string &hash, const std::vector<char> &data) {
    std::string folder = hash.substr(0, 2);
    std::string file_name = hash.substr(2, 38);
    std::string folder_location = ".jh/objects/" + folder;
    std::string file_location = folder_location + "/" + file_name;
    mode_t mode = S_IRWXU;
    if (createFolder(folder_location.c_str(), mode) != -1) {
      std::ofstream outFile(file_location, std::ios::binary);
      if (!outFile.is_open()) {
        std::cerr << "Failed to open output file: " << file_location
                  << std::endl;
        return false;
      }
      outFile.write(data.data(), data.size());
      outFile.close();

      return true;
    }

    return false;
  }
};
} // namespace jh
