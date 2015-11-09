#include <manager/export.h>
#include <fstream>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace manager {

Export::Export()
          : locale_(std::cout.getloc(),
                    new boost::posix_time::time_facet("%Y/%m/%d %H:%M:%S")) {
}

bool Export::ExportToFile(const std::vector<vk_api::FriendsAPI::Friend>& friends,
                          const std::shared_ptr<storage::History>& history,
                          const std::string& path) const {
    std::ofstream f(path);
    if (!f) {
        std::cerr << "Unable to open file '" << path << "' for writing\n";
        return false;
    }
    auto messages = history->GetData();
    for (auto& message: messages) {
        f << FormateDate(message.date) << " ";
        f << GetUserName(friends, message.from_user_id) << ": ";
        f << message.body << "\n";
        f.flush();
        if (!f) {
            std::cerr << "Some file error has happend\n";
            return false;
        }
    }
    return true;
}

std::string Export::FormateDate(time_t date_time) const {
    std::stringstream s;
    s.imbue(locale_);
    s << boost::posix_time::from_time_t(date_time);
    return s.str();
}

namespace {
struct FriendsIdCompare {
    uint64_t id;
    bool operator()(const vk_api::FriendsAPI::Friend& user) const {
        return user.user_id == id;
    }
};
}

std::string Export::GetUserName(const std::vector<vk_api::FriendsAPI::Friend>& friends, uint64_t id) {
    auto it = std::find_if(friends.begin(), friends.end(), FriendsIdCompare{id});
    if (it == friends.end()) {
        return std::to_string(id);
    }
    return it->first_name + " " + it->last_name;
}

}
