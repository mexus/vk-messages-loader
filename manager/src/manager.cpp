#include <manager/manager.h>
#include <algorithm>
#include <boost/filesystem.hpp>
#include <vk-api/users.h>
#include <utils/exceptions.h>

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
    try {
        friends_cache_.Load();
    } catch (const util::FileReadException& e) {
        std::cout << "Can't load friends cache from file " << e.GetFileName() << "\n";
    }
    vk_api::UsersAPI users_api(&vk_interface_);
    auto user = users_api.GetUser();
    current_user_id_ = user.user_id;
}

Manager::~Manager() {
    try {
        SaveSettings();
    } catch (const std::runtime_error& e) {
        std::cerr << "Caught a runtime error while saving the settings: " << e.what() << "\n";
    }
}

Settings Manager::LoadSettings( const std::string& file_name) {
    Settings settings;
    try {
        auto doc = util::JsonFromFile(file_name);
        util::JsonGetMember(doc, kSettingsField, &settings);
    } catch (const util::FileReadException& e) {
        std::cout << "Can't load settings from file " << e.GetFileName() << "\n";
    }
    return settings;
}

void Manager::SaveSettings() const {
    rapidjson::Document doc;
    doc.SetObject();
    auto& allocator = doc.GetAllocator();
    util::JsonAddMember(&doc, kSettingsField, settings_, allocator);
    util::JsonToFile(config_file_, doc);
}

void Manager::LoadMessages() {
    vk_api::MessageAPI messages_api(&vk_interface_);
    for (auto& user_id: settings_.users) {
        auto history = history_db_.GetUser(user_id);
        uint64_t last_message_id = history->LastMessageId();
        auto messages = messages_api.GetMessages(user_id, last_message_id);
        if (messages.empty()) {
            continue ;
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
    try {
        friends_cache_.Load();
    } catch (const util::FileReadException& e) {
        std::cout << "Can't load friends cache from file " << e.GetFileName() << "\n";
    }
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

std::vector<vk_api::FriendsAPI::Friend> Manager::GetFriends() const {
    return friends_cache_.GetFriends();
}

std::vector<uint64_t> Manager::GetActiveFriends() const {
    return settings_.users;
}

struct UsersIdComparison {
    UsersIdComparison(uint64_t id) : id(id) {}
    bool operator()(const uint64_t& user) const {
        return user == id;
    }
    uint64_t id;
};

void Manager::AddActiveFriend(uint64_t user_id) {
    auto it = std::find_if(settings_.users.begin(), settings_.users.end(), UsersIdComparison(user_id));
    if (it == settings_.users.end()) {
        settings_.users.push_back(user_id);
    }
}

void Manager::ExportHistory() {
    auto path = boost::filesystem::path(settings_.storage_path) / "exported/";
    if (!boost::filesystem::exists(path)) {
        boost::filesystem::create_directory(path);
    }
    auto& friends = friends_cache_.GetFriends();
    for (auto& user_id: settings_.users) {
        std::string user_path = (path / std::to_string(user_id)).string();
        auto user_history = history_db_.GetUser(user_id);
        if (!user_history) {
            continue ;
        }
        export_.ExportToFile(friends, user_history, user_path);
    }
}

}
