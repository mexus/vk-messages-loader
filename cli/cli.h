#pragma once
#include <vk-api/friends.h>
#include <manager/manager.h>
#include "callbacks.h"

namespace cli {

class CommandLineInterface {
public:
    CommandLineInterface(manager::Settings* settings);

    void Execute();

private:
    manager::Settings* settings_;
    Callbacks callbacks_;
    manager::Manager manager_;

    void PrintActiveUsers() const;
    void AddActiveUser(const std::vector<vk_api::FriendsAPI::Friend>& friends);

    static bool InputYesOrNo(const std::string& question);
    static void PrintFriends(const std::vector<vk_api::FriendsAPI::Friend>& friends);
};

}
