#pragma once

#include <vk-api/friends.h>
#include "cache.h"

namespace manager {
namespace cache {

typedef MappedCache<vk_api::FriendsAPI::Friend> FriendsCache;

template<> uint64_t GetIdentificator<vk_api::FriendsAPI::Friend>(const vk_api::FriendsAPI::Friend& data);

}
}
