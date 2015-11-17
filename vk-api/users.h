#pragma once

#include "communication-interface.h"

namespace vk_api {

class NoUsersException: public ::util::BasicException {
public:
    NoUsersException(const std::string& at);
};

class UsersAPI {
public:
    UsersAPI(CommunicationInterface* vk_interface);
    User GetUser();

private:
    CommunicationInterface * const vk_interface_;
    static const std::string kInterfaceName;
};

}
