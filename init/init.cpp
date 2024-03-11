#include "init.h"

namespace jh {
namespace init {
Init::Init(const char *filepath) {
  repository = "";
  objects = "objects";
  branches = "branches";
  this->filepath = resolveFilePath(filepath, ".jh");
  mode = S_IRWXU;

  // execute();
};

Init::Init(const char *filepath, const char *repository, mode_t mode) {
  this->repository = repository;
  objects = "objects";
  branches = "branches";
  this->filepath = resolveFilePath(filepath, repository);
  mode = S_IRWXU;

  // execute();
}

// creates folders in provided filepath if success return 0 else return -1
int Init::createFolder(const char *filepath, mode_t mode) {
  if (mkdir(filepath, mode) == -1) {
    if (errno != EEXIST) {
      fprintf(stderr, "Error creating folder: %s\n", strerror(errno));
      return -1;
    }
  }

  return 0;
}

int Init::createConfig(const char *filepath, mode_t mode) {
  FILE *file = fopen(resolveFilePath(filepath, "config"), "w");
  if (file == NULL) {
    fprintf(stderr, "Error creating config: %s\n", strerror(errno));
    return -1;
  }
  fprintf(file, "This is config file all config stays here\n");
  fclose(file);

  return 0;
}

int Init::createHead(const char *filepath, mode_t mode) {
  FILE *file = fopen(resolveFilePath(filepath, "HEAD"), "w");
  if (file == NULL) {
    fprintf(stderr, "Error creating HEAD: %s\n", strerror(errno));
    return -1;
  }
  fprintf(file, "ref: refs/heads/master");
  fclose(file);

  return 0;
}

// deletes a folder for rollback
int Init::deleteFolder(const char *filepath) {
  if (rmdir(filepath) != 0) {
    return -1; // failure
  }
  return 0;
}

int Init::deleteFile(const char *filepath) {
  if (unlink(filepath) != 0)
    return -1; // failure
  return 0;
}

const char *Init::resolveFilePath(const char *filepath1,
                                  const char *filepath2) const {
  size_t len1 = strlen(filepath1);
  size_t len2 = strlen(filepath2);
  size_t len_filepath = len1 + len2 + 2;

  char *filepath = new char[len_filepath];
  strcpy(filepath, filepath1);
  strcat(filepath, "/");
  strcat(filepath, filepath2);

  return filepath;
}

int Init::execute() {
  if (createFolder(filepath, mode) == -1) {
    return -1;
  }

  const char *subfolderpath = resolveFilePath(filepath, objects);
  if (createFolder(subfolderpath, mode) == -1) {
    deleteFolder(filepath);
    return -1;
  }

  const char *subfolderpath2 = resolveFilePath(filepath, branches);
  if (createFolder(subfolderpath2, mode) == -1) {
    deleteFolder(filepath);
    return -1;
  }

  if (createConfig(filepath, mode) == -1) {
    deleteFolder(filepath);
    return -1;
  }
  if (createHead(filepath, mode) == -1) {
    deleteFolder(filepath);
    return -1;
  }

  return 0;
};
} // namespace init
}; // namespace jh
