#pragma once

#include "communication-interface.h"
#include "data-types.h"

namespace vk_api {

struct MessageInfo {
  uint64_t id;
  uint64_t user_id;
  uint64_t chat_id;
};

class MessageAPI {
 public:
  MessageAPI(CommunicationInterface* interface);

  std::vector<Message> GetMessages(uint64_t user_id, uint64_t start_message_id,
                                   uint64_t count = 0) const;
  std::vector<MessageInfo> GetLastMessages() const;

  std::vector<Chat> GetChats(const std::vector<uint64_t>& chat_ids) const;

 private:
  static const std::string kInterfaceName;
  CommunicationInterface* const vk_interface_;
};
}
