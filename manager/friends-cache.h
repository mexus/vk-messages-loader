#pragma once
#include <vk-api/friends.h>

namespace manager {

class FriendsCache {
public:
    FriendsCache(const std::string& file_name);

    void Load();
    void Save() const;

    std::vector<vk_api::FriendsAPI::Friend>& GetFriends();
    const std::vector<vk_api::FriendsAPI::Friend>& GetFriends() const;

private:
    static const std::string kJsonField;
    const std::string file_name_;
    std::vector<vk_api::FriendsAPI::Friend> friends_;

};

}
