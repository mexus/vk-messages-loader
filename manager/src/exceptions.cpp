#include <manager/exceptions.h>

namespace manager {

NoApplicationIdException::NoApplicationIdException(const std::string& at)
        : util::BasicException(at, "no application id") {
}

NoTokenException::NoTokenException(const std::string& at)
        : util::BasicException(at, "no token provided") {
}

}
