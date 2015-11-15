#pragma once

#include "cache.h"

namespace manager {
namespace cache {

struct User {
    uint64_t id;
    std::string first_name;
    std::string last_name;
};

typedef MappedCache<User> Users;
template<> uint64_t GetIdentificator<User>(const User& data);

}
}
