#include <vk-api/users.h>
#include <iostream>
#include <utils/json.h>

namespace vk_api {

const std::string UsersAPI::kInterfaceName = "users";
const uint64_t UsersAPI::kUserSelf = 0;

NoUsersException::NoUsersException(const std::string& at)
        : util::BasicException(at, "No users received") {
}

UsersAPI::UsersAPI(CommunicationInterface* vk_interface)
        : vk_interface_(vk_interface) {
}

User UsersAPI::GetUser() const {
    return GetUser(kUserSelf);
}

User UsersAPI::GetUser(uint64_t user_id) const {
    cpr::Parameters params{{"fields", "uid,first_name,last_name"}};
    if (user_id != kUserSelf) {
        params.AddParameter({"user_ids", std::to_string(user_id)});
    }
    auto reply = vk_interface_->SendRequest(kInterfaceName, "get", params);

    std::vector<User> users;
    util::JsonGetMember(reply, "response", &users);
    if (users.empty()) {
        THROW_AT(NoUsersException);
    }
    return users.front();
}

std::vector<User> UsersAPI::GetUsers(const std::vector<uint64_t>& ids) const {
    static const size_t kMaxUsers = 1000;
    std::vector<User> result;
    result.reserve(ids.size());
    for (size_t part = 0, parts = ids.size() / kMaxUsers; part <= parts; ++part) {
        const size_t start = kMaxUsers * part;
        const size_t end = std::min(kMaxUsers * (part + 1), ids.size());
        std::string ids_string;
        for (size_t i = start; i != end; ++i) {
            uint64_t id = ids[i];
            if (i != start) {
                ids_string += ",";
            }
            ids_string += std::to_string(id);
        }

        cpr::Parameters params{{"fields", "uid,first_name,last_name"},
                               {"user_ids", ids_string}};
        auto reply = vk_interface_->SendRequest(kInterfaceName, "get", params);

        std::vector<User> users;
        util::JsonGetMember(reply, "response", &users);
        result.insert(result.end(),
                      std::make_move_iterator(users.begin()),
                      std::make_move_iterator(users.end()));
    }
    return result;
}

}
