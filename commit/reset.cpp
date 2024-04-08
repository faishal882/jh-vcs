#include "reset.h"

namespace jh {
namespace commit {
ResetCommit::ResetCommit(const char *hash) {
  this->hash = hash;
  std::cout << hash << std::endl;
}
} // namespace commit
} // namespace jh
