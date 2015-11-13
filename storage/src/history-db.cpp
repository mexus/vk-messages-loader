#include <storage/history-db.h>
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <storage/exceptions.h>

namespace storage {

HistoryDB::HistoryDB(const std::string& path) : path_(path) {
}

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

std::shared_ptr<History> HistoryDB::GetStorage(uint64_t user_id) {
    CheckPath();
    boost::filesystem::path path(path_);
    path /= std::to_string(user_id);
    if (!boost::filesystem::exists(path)) {
        CheckFile(path.string<std::string>());
    }
    return std::make_shared<History>(path.string<std::string>());
}

void HistoryDB::CheckPath() const {
    boost::filesystem::path path(path_);
    if (boost::filesystem::exists(path)) {
        if (!boost::filesystem::is_directory(path)) {
            throw PathIsFileException(path.string());
        }
    } else {
        boost::filesystem::create_directories(path);
        if (!boost::filesystem::exists(path)) {
            throw PathCreateException(path.string());
        }
    }
}

void HistoryDB::CheckFile(const std::string& path) {
    std::ofstream f(path, std::ios_base::binary | std::ios_base::app);
    if (!f.is_open()) {
        throw util::FileWriteException(path);
    }
}

}
