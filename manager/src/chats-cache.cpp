#include <manager/chats-cache.h>

namespace manager {
namespace cache {

template <>
uint64_t GetIdentificator(const vk_api::Chat& chat) {
  return chat.chat_id;
}
}
}

