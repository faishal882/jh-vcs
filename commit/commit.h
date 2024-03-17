#pragma once

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace jh {
namespace commit {

typedef unsigned char byte;

struct Blob {
  byte *data;
  size_t size;
};

struct Tree {
  struct TreeNode **nodes;
  char **keys;
  size_t size;
};

struct TreeNode {
  enum { TREE, BLOB } type;
  union {
    struct Tree *tree;
    struct Blob *blob;
  } data;
};

struct Commit {
  struct Commit *parent;
  const char *author;
  const char *message;
  struct TreeNode *snapshot;

  static int commitex(const char *filename, const char *author,
                      const char *message);
  // struct Blob *createBlob(size_t size);
  // struct Tree *createTree(size_t size);

  // struct TreeNode *createBlobNode(byte *data);
  // struct TreeNode *createTreeNode(struct Tree *tree);

  // // Function to create a new commit
  // static struct Commit commit(struct Commit *parent, char *author,
  // char *message, struct TreeNode *snapshot);

  // void freeTreeNode(struct TreeNode *node);
  // void freeTree(struct Tree *treeMap);
  // void freeCommit(struct Commit *commit);

  // byte *readFileIntoBuffer(const char *filename, size_t *size);
  // int dumpBytesIntoFile(const char *filename, const byte *data, size_t
  // size);
};

} // namespace commit
} // namespace jh
