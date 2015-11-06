#include <storage/history-db.h>
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>

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
    if (!storage) {
        return {};
    }
    users_history_[user_id] = storage;
    return storage;
}

std::shared_ptr<History> HistoryDB::GetStorage(uint64_t user_id) {
    if (!CheckPath()) {
        return {};
    }
    boost::filesystem::path path(path_);
    path /= std::to_string(user_id);
    if (!boost::filesystem::exists(path) && !CheckFile(path.string<std::string>())) {
        return {};
    }
    auto storage = new History(path.string<std::string>());
    return std::shared_ptr<History>(storage);
}

bool HistoryDB::CheckPath() const {
    boost::filesystem::path path(path_);
    if (boost::filesystem::exists(path)) {
        if (!boost::filesystem::is_directory(path)) {
            std::cerr << "Path " << path << " exists, but it is not a directory\n";
            return false;
        }
    } else {
        try {
            boost::filesystem::create_directories(path);
        } catch(const boost::filesystem::filesystem_error& fs_error) {
            std::cerr << "Unable to create path because of an error: " << fs_error.what() << "\n";
            return false;
        }
        if (!boost::filesystem::exists(path)) {
            std::cerr << "Unable to create path " << path << "\n";
            return false;
        }
    }
    return true;
}

bool HistoryDB::CheckFile(const std::string& path) {
    std::ofstream f(path, std::ios_base::binary | std::ios_base::app);
    if (!f.is_open()) {
        std::cerr << "Can't create a file {" << path << "}\n";
        return false;
    }
    return true;
}

}
