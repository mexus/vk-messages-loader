#pragma once

#include <storage/history.h>
#include <vk-api/friends.h>

namespace manager {

class Export {
public:
    Export();
    bool ExportToFile(const std::vector<vk_api::FriendsAPI::Friend>& friends,
                      const std::shared_ptr<storage::History>& history,
                      const std::string& path) const;

private:
    std::locale locale_;

    std::string FormateDate(time_t date_time) const;
    static std::string GetUserName(const std::vector<vk_api::FriendsAPI::Friend>& friends,
                                   uint64_t id);
};

}
