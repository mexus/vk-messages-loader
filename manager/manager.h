#include <algorithm>
#include <ctime>
#include <iostream>
#include <manager/history-export.h>
#include <manager/users-cache.h>
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

    void UpdateFriends();
    void UpdateMessages();

    std::vector<vk_api::FriendsAPI::Friend> GetFriends() const;
    std::vector<uint64_t> GetActiveUsers() const;
    void AddActiveUser(uint64_t id);
    void ExportHistory();
    const cache::Users& GetUsersCache() const;

private:
    static const std::string kSettingsField;

    const std::string config_file_;
    Settings settings_;
    Token token_;
    vk_api::CommunicationInterface vk_interface_;
    storage::HistoryDB history_db_;
    cache::Users users_cache_;
    cache::FriendsCache friends_cache_;
    uint64_t current_user_id_;
    HistoryExport history_export_;

    void SaveSettings() const;

    static Settings LoadSettings(const std::string& file_name);

};

}
