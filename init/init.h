#pragma once

#include <iostream>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace jh {
namespace init {

#define FOLDER_SIZE 256

class Init {
private:
  const char *repository;
  const char *filepath;
  const char *branches;
  const char *objects;
  mode_t mode;

public:
  Init(const char *filepath);
  // creates folders in provided filepath if success return 0 else return -1
  Init(const char *filepath, const char *repository, mode_t mode);

  int createFolder(const char *filepath, mode_t mode);
  int createConfig(const char *filepath, mode_t mode);
  int createHead(const char *filepath, mode_t mode);

  int deleteFolder(const char *filepath);
  int deleteFile(const char *filepath);

  int execute();
  // private:

protected:
  const char *resolveFilePath(const char *str1, const char *str2) const;
};
} // namespace init
}; // namespace jh
