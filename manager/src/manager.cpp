#include <manager/manager.h>
#include <algorithm>
#include <boost/filesystem.hpp>
#include <vk-api/users.h>
#include <vk-api/friends.h>
#include <utils/exceptions.h>

namespace manager {

const std::string Manager::kSettingsField = "settings";

Manager::Manager(const std::string& config_file)
        : config_file_(config_file),
          settings_(LoadSettings(config_file)),
          token_(settings_.application_id, settings_.storage_path + "/token.data"),
          vk_interface_(std::bind(&Token::GetToken, token_)),
          history_db_(settings_.storage_path + "/messages"),
          users_cache_(settings_.storage_path + "/users.cache"),
          friends_cache_(settings_.storage_path + "/friends.cache"),
          history_export_(users_cache_) {
    try {
        users_cache_.Load();
    } catch (const util::FileReadException& e) {
        std::cout << "Can't load users cache from file " << e.GetFileName() << "\n";
    }
    try {
        friends_cache_.Load();
    } catch (const util::FileReadException& e) {
        std::cout << "Can't load friends cache from file " << e.GetFileName() << "\n";
    }
    vk_api::UsersAPI users_api(&vk_interface_);
    auto user = users_api.GetUser();
    users_cache_.AddData({user.user_id, user.first_name, user.last_name});
    current_user_id_ = user.user_id;
}

Manager::~Manager() {
    try {
        SaveSettings();
    } catch (const util::BasicException& e) {
        std::cerr << "Caught an application exception at `" << e.GetAt() << "` "
                  << "while saving settings: " << e.GetMessage() << "\n";
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

void Manager::UpdateMessages() {
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

void Manager::UpdateFriends() {
    vk_api::FriendsAPI friends_api(&vk_interface_);
    auto friends = friends_api.GetFriends();
    if (friends.empty()) {
        return ;
    }
    for (auto& user: friends) {
        users_cache_.AddData(cache::User{user.user_id, user.first_name, user.last_name});
        friends_cache_.AddData(std::move(user));
    }
    users_cache_.Save();
    friends_cache_.Save();
}

std::vector<vk_api::FriendsAPI::Friend> Manager::GetFriends() const {
    return friends_cache_.GetDataAsVector();
}

std::vector<uint64_t> Manager::GetActiveUsers() const {
    return settings_.users;
}

void Manager::AddActiveUser(uint64_t user_id) {
    auto it = std::find_if(settings_.users.begin(),
                           settings_.users.end(),
                           [user_id](const uint64_t& user){return user_id == user;});
    if (it == settings_.users.end()) {
        settings_.users.push_back(user_id);
    }
}

void Manager::ExportHistory() {
    auto path = boost::filesystem::path(settings_.storage_path) / "exported/";
    if (!boost::filesystem::exists(path)) {
        boost::filesystem::create_directory(path);
    }
    for (auto& user_id: settings_.users) {
        std::string user_path = (path / std::to_string(user_id)).string();
        auto user_history = history_db_.GetUser(user_id);
        history_export_.ExportToFile(user_history, user_path);
    }
}

const cache::Users& Manager::GetUsersCache() const {
    return users_cache_;
}

}
