#pragma once

#include <manager/users-cache.h>
#include <storage/history.h>

namespace manager {

class HistoryExport {
public:
    HistoryExport(const cache::Users& users_cache);
    void ExportToFile(const std::shared_ptr<storage::History>& history,
                      const std::string& path) const;

private:
    const cache::Users& users_cache_;
    std::locale locale_;

    std::string FormateDate(time_t date_time) const;
    std::string GetUserName(uint64_t id) const;
};

}
