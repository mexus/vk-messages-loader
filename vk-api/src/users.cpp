#include <vk-api/users.h>
#include <iostream>
#include <utils/json.h>

namespace vk_api {

NoUsersException::NoUsersException(const std::string& at)
        : util::BasicException(at, "No users received") {
}

const std::string UsersAPI::kInterfaceName = "users";

UsersAPI::UsersAPI(CommunicationInterface* vk_interface)
        : vk_interface_(vk_interface) {
}

FriendsAPI::Friend UsersAPI::GetUser() {
    auto reply = vk_interface_->SendRequest(kInterfaceName, "get", {{"fields", "uid,first_name,last_name"}});

    std::vector<FriendsAPI::Friend> users;
    util::JsonGetMember(reply, "response", &users);
    if (users.empty()) {
        THROW_AT(NoUsersException);
    }
    return users.front();
}

}
