#include <vk-api/users.h>
#include <iostream>
#include <utils/json.h>

namespace util {

template<>
bool JsonToObject<vk_api::UsersAPI::User>(const rapidjson::Value& json, vk_api::UsersAPI::User* object) {
    bool res = JsonGetMembers(json,
                              "id", &object->user_id);
    if (!res) {
        std::cerr << "Unable to convert json value to a 'User' object\n";
    }
    return res;
}

}

namespace vk_api {

const std::string UsersAPI::kInterfaceName = "users";

UsersAPI::UsersAPI(CommunicationInterface* vk_interface)
        : vk_interface_(vk_interface) {
}

UsersAPI::User UsersAPI::GetUser() {
    RequestsManager::Response reply = vk_interface_->SendRequest(kInterfaceName, "get", {});
    if (reply.error.status != Error::OK) {
        std::cerr << "Can't process with a response due to error(s)\n";
        return {};
    }

    std::vector<User> users;
    bool res = util::JsonGetMember(reply.json, "response", &users);
    if (!res) {
        std::cerr << "Unable to convert response to a list of users\n";
        return {};
    }
    if (users.empty()) {
        std::cerr << "No users received\n";
        return {};
    }
    return users.front();
}

}
