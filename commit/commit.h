#pragma once

#include <iostream>

namespace jh {
namespace commit {
class Commit {
public:
  int welcome;
  char *commitMessage;

  Commit();

  int execute();
};
} // namespace commit
} // namespace jh
