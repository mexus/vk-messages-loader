#include <manager/friends-cache.h>
#include <utils/json.h>

namespace manager {

const std::string FriendsCache::kJsonField = "friends";

FriendsCache::FriendsCache(const std::string& file_name) : file_name_(file_name) {
}

void FriendsCache::Load() {
    auto doc = util::JsonFromFile(file_name_);
    if (!doc.IsObject()) {
        throw util::json::NotAnObjectException();
    }
    friends_.clear();
    util::JsonGetMember(doc, kJsonField, &friends_);
}

void FriendsCache::Save() const {
    rapidjson::Document doc;
    auto& allocator = doc.GetAllocator();
    doc.SetObject();
    util::JsonAddMember(&doc, kJsonField, friends_, allocator);
    util::JsonToFile(file_name_, doc);
}

std::vector<vk_api::FriendsAPI::Friend>& FriendsCache::GetFriends() {
    return friends_;
}

const std::vector<vk_api::FriendsAPI::Friend>& FriendsCache::GetFriends() const {
    return friends_;
}

}
