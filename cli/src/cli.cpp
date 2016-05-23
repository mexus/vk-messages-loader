#include <cli/cli.h>
#include <boost/algorithm/string/predicate.hpp>

namespace cli {

CommandLineInterface::CommandLineInterface(
    const std::shared_ptr<manager::Settings>& settings)
    : settings_(settings),
      callbacks_(settings_->GetApplicationId(),
                 settings_->GetStoragePath() + "/token.data"),
      manager_(settings_, &callbacks_) {}

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

void CommandLineInterface::PrintUsers(const std::vector<vk_api::User>& users) {
  size_t i = 0;
  for (auto& user : users) {
    std::cout << ++i << ". " << user.user_id << " user: " << user.first_name
              << " " << user.last_name << "\n";
  }
}

void CommandLineInterface::PrintChats(const std::vector<vk_api::Chat>& chats) {
  size_t i = 0;
  for (auto& chat : chats) {
    std::cout << ++i << ". " << chat.title << "\n";
  }
}

void CommandLineInterface::PrintActiveUsers() const {
  auto& users_cache = manager_.GetUsersCache();
  auto active_users = manager_.GetActiveUsers();
  for (uint64_t user_id : active_users) {
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

void CommandLineInterface::PrintActiveChats() const {
  auto chats_cache = manager_.GetChatsCache();
  auto active_chats = manager_.GetActiveChats();
  for (uint64_t chat_id : active_chats) {
    std::cout << "    Id " << chat_id;
    try {
      auto chat = chats_cache.GetData(chat_id);
      std::cout << " `" << chat.title << "`";
    } catch (const manager::cache::NoDataException&) {
      // No chat, no title, fair enough
    }
    std::cout << "\n";
  }
}

void CommandLineInterface::AddActiveUser(
    const std::vector<vk_api::User>& friends) {
  std::cout << "Enter a friends's number from the list above (the first one, "
               "not `id`): ";
  size_t index;
  std::cin >> index;
  if (index > friends.size() || index == 0) {
    std::cout << "Unknown friend\n";
    return;
  }
  manager_.AddActiveUser(friends[index - 1].user_id);
}

void CommandLineInterface::AddActiveChat(
    const std::vector<vk_api::Chat>& chats) {
  std::cout << "Enter a chat's number from the list above (the first one, not "
               "`id`): ";
  size_t index;
  std::cin >> index;
  if (index > chats.size() || index == 0) {
    std::cout << "Unknown chat\n";
    return;
  }
  manager_.AddActiveChat(chats[index - 1].chat_id);
}

void CommandLineInterface::Execute() {
  if (InputYesOrNo("Update dialogues list?")) {
    manager_.UpdateDialoguesList();
  }
  auto active_friends = manager_.GetActiveUsers();
  auto dialogues_users = manager_.GetDialoguesUsers();

  auto active_chats = manager_.GetActiveChats();
  auto all_chats = manager_.GetChatsCache().GetDataAsVector();

  std::cout << "Friends to fetch message history:\n";
  PrintActiveUsers();
  if (InputYesOrNo("Print dialogues list?")) {
    PrintUsers(dialogues_users);
    while (InputYesOrNo("Add a user to fetch message history?")) {
      AddActiveUser(dialogues_users);
    }
    PrintActiveUsers();
  }

  std::cout << "Chats to fetch message history:\n";
  PrintActiveChats();
  if (InputYesOrNo("Print chat list?")) {
    PrintChats(all_chats);
    while (InputYesOrNo("Add a chat to fetch message history?")) {
      AddActiveChat(all_chats);
    }
    PrintActiveChats();
  }

  if (InputYesOrNo("Update messages database?")) {
    manager_.UpdateMessages();
  }
  if (InputYesOrNo("Export messages?")) {
    manager_.ExportHistory();
  }
}
}
