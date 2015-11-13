#include <storage/exceptions.h>

namespace storage {

PathException::PathException(const std::string& path)
        : util::BasicException("Path exception: " + path),
          path_(path) {
}

std::string PathException::GetPath() const {
    return path_;
}

PathIsFileException::PathIsFileException(const std::string& path)
        : PathException(path) {
}

PathCreateException::PathCreateException(const std::string& path)
        : PathException(path) {
}

}
