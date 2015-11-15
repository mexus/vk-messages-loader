#include <manager/exceptions.h>

namespace manager {

NoApplicationIdException::NoApplicationIdException(const std::string& at)
        : util::BasicException(at, "no application id") {
}

NoTokenException::NoTokenException(const std::string& at)
        : util::BasicException(at, "no token provided") {
}

namespace cache {

NoDataException::NoDataException(const std::string& at, uint64_t id)
        : util::BasicException(at, "No data for id " + std::to_string(id)),
          id_(id) {
}

uint64_t NoDataException::GetId() const {
    return id_;
}

}

}
