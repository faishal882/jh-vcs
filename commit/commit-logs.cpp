#include "commit-logs.h"

namespace jh {
namespace commit {

commitLog::commitLog() { logs = getLogs(); };

std::stringstream commitLog::getLogs() {
  std::stringstream logs;
  std::string file = ".jh/logs/HEAD";

  std::ifstream logFile(file, std::ios::binary);
  if (!logFile.is_open()) {
    std::cerr << "Failed to open log file: " << file << std::endl;
    return logs;
  }

  logs << logFile.rdbuf();

  logFile.close();

  return logs;
}

std::vector<std::vector<std::string>> commitLog::extractInfo() {
  std::vector<std::vector<std::string>> logs;
  std::string line;

  while (std::getline(this->logs, line)) {
    std::istringstream iss(line);

    std::string parent, hash, author, timestamp, message;
    if (iss >> parent >> hash >> author >> timestamp) {
      // extracting commit message
      auto pos = line.find("commit:");
      if (pos != std::string::npos)
        message = line.substr(pos + sizeof("commit:") - 1);
      else
        message = "";
      std::vector<std::string> temp{hash, author, timestamp, message};
      logs.push_back(temp);
    } else {
      // Handle extraction failure
      std::cerr << "Failed to extract data from line: " << line << std::endl;
    }
  }

  return logs;
}

std::string commitLog::representTime(const std::string &timestamp) {
  time_t time = std::stol(timestamp);
  std::stringstream ss;
  ss << std::put_time(std::localtime(&time), "%a %b %d %T %Y %z");
  return ss.str();
}

void commitLog::execute() {
  std::vector<std::vector<std::string>> logs = extractInfo();

  char timestampStr[20];
  for (const auto &i : logs) {
    std::cout << "commit: " << i[0] << std::endl
              << "Author: " << i[1] << std::endl
              << "Date: " << representTime(i[2]) << std::endl;
    std::cout << std::endl << "\t" << i[3] << std::endl << std::endl;
  }
}

} // namespace commit
} // namespace jh
