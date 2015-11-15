#include <manager/manager.h>
#include <boost/algorithm/string/predicate.hpp>
#include "cli-exceptions-handler.h"

CliExceptionsHandler exceptions_handler;

std::shared_ptr<manager::Manager> GetManager() {
    static auto pointer = std::make_shared<manager::Manager>("config.data");
    return pointer;
}

bool InputYesOrNo(const std::string& question) {
    static const std::string yes("yes"), no("no");
    std::cout << question << " [yes/no]: ";
    while (true) {
        std::string input;
        std::cin >> input;
        if (boost::iequals(input, yes)) {
            return true;
        } else if (boost::iequals(input, no) || input.empty()) {
            return false;
        }
        std::cout << "Please input 'yes' or 'no': ";
    }
}

void PrintFriends(const std::vector<vk_api::FriendsAPI::Friend>& friends) {
    size_t i = 0;
    for (auto& user: friends) {
        std::cout << ++i << ". " << user.user_id << " user: " << user.first_name << " " << user.last_name << "\n";
    }
}

struct CompareFriends {
    CompareFriends(uint64_t id) : id(id) {}
    bool operator()(const vk_api::FriendsAPI::Friend& user) const {
        return user.user_id == id;
    }
    uint64_t id;
};

void PrintActiveUsers(const std::vector<uint64_t>& active) {
    auto& users_cache = GetManager()->GetUsersCache();
    for (uint64_t user_id: active) {
        std::cout << "    Id " << user_id;
        try {
            auto user = users_cache.GetData(user_id);
            std::cout << " (" << user.first_name << " " << user.last_name << ")";
        } catch (const manager::cache::NoDataException&) {
            // No user, no name conversion, fair enough
        }
        std::cout << "\n";
    }
}

void AddActiveUser(const std::vector<vk_api::FriendsAPI::Friend>& friends) {
    std::cout << "Enter a friends's number from the list above (the first one, not `id`): ";
    size_t index;
    std::cin >> index;
    if (index > friends.size() || index == 0) {
        std::cout << "Unknown friend\n";
        return ;
    }
    GetManager()->AddActiveUser(friends[index - 1].user_id);
}

void Flow() {
    auto manager_ = GetManager();
    if (InputYesOrNo("Update friends list?")) {
        manager_->UpdateFriends();
    }
    auto active_friends = manager_->GetActiveUsers();
    auto friends = manager_->GetFriends();
    std::cout << "Friends to fetch message history:\n";
    PrintActiveUsers(active_friends);
    if (InputYesOrNo("Print friends list?")) {
        PrintFriends(friends);
        while (InputYesOrNo("Add a friend to fetch message history?")) {
            AddActiveUser(friends);
        }
        PrintActiveUsers(active_friends);
    }
    if (InputYesOrNo("Update messages database?")) {
        manager_->UpdateMessages();
    }
    if (InputYesOrNo("Export messages?")) {
        manager_->ExportHistory();
    }
}

int main() {
    exceptions_handler.ProcessFunction(Flow);
    return 0;
}
