#include <storage/exceptions.h>

namespace storage {

PathException::PathException(const std::string& at, const std::string& path)
        : util::BasicException(at, "Path exception: " + path),
          path_(path) {
}

std::string PathException::GetPath() const {
    return path_;
}

PathIsFileException::PathIsFileException(const std::string& at, const std::string& path)
        : PathException(at, path) {
}

PathCreateException::PathCreateException(const std::string& at, const std::string& path)
        : PathException(at, path) {
}

}
