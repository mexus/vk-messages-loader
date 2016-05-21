#pragma once
#include <utils/exceptions.h>

namespace storage {

class PathException : public util::BasicException {
 public:
  PathException(const std::string& at, const std::string& path);
  std::string GetPath() const;

 private:
  const std::string path_;
};

class PathIsFileException : public PathException {
 public:
  PathIsFileException(const std::string& at, const std::string& path);
};

class PathCreateException : public PathException {
 public:
  PathCreateException(const std::string& at, const std::string& path);
};
}
