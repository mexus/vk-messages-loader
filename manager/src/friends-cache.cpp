#include <manager/friends-cache.h>

namespace manager {
namespace cache {

template<>
uint64_t GetIdentificator<vk_api::FriendsAPI::Friend>(const vk_api::FriendsAPI::Friend& data) {
    return data.user_id;
}

}
}
