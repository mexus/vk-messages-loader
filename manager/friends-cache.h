#pragma once

#include <vk-api/friends.h>
#include "cache.h"

namespace manager {
namespace cache {

typedef MappedCache<vk_api::User> FriendsCache;

template<> uint64_t GetIdentificator<vk_api::User>(const vk_api::User& data);

}
}
