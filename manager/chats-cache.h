#pragma once

#include <vk-api/data-types.h>
#include "cache.h"

namespace manager {
namespace cache {

typedef MappedCache<vk_api::Chat> Chats;

template <>
uint64_t GetIdentificator(const vk_api::Chat& chat);
}
}
