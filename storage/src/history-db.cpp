#include <storage/exceptions.h>
#include <storage/history-db.h>
#include <boost/filesystem.hpp>
#include <fstream>
#include <iostream>

namespace storage {

HistoryDB::HistoryDB(const std::string& path) : path_(path) {}

std::shared_ptr<History> HistoryDB::GetUser(uint64_t user_id) {
  auto it = users_history_.find(user_id);
  if (it != users_history_.end()) {
    auto pointer = it->second.lock();
    if (pointer) {
      return pointer;
    }
  }
  auto storage = GetStorage(user_id);
  users_history_[user_id] = storage;
  return storage;
}

std::shared_ptr<History> HistoryDB::GetChat(uint64_t chat_id) {
  auto it = chats_history_.find(chat_id);
  if (it != chats_history_.end()) {
    auto pointer = it->second.lock();
    if (pointer) {
      return pointer;
    }
  }
  auto storage = GetStorage(chat_id, "chat-");
  chats_history_[chat_id] = storage;
  return storage;
}

std::shared_ptr<History> HistoryDB::GetStorage(uint64_t user_id, const std::string& prefix) {
  CheckPath();
  boost::filesystem::path path(path_);
  path /= prefix + std::to_string(user_id);
  return std::make_shared<History>(path.string<std::string>());
}

void HistoryDB::CheckPath() const {
  boost::filesystem::path path(path_);
  if (boost::filesystem::exists(path)) {
    if (!boost::filesystem::is_directory(path)) {
      THROW_AT(PathIsFileException, path.string());
    }
  } else {
    boost::filesystem::create_directories(path);
    if (!boost::filesystem::exists(path)) {
      THROW_AT(PathCreateException, path.string());
    }
  }
}
}
