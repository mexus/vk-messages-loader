#pragma once
#include <stdexcept>

namespace util {

class BasicException: public std::runtime_error {
public:
    BasicException(const std::string& message);
};

class FileException: public BasicException {
public:
    enum Operation {
        READ = 0,
        WRITE = 1
    };

    FileException(Operation operation, const std::string& file_name);
    Operation GetOperation() const;
    std::string GetFileName() const;

private:
    const Operation operation_;
    const std::string file_name_;
};

class FileReadException: public FileException {
public:
    FileReadException(const std::string& file_name);
};

class FileWriteException: public FileException {
public:
    FileWriteException(const std::string& file_name);
};

}
