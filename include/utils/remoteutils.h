// utils/configutils.h
#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <string>

namespace jh {
class remoteUtils {
public:
  static void setRemoteUrl(const std::string &remoteName,
                           const std::string &url);

  static std::string getRemoteUrl(const std::string &remoteName);
};
} // namespace jh