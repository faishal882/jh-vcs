#include "commit.h"

namespace jh {
namespace commit {

struct Blob *createBlob(size_t *size) {
  struct Blob *blob = (struct Blob *)malloc(sizeof(Blob));
  if (blob == NULL)
    return NULL;

  byte *blobBuffer = (byte *)malloc((*size) * sizeof(byte));
  if (blobBuffer == NULL)
    return NULL;

  blob->data = blobBuffer;
  blob->size = *size;

  return blob;
}

struct Tree *createTree(size_t size) {
  struct Tree *tree = (struct Tree *)malloc(sizeof(struct Tree));
  if (tree == NULL)
    return NULL;

  tree->nodes = (struct TreeNode **)malloc(size * sizeof(struct TreeNode *));
  tree->keys = (char **)malloc(size * sizeof(char *));
  if (tree->nodes == NULL || tree->keys == NULL) {
    free(tree);
    return NULL;
  }
  tree->size = size;
  return tree;
}

struct TreeNode *createBlobNode(struct Blob *blob) {
  struct TreeNode *node = (struct TreeNode *)malloc(sizeof(struct TreeNode));
  if (node == NULL) {
    return NULL; // Memory allocation failed
  }
  node->type = TreeNode::BLOB;
  node->data.blob = blob;
  return node;
}

struct TreeNode *createTreeNode(struct Tree *tree) {
  struct TreeNode *node = (struct TreeNode *)malloc(sizeof(struct TreeNode));
  if (node == NULL) {
    return NULL; // Memory allocation failed
  }
  node->type = TreeNode::TREE;
  node->data.tree = tree;
  return node;
}

// Function to create a new commit
struct Commit *createCommit(struct Commit *parent, const char *author,
                            const char *message, struct TreeNode *snapshot) {
  struct Commit *commit = (struct Commit *)malloc(sizeof(struct Commit));
  if (commit == NULL)
    return NULL; // Memory allocation failed

  commit->parent = (struct Commit *)malloc(sizeof(struct Commit *));
  if (commit->parent == NULL) {
    free(commit); // Free the allocated memory
    return NULL;  // Memory allocation failed
  }

  commit->author = author;
  commit->message = message;
  commit->snapshot = snapshot;
  return commit;
}

void freeTreeNode(struct TreeNode *node) {
  if (node == NULL)
    return;

  if (node->type == TreeNode::TREE) {
    free(node->data.tree);
  } else {
    free(node->data.blob);
  }
  free(node);
}

void freeBlob(struct Blob *blob) {
  if (blob == NULL)
    return;

  free(blob->data);
  // free(blob->size);
  free(blob);
}

void freeTree(struct Tree *tree) {
  if (tree == NULL)
    return;

  for (size_t i = 0; i < tree->size; ++i) {
    freeTreeNode(tree->nodes[i]);
    free(tree->keys[i]);
  }
  free(tree->nodes);
  free(tree->keys);
  free(tree);
}

void freeCommit(struct Commit *commit) {
  if (commit == NULL)
    return;

  freeCommit(commit->parent);
  freeTreeNode(commit->snapshot);
  free(commit);
}

struct Blob *readFileIntoBuffer(const char *filename, size_t *size) {
  FILE *file = fopen("file.txt", "rb");
  std::cout << file << std::endl;
  if (file == NULL) {
    printf("Error opening file %s\n", filename);
    return NULL;
  }

  // Get the file size
  fseek(file, 0, SEEK_END);
  *size = ftell(file);
  fseek(file, 0, SEEK_SET);

  // Allocate memory for the buffer
  struct Blob *blob = createBlob(size);

  if (blob == NULL) {
    fclose(file);
    return NULL;
  }

  // Read the file into the buffer
  size_t bytesRead = fread(blob->data, 1, *size, file);
  if (bytesRead != *size) {
    printf("Error reading file %s\n", filename);
    fclose(file);
    freeBlob(blob);
    return NULL;
  }

  fclose(file);
  return blob;
}

int Commit::commitex(const char *filename, const char *author,
                     const char *message) {
  // Example usage:
  size_t fileSize;
  struct Blob *blob = readFileIntoBuffer("file.py", &fileSize);
  if (blob == NULL) {
    printf("Error reading file contents\n");
    return 1;
  }

  // Create a blob node
  struct TreeNode *blobNode = createBlobNode(blob);
  if (blob == NULL) {
    printf("Memory allocation failed\n");
    freeBlob(blob); // Free the allocated memory
    return 1;
  }

  // Create a tree map (directory)
  struct Tree *tree = createTree(5);
  if (tree == NULL) {
    printf("Memory allocation failed\n");
    freeTreeNode(blobNode); // Free the allocated memory
    return 1;
  }

  // Insert blob node into tree map (e.g., with a file name)
  tree->nodes[0] = blobNode;
  tree->keys[0] = strdup("example"); // strdup is used to duplicate the string
  if (tree->keys[0] == NULL) {
    printf("Memory allocation failed\n");
    freeTreeNode(blobNode); // Free the allocated memory
    freeTree(tree);         // Free the allocated memory
    return 1;
  }

  struct TreeNode *treeNode = createTreeNode(tree);

  // Create a commit
  struct Commit *commit = createCommit(NULL, author, message, treeNode);
  if (commit == NULL) {
    printf("Memory allocation failed\n");
    freeTree(tree); // Free the allocated memory
    return 1;
  }
  // Free the allocated memory
  freeCommit(commit);
  return 0;
}
// int dumpBytesIntoFile(const char *filename, const byte *data, size_t size);
} // namespace commit
} // namespace jh
