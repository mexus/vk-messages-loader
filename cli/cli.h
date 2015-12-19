#pragma once
#include <vk-api/friends.h>
#include <manager/manager.h>
#include "callbacks.h"

namespace cli {

class CommandLineInterface {
public:
    CommandLineInterface(const std::shared_ptr<manager::Settings>& settings);

    void Execute();

private:
    const std::shared_ptr<manager::Settings> settings_;
    Callbacks callbacks_;
    manager::Manager manager_;

    void PrintActiveUsers() const;
    void AddActiveUser(const std::vector<vk_api::User>& friends);

    static bool InputYesOrNo(const std::string& question);
    static void PrintUsers(const std::vector<vk_api::User>& users);
};

}
