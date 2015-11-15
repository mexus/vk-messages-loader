#pragma once
#include <vk-api/friends.h>
#include <manager/manager.h>

namespace cli {

class CommandLineInterface {
public:
    CommandLineInterface();

    void Execute();

private:
    manager::Manager manager_;

    void PrintActiveUsers() const;
    void AddActiveUser(const std::vector<vk_api::FriendsAPI::Friend>& friends);

    static bool InputYesOrNo(const std::string& question);
    static void PrintFriends(const std::vector<vk_api::FriendsAPI::Friend>& friends);
};

}
