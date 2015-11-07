#include <manager/friends-cache.h>
#include <utils/json.h>

namespace manager {

const std::string FriendsCache::kJsonField = "friends";

FriendsCache::FriendsCache(const std::string& file_name) : file_name_(file_name) {
}

void FriendsCache::Load() {
    auto doc = util::JsonFromFile(file_name_);
    if (!doc.IsObject()) {
        std::cerr << "Json document is not an object\n";
        return ;
    }
    friends_.clear();
    if (!util::JsonGetMember(doc, kJsonField, &friends_)) {
        std::cerr << "Can't load friends\n";
    }
}

void FriendsCache::Save() const {
    rapidjson::Document doc;
    auto& allocator = doc.GetAllocator();
    doc.SetObject();
    util::JsonAddMember(&doc, kJsonField, friends_, allocator);
    if (!util::JsonToFile(file_name_, doc)) {
        std::cerr << "Can't write friends to a file\n";
    }
}

std::vector<vk_api::FriendsAPI::Friend>& FriendsCache::GetFriends() {
    return friends_;
}

}
