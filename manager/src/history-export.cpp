#include <manager/history-export.h>
#include <fstream>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace manager {

HistoryExport::HistoryExport(const cache::Users& users_cache)
          : users_cache_(users_cache),
            locale_(std::cout.getloc(),
                    new boost::posix_time::time_facet("%Y/%m/%d %H:%M:%S")) {
}

void HistoryExport::ExportToFile(const std::shared_ptr<storage::History>& history,
                          const std::string& path) const {
    std::ofstream f(path);
    if (!f) {
        THROW_AT(util::FileWriteException, path);
    }
    auto messages = history->GetData();
    for (auto& message: messages) {
        f << FormateDate(message.date) << " ";
        f << GetUserName(message.from_user_id) << ": ";
        f << message.body << "\n";
    }
}

std::string HistoryExport::FormateDate(time_t date_time) const {
    std::stringstream s;
    s.imbue(locale_);
    s << boost::posix_time::from_time_t(date_time);
    return s.str();
}

std::string HistoryExport::GetUserName(uint64_t id) const {
    try {
        auto user = users_cache_.GetData(id);
        return user.first_name + " " + user.last_name;
    } catch (const manager::cache::NoDataException&) {
        return std::to_string(id);
    }
}

}