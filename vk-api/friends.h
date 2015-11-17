#pragma once

#include "communication-interface.h"

namespace vk_api {

class FriendsAPI{
public:
    FriendsAPI(CommunicationInterface* vk_interface);

    std::vector<User> GetFriends();

private:
    CommunicationInterface * const vk_interface_;
    static const std::string kInterfaceName;
};

}
