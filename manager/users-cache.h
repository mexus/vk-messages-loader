#pragma once

#include <vk-api/friends.h>
#include "cache.h"

namespace manager {
namespace cache {

typedef MappedCache<vk_api::User> Users;

template <>
uint64_t GetIdentificator(const vk_api::User& data);
}
}
