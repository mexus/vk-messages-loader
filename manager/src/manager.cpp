#include <manager/manager.h>
#include <utils/exceptions.h>
#include <vk-api/users.h>
#include <algorithm>
#include <boost/filesystem.hpp>

namespace manager {

Manager::Manager(const std::shared_ptr<Settings>& settings,
                 vk_api::Callbacks* callbacks)
    : settings_(settings),
      vk_interface_(callbacks),
      history_db_(settings_->GetStoragePath() + "/messages"),
      users_cache_(settings_->GetStoragePath() + "/users.cache"),
      dialogues_users_cache_(settings_->GetStoragePath() +
                             "/dialogues-users.cache"),
      history_export_(users_cache_) {
  try {
    users_cache_.Load();
  } catch (const util::FileReadException& e) {
    LOG(WARNING) << "Can't load users cache from file " << e.GetFileName();
  }
  try {
    dialogues_users_cache_.Load();
  } catch (const util::FileReadException& e) {
    LOG(WARNING) << "Can't load dialogues users cache from file "
                 << e.GetFileName();
  }
  vk_api::UsersAPI users_api(&vk_interface_);
  auto user = users_api.GetUser();
  users_cache_.AddData({user.user_id, user.first_name, user.last_name});
  current_user_id_ = user.user_id;
}

Manager::~Manager() {}

void Manager::UpdateMessages() {
  vk_api::MessageAPI messages_api(&vk_interface_);
  auto users = settings_->GetUsers();
  for (auto& user_id : users) {
    auto history = history_db_.GetUser(user_id);
    uint64_t last_message_id = history->LastMessageId();
    auto messages = messages_api.GetMessages(user_id, last_message_id);
    if (messages.empty()) {
      continue;
    }
    for (auto& vk_message : messages) {
      storage::Message storage_message{vk_message.id,
                                       vk_message.date,
                                       vk_message.from_id,
                                       std::move(vk_message.body),
                                       {}};
      for (auto& attachment : vk_message.attachments) {
        AddAttachment(attachment, &storage_message);
      }
      history->AddMessage(std::move(storage_message));
    }
  }
  return;
}

void Manager::UpdateDialoguesList() {
  vk_api::MessageAPI messages_api(&vk_interface_);
  vk_api::UsersAPI users_api(&vk_interface_);
  auto last_messages = messages_api.GetLastMessages();
  std::vector<uint64_t> user_ids;
  for (auto& msg : last_messages) {
    user_ids.push_back(msg.user_id);
  }
  auto users = users_api.GetUsers(user_ids);
  for (auto& user : users) {
    users_cache_.AddData(user);
    dialogues_users_cache_.AddData(user);
  }
  users_cache_.Save();
  dialogues_users_cache_.Save();
}

std::vector<vk_api::User> Manager::GetDialoguesUsers() const {
  return dialogues_users_cache_.GetDataAsVector();
}

std::vector<uint64_t> Manager::GetActiveUsers() const {
  return settings_->GetUsers();
}

void Manager::AddActiveUser(uint64_t user_id) {
  auto& users = settings_->GetUsers();
  auto it =
      std::find_if(users.begin(), users.end(),
                   [user_id](const uint64_t& user) { return user_id == user; });
  if (it == users.end()) {
    users.push_back(user_id);
  }
}

std::vector<uint64_t> Manager::GetActiveChats() const {
  return settings_->GetChats();
}

void Manager::AddActiveChat(uint64_t chat_id) {
  auto& chats = settings_->GetChats();
  auto it =
      std::find_if(chats.begin(), chats.end(),
                   [chat_id](const uint64_t& chat) { return chat_id == chat; });
  if (it == chats.end()) {
    chats.push_back(chat_id);
  }
}

void Manager::ExportHistory(
    const std::string& path,
    std::function<std::shared_ptr<storage::History>(uint64_t)> f,
    const std::vector<uint64_t>& ids, const std::string& prefix) const {
  for (auto& id : ids) {
    std::string path_for_id = path + "/" + prefix + std::to_string(id);
    auto history = f(id);
    history_export_.ExportToFile(history, path_for_id);
  }
}

void Manager::ExportHistory() {
  auto path =
      boost::filesystem::path(settings_->GetStoragePath()) / "exported/";
  if (!boost::filesystem::exists(path)) {
    boost::filesystem::create_directory(path);
  }
  auto users = settings_->GetUsers();
  auto str_path = path.string();
  ExportHistory(str_path, std::bind(&storage::HistoryDB::GetUser, &history_db_,
                                    std::placeholders::_1),
                users, "user-");
  auto chats = settings_->GetChats();
  ExportHistory(str_path, std::bind(&storage::HistoryDB::GetChat, &history_db_,
                                    std::placeholders::_1),
                chats, "chat-");
}

const cache::Users& Manager::GetUsersCache() const { return users_cache_; }

void Manager::AddAttachment(
    const std::unique_ptr<vk_api::Attachment>& attachment,
    storage::Message* storage_message) {
  switch (attachment->GetKind()) {
    case vk_api::PHOTO:
      AddAttachment(
          static_cast<const vk_api::PhotoAttachment*>(attachment.get()),
          storage_message);
      break;
    case vk_api::VIDEO:
      AddAttachment(
          static_cast<const vk_api::VideoAttachment*>(attachment.get()),
          storage_message);
      break;
    case vk_api::STICKER:
      AddAttachment(
          static_cast<const vk_api::StickerAttachment*>(attachment.get()),
          storage_message);
      break;
  }
}

void Manager::AddAttachment(const vk_api::PhotoAttachment* photo,
                            storage::Message* storage_message) {
  storage::Attachment storage_attachment{storage::PHOTO, photo->url};
  storage_message->attachments.push_back(std::move(storage_attachment));
}

void Manager::AddAttachment(const vk_api::VideoAttachment* video,
                            storage::Message* storage_message) {
  std::string body;
  if (!video->title.empty()) {
    body = "`" + video->title + "`";
  }
  if (!video->description.empty()) {
    if (!body.empty()) body += ", ";
    body += "`" + video->description + "`";
  }
  if (!body.empty()) body += ": ";
  body += video->preview_url;
  storage::Attachment storage_attachment{storage::VIDEO, std::move(body)};
  storage_message->attachments.push_back(std::move(storage_attachment));
}

void Manager::AddAttachment(const vk_api::StickerAttachment* sticker,
                            storage::Message* storage_message) {
  storage::Attachment storage_attachment{storage::STICKER, sticker->url};
  storage_message->attachments.push_back(std::move(storage_attachment));
}
}
