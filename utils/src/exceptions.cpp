#include <utils/exceptions.h>

namespace util {

BasicException::BasicException(const std::string& at, const std::string& message)
        : std::runtime_error("At " + at + ": " + message),
          at_(at),
          message_(message) {
}

std::string BasicException::GetAt() const {
    return at_;
}

std::string BasicException::GetMessage() const {
    return message_;
}

FileException::FileException(const std::string& at, Operation operation, const std::string& file_name)
        : BasicException(at, "Operation #" + std::to_string(operation) + " has failed on a file '" + file_name + "'"),
          operation_(operation),
          file_name_(file_name) {
}

FileException::Operation FileException::GetOperation() const {
    return operation_;
}

std::string FileException::GetFileName() const {
    return file_name_;
}

FileReadException::FileReadException(const std::string& at, const std::string& file_name)
        : FileException(at, FileException::READ, file_name) {
}

FileWriteException::FileWriteException(const std::string& at, const std::string& file_name)
        : FileException(at, FileException::WRITE, file_name) {
}

}
