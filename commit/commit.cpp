#include "commit.h"

namespace jh {
namespace commit {
Commit::Commit() {
  welcome = 100;
  commitMessage = "Initial Commit";
}

int Commit::execute() {
  std::cout << welcome << " " << commitMessage << std::endl;
  return 0;
}
} // namespace commit
} // namespace jh
