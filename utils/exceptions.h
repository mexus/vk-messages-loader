#pragma once
#include <stdexcept>

#ifdef _MSC_VER
#define THROW_AT(E, ...)                                                    \
  throw E(                                                                  \
      std::string(__FILE__) + std::string("::") + std::to_string(__LINE__), \
      ##__VA_ARGS__)
#else
#define THROW_AT(E, ...) throw E(__PRETTY_FUNCTION__, ##__VA_ARGS__)
#endif

namespace util {

class BasicException : public std::runtime_error {
 public:
  BasicException(const std::string& at, const std::string& message);
  std::string GetAt() const;
  std::string GetText() const;

 private:
  const std::string at_;
  const std::string message_;
};

class FileException : public BasicException {
 public:
  enum Operation { READ = 0, WRITE = 1 };

  FileException(const std::string& at, Operation operation,
                const std::string& file_name);
  Operation GetOperation() const;
  std::string GetFileName() const;

 private:
  const Operation operation_;
  const std::string file_name_;

  static std::string OperationToString(Operation operation);
};

class FileReadException : public FileException {
 public:
  FileReadException(const std::string& at, const std::string& file_name);
};

class FileWriteException : public FileException {
 public:
  FileWriteException(const std::string& at, const std::string& file_name);
};
}
