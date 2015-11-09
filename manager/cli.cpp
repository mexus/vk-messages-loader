#include <manager/manager.h>
#include <boost/algorithm/string/predicate.hpp>

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

void PrintActiveFriends(const std::vector<vk_api::FriendsAPI::Friend>& friends, const std::vector<uint64_t>& active) {
    for (uint64_t user_id: active) {
        auto it = std::find_if(friends.begin(), friends.end(), CompareFriends(user_id));
        std::cout << "    Id " << user_id;
        if (it != friends.end()) {
            std::cout << " (" << it->first_name << " " << it->last_name << ")";
        }
        std::cout << "\n";
    }
}

void AddActiveFriend(const std::vector<vk_api::FriendsAPI::Friend>& friends, manager::Manager *manager) {
    std::cout << "Enter a friends's number from the list above (the first one, not `id`): ";
    size_t index;
    std::cin >> index;
    if (index > friends.size() || index == 0) {
        std::cout << "Unknown friend\n";
        return ;
    }
    manager->AddActiveFriend(friends[index - 1].user_id);
}

int main() {
    manager::Manager manager("config.data");
    if (InputYesOrNo("Update friends list?")) {
        manager.LoadFriends();
    }
    auto friends = manager.GetFriends();
    auto active_friends = manager.GetActiveFriends();
    std::cout << "Friends to fetch message history:\n";
    PrintActiveFriends(friends, active_friends);
    if (InputYesOrNo("Print friends list?")) {
        PrintFriends(manager.GetFriends());
        while (InputYesOrNo("Add a friend to fetch message history?")) {
            AddActiveFriend(friends, &manager);
        }
        PrintActiveFriends(friends, manager.GetActiveFriends());
    }
    if (InputYesOrNo("Update messages database?")) {
        manager.LoadMessages();
    }
    if (InputYesOrNo("Export messages?")) {
        manager.ExportHistory();
    }
    return 0;
}
