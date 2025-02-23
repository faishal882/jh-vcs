#pragma once

#include <string>
#include <tuple>

namespace jh {
static std::tuple<std::string, std::string, std::string>
resoleCommands(int argc, char *argv[]) {
  // Default values
  std::string command = "command";
  std::string arg1 = "arg1";
  std::string flag = "flag";

  // Check if arguments are provided and assign them
  if (argc > 1) {
    command = argv[1];
  }
  if (argc > 2) {
    arg1 = argv[2];
  }
  if (argc > 3) {
    flag = argv[3];
  }

  return std::make_tuple(command, arg1, flag);
}
} // namespace jh
