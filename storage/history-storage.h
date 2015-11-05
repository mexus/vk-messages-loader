#pragma once
#include <map>
#include <memory>
#include "message.h"
#include "substorage.h"

namespace storage {

class HistoryStorage {
public:
    HistoryStorage(const std::string& path);

    std::shared_ptr<Substorage> GetUser(uint64_t user_id);

private:
    const std::string path_;
    std::map<uint64_t, std::weak_ptr<Substorage>> users_history_;

    std::shared_ptr<Substorage> GetStorage(uint64_t user_id);

    bool CheckPath() const;
    static bool CheckFile(const std::string& file_name);

};

}
