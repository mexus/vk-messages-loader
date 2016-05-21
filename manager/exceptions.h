#pragma once
#include <utils/exceptions.h>

namespace manager {

class NoApplicationIdException : public util::BasicException {
 public:
  NoApplicationIdException(const std::string& at);
};

class NoTokenException : public util::BasicException {
 public:
  NoTokenException(const std::string& at);
};

namespace cache {

class NoDataException : public util::BasicException {
 public:
  NoDataException(const std::string& at, uint64_t id);
  uint64_t GetId() const;

 private:
  uint64_t id_;
};
}
}
