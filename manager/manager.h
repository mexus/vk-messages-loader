#include <algorithm>
#include <ctime>
#include <iostream>
#include <vk-api/communication-interface.h>
#include <vk-api/messages.h>
#include <manager/friends-cache.h>
#include <manager/settings.h>
#include <manager/token.h>
#include <storage/history-db.h>

namespace manager {

class Manager {
public:
    Manager(const std::string& config_file);
    ~Manager();

    void LoadFriends();
    void LoadMessages();

private:
    static const std::string kSettingsField;

    const std::string config_file_;
    Settings settings_;
    Token token_;
    vk_api::CommunicationInterface vk_interface_;
    storage::HistoryDB history_db_;
    FriendsCache friends_cache_;

    void SaveSettings() const;

    static Settings LoadSettings(const std::string& file_name);

};

}
