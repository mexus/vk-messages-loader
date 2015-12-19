#include <vk-api/friends.h>
#include <iostream>
#include <utils/json.h>
#include <vk-api/data-types.h>

namespace vk_api {

const std::string FriendsAPI::kInterfaceName = "friends";

FriendsAPI::FriendsAPI(CommunicationInterface* vk_interface)
        : vk_interface_(vk_interface) {
}

std::vector<User> FriendsAPI::GetFriends() {
    const cpr::Parameters const_params {
            {"fields", "uid,first_name,last_name"},
            {"order", "hints"}
        };
    std::vector<User> total_friends;
    unsigned offset = 0;
    while (true) {
        auto params = const_params;
        params.AddParameter({"offset", std::to_string(offset)});
        auto doc = vk_interface_->SendRequest(kInterfaceName, "get", std::move(params));

        List<User> response;
        try {
            util::JsonGetMember(doc, "response", &response);
        } catch (const util::json::Exception& e) {
            LOG(ERROR) << "Unable to convert response to a list of friends: " << e.what();
            break ;
        }
        size_t count = response.items.size();
        LOG(DEBUG) << "Received " << count << " friends out of " << response.count;
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
