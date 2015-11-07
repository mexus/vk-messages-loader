#include <manager/manager.h>
#include <vk-api/users.h>

namespace {
struct FriendsIdCompare {
    FriendsIdCompare(uint64_t id) : id(id) {}
    bool operator()(const vk_api::FriendsAPI::Friend& lhs) const {
        return lhs.user_id == id;
    }
    uint64_t id;
};
}

namespace manager {

const std::string Manager::kSettingsField = "settings";

Manager::Manager(const std::string& config_file)
        : config_file_(config_file),
          settings_(LoadSettings(config_file_)),
          token_(settings_.application_id, settings_.storage_path + "/token.data"),
          vk_interface_(std::bind(&Token::GetToken, token_)),
          history_db_(settings_.storage_path + "/messages"),
          friends_cache_(settings_.storage_path + "/friends.data") {
    friends_cache_.Load();
    vk_api::UsersAPI users_api(&vk_interface_);
    auto user = users_api.GetUser();
    current_user_id_ = user.user_id;
}

Manager::~Manager() {
    SaveSettings();
}

Settings Manager::LoadSettings( const std::string& file_name) {
    Settings settings;
    auto doc = util::JsonFromFile(file_name);
    if (!doc.IsObject()) {
        std::cerr << "Json document is not an object\n";
        return {};
    }
    if (!util::JsonGetMember(doc, kSettingsField, &settings)) {
        std::cerr << "Can't extract token data from a json\n";
        return {};
    }
    return settings;
}

void Manager::SaveSettings() const {
    rapidjson::Document doc;
    doc.SetObject();
    auto& allocator = doc.GetAllocator();
    util::JsonAddMember(&doc, kSettingsField, settings_, allocator);
    if (!util::JsonToFile(config_file_, doc)) {
        std::cerr << "Unable to save token data\n";
    }
}

void Manager::LoadMessages() {
    vk_api::MessageAPI messages_api(&vk_interface_);
    for (auto& user_id: settings_.users) {
        auto history = history_db_.GetUser(user_id);
        uint64_t last_message_id = history->LastMessageId();
        auto messages = messages_api.GetMessages(user_id, last_message_id);
        if (messages.empty()) {
            break ;
        }
        for (auto& vk_message: messages) {
            storage::Message storage_message {
                    vk_message.id,
                    vk_message.date,
                    vk_message.from_id,
                    (vk_message.user_id == vk_message.from_id) ? current_user_id_ : vk_message.user_id,
                    std::move(vk_message.body),
                    {}
            };
            history->AddMessage(std::move(storage_message));
        }
    }
    return ;
}

void Manager::LoadFriends() {
    friends_cache_.Load();
    auto& cached_friends = friends_cache_.GetFriends();
    vk_api::FriendsAPI friends_api(&vk_interface_);
    auto friends = friends_api.GetFriends();
    if (friends.empty()) {
        return ;
    }
    for (auto& user: friends) {
        auto cached_it = std::find_if(cached_friends.begin(), cached_friends.end(), FriendsIdCompare(user.user_id));
        if (cached_it == cached_friends.end()) {
            cached_friends.push_back(std::move(user));
        } else {
            *cached_it = std::move(user);
        }
    }
    friends_cache_.Save();
}

}
