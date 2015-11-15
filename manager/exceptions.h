#pragma once
#include <utils/exceptions.h>

namespace manager {

class NoApplicationIdException: public util::BasicException {
public:
    NoApplicationIdException(const std::string& at);
};

class NoTokenException: public util::BasicException {
public:
    NoTokenException(const std::string& at);
};

}
