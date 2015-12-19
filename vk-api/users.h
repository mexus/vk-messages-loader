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
    User GetUser() const;
    User GetUser(uint64_t user_id) const;
    std::vector<User> GetUsers(const std::vector<uint64_t>& ids) const;

private:
    static const std::string kInterfaceName;
    static const uint64_t kUserSelf;
    CommunicationInterface * const vk_interface_;
};

}
