#pragma once

#include "communication-interface.h"

namespace vk_api {

class NoUsersException: public ::util::BasicException {
public:
    NoUsersException();
};

class UsersAPI {
public:
    struct User {
        uint64_t user_id;
    };

    UsersAPI(CommunicationInterface* vk_interface);
    User GetUser();

private:
    CommunicationInterface * const vk_interface_;
    static const std::string kInterfaceName;
};

}
