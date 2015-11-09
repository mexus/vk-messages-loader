#include <algorithm>
#include <ctime>
#include <iostream>
#include <manager/export.h>
#include <manager/friends-cache.h>
#include <manager/settings.h>
#include <manager/token.h>
#include <storage/history-db.h>
#include <vk-api/communication-interface.h>
#include <vk-api/messages.h>

namespace manager {

class Manager {
public:
    Manager(const std::string& config_file);
    ~Manager();

    void LoadFriends();
    void LoadMessages();

    std::vector<vk_api::FriendsAPI::Friend> GetFriends() const;
    std::vector<uint64_t> GetActiveFriends() const;

    void AddActiveFriend(uint64_t id);
    void ExportHistory();

private:
    static const std::string kSettingsField;

    const std::string config_file_;
    Settings settings_;
    Token token_;
    vk_api::CommunicationInterface vk_interface_;
    storage::HistoryDB history_db_;
    FriendsCache friends_cache_;
    uint64_t current_user_id_;
    Export export_;

    void SaveSettings() const;

    static Settings LoadSettings(const std::string& file_name);

};

}
