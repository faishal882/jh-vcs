#include "../include/utils/remoteutils.h"

namespace jh {
void remoteUtils::setRemoteUrl(const std::string &remoteName,
                               const std::string &url) {
  std::ofstream configFile(".jh/config", std::ios::app);
  if (configFile.is_open()) {
    configFile << "[remote \"" << remoteName << "\"]\n";
    configFile << "url = " << url << "\n";
    configFile.close();
  } else {
    std::cerr << "Failed to open .jh/config file" << std::endl;
  }
}

std::string remoteUtils::getRemoteUrl(const std::string &remoteName) {
  std::ifstream configFile(".jh/config");
  std::string line;
  std::string url;
  bool foundRemote = false;

  while (std::getline(configFile, line)) {
    if (line.find("[remote \"" + remoteName + "\"]") != std::string::npos) {
      foundRemote = true;
    }
    if (foundRemote && line.find("url = ") != std::string::npos) {
      url = line.substr(line.find("url = ") + 6);
      break;
    }
  }

  configFile.close();
  return url;
}

} // namespace jh