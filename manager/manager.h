#include <manager/history-export.h>
#include <manager/settings.h>
#include <manager/users-cache.h>
#include <storage/history-db.h>
#include <vk-api/callbacks.h>
#include <vk-api/messages.h>
#include <algorithm>
#include <ctime>
#include <iostream>

namespace manager {

class Manager {
 public:
  Manager(const std::shared_ptr<Settings>& settings,
          vk_api::Callbacks* callbacks);
  ~Manager();

  void UpdateDialoguesList();
  void UpdateMessages();

  std::vector<vk_api::User> GetDialoguesUsers() const;

  std::vector<uint64_t> GetActiveUsers() const;
  void AddActiveUser(uint64_t id);

  std::vector<uint64_t> GetActiveChats() const;
  void AddActiveChat(uint64_t id);

  void ExportHistory();
  const cache::Users& GetUsersCache() const;

 private:
  const std::shared_ptr<Settings> settings_;
  vk_api::CommunicationInterface vk_interface_;
  storage::HistoryDB history_db_;
  cache::Users users_cache_, dialogues_users_cache_;
  uint64_t current_user_id_;
  HistoryExport history_export_;

  void ExportHistory(
      const std::string& path,
      std::function<std::shared_ptr<storage::History>(uint64_t)> f,
      const std::vector<uint64_t>& ids, const std::string& prefix) const;

  static void AddAttachment(
      const std::unique_ptr<vk_api::Attachment>& attachment,
      storage::Message* storage_message);
  static void AddAttachment(const vk_api::PhotoAttachment* attachment,
                            storage::Message* storage_message);
  static void AddAttachment(const vk_api::VideoAttachment* attachment,
                            storage::Message* storage_message);
  static void AddAttachment(const vk_api::StickerAttachment* attachment,
                            storage::Message* storage_message);
};
}
