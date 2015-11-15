#include <vk-api/users.h>
#include <iostream>
#include <utils/json.h>

namespace util {

template<>
void JsonToObject<vk_api::UsersAPI::User>(const rapidjson::Value& json, vk_api::UsersAPI::User* object) {
    JsonGetMembers(json, "id", &object->user_id);
}

}

namespace vk_api {

NoUsersException::NoUsersException(const std::string& at)
        : util::BasicException(at, "No users received") {
}

const std::string UsersAPI::kInterfaceName = "users";

UsersAPI::UsersAPI(CommunicationInterface* vk_interface)
        : vk_interface_(vk_interface) {
}

UsersAPI::User UsersAPI::GetUser() {
    auto reply = vk_interface_->SendRequest(kInterfaceName, "get", {});

    std::vector<User> users;
    util::JsonGetMember(reply, "response", &users);
    if (users.empty()) {
        THROW_AT(NoUsersException);
    }
    return users.front();
}

}
