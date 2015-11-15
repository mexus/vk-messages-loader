#pragma once

#include "friends.h"

namespace vk_api {

class NoUsersException: public ::util::BasicException {
public:
    NoUsersException(const std::string& at);
};

class UsersAPI {
public:
    UsersAPI(CommunicationInterface* vk_interface);
    FriendsAPI::Friend GetUser();

private:
    CommunicationInterface * const vk_interface_;
    static const std::string kInterfaceName;
};

}
