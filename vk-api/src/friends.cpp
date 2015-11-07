#include <vk-api/friends.h>
#include <iostream>
#include <utils/json.h>
#include <vk-api/data-types.h>

namespace util {

template<>
bool JsonToObject<vk_api::FriendsAPI::Friend>(const rapidjson::Value& json, vk_api::FriendsAPI::Friend* object) {
    bool res = JsonGetMembers(json,
                              "id", &object->user_id,
                              "first_name", &object->first_name,
                              "last_name", &object->last_name);
    if (!res) {
        std::cerr << "Unable to convert json value to a 'Friend' object\n";
    }
    return res;
}

// This function is needed to avoid a linker error
template<>
bool JsonToObject<vk_api::List<vk_api::FriendsAPI::Friend>>(const rapidjson::Value& json, vk_api::List<vk_api::FriendsAPI::Friend>* object) {
    // This call will be resolved to a template function for vk_api::List<T>,
    // so no loops will be created
    return JsonToObject<vk_api::FriendsAPI::Friend>(json, object);
}

}

namespace vk_api {

const std::string FriendsAPI::kInterfaceName = "friends";

FriendsAPI::FriendsAPI(CommunicationInterface* vk_interface)
        : vk_interface_(vk_interface) {
}

std::vector<FriendsAPI::Friend> FriendsAPI::GetFriends() {
    const cpr::Parameters const_params {
            {"fields", "uid,first_name,last_name"},
            {"order", "hints"}
        };
    std::vector<Friend> total_friends;
    unsigned offset = 0;
    while (true) {
        auto params = const_params;
        params.AddParameter({"offset", std::to_string(offset)});
        RequestsManager::Response reply = vk_interface_->SendRequest(kInterfaceName, "get", std::move(params));
        if (reply.error.status != Error::OK) {
            std::cerr << "Can't process with a response due to error(s)\n";
            return {};
        }

        List<Friend> response;
        bool res = util::JsonGetMember(reply.json, "response", &response);
        if (!res) {
            std::cerr << "Unable to convert response to a list of friends\n";
            break ;
        }
        size_t count = response.items.size();
        std::cout << "Received " << count << " friends out of " << response.count << "\n";
        offset += count;
        total_friends.insert(total_friends.end(),
                             std::make_move_iterator(response.items.begin()),
                             std::make_move_iterator(response.items.end()));
        if (offset >= response.count || count == 0) {
            break;
        }
    }
    return total_friends;
}

}
