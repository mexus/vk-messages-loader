#pragma once
#include <manager/manager.h>
#include <vk-api/friends.h>
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
  void PrintActiveChats() const;
  void AddActiveUser(const std::vector<vk_api::User>& friends);
  void AddActiveChat(const std::vector<vk_api::Chat>& chats);

  static bool InputYesOrNo(const std::string& question);
  static void PrintUsers(const std::vector<vk_api::User>& users);
  static void PrintChats(const std::vector<vk_api::Chat>& chats);
};
}
