#include <cli/cli.h>
#include <boost/algorithm/string/predicate.hpp>

namespace cli {

CommandLineInterface::CommandLineInterface(manager::Settings* settings)
        : settings_(settings),
          callbacks_(settings_->GetApplicationId(), settings_->GetStoragePath() + "/token.data"),
          manager_(settings_, &callbacks_) {
}

bool CommandLineInterface::InputYesOrNo(const std::string& question) {
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

void CommandLineInterface::PrintFriends(const std::vector<vk_api::User>& friends) {
    size_t i = 0;
    for (auto& user: friends) {
        std::cout << ++i << ". " << user.user_id << " user: " << user.first_name << " " << user.last_name << "\n";
    }
}

void CommandLineInterface::PrintActiveUsers() const {
    auto& users_cache = manager_.GetUsersCache();
    auto active_users = manager_.GetActiveUsers();
    for (uint64_t user_id: active_users) {
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

void CommandLineInterface::AddActiveUser(const std::vector<vk_api::User>& friends) {
    std::cout << "Enter a friends's number from the list above (the first one, not `id`): ";
    size_t index;
    std::cin >> index;
    if (index > friends.size() || index == 0) {
        std::cout << "Unknown friend\n";
        return ;
    }
    manager_.AddActiveUser(friends[index - 1].user_id);
}

void CommandLineInterface::Execute() {
    if (InputYesOrNo("Update friends list?")) {
        manager_.UpdateFriends();
    }
    auto active_friends = manager_.GetActiveUsers();
    auto friends = manager_.GetFriends();
    std::cout << "Friends to fetch message history:\n";
    PrintActiveUsers();
    if (InputYesOrNo("Print friends list?")) {
        PrintFriends(friends);
        while (InputYesOrNo("Add a friend to fetch message history?")) {
            AddActiveUser(friends);
        }
        PrintActiveUsers();
    }
    if (InputYesOrNo("Update messages database?")) {
        manager_.UpdateMessages();
    }
    if (InputYesOrNo("Export messages?")) {
        manager_.ExportHistory();
    }
}

}
