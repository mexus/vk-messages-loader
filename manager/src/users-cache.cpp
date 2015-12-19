#include <manager/users-cache.h>

namespace manager {
namespace cache {

template<>
uint64_t GetIdentificator(const vk_api::User& data) {
    return data.user_id;
}

}
}
