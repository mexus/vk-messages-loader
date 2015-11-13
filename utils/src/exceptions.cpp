#include <utils/exceptions.h>

namespace util {

BasicException::BasicException(const std::string& message) : std::runtime_error(message) {
}

FileException::FileException(Operation operation, const std::string& file_name)
        : BasicException("Operation #" + std::to_string(operation) + " has failed on a file '" + file_name + "'"),
          operation_(operation),
          file_name_(file_name) {
}

FileException::Operation FileException::GetOperation() const {
    return operation_;
}

std::string FileException::GetFileName() const {
    return file_name_;
}

FileReadException::FileReadException(const std::string& file_name)
        : FileException(FileException::READ, file_name) {
}

FileWriteException::FileWriteException(const std::string& file_name)
        : FileException(FileException::WRITE, file_name) {
}

}
