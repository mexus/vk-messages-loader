#pragma once

#include "communication-interface.h"

namespace vk_api {

class FriendsAPI{
public:
    struct Friend {
        uint64_t user_id;
        std::string first_name;
        std::string last_name;
    };

    FriendsAPI(CommunicationInterface* vk_interface);

    std::vector<Friend> GetFriends();

private:
    CommunicationInterface * const vk_interface_;
    static const std::string kInterfaceName;
};

}
