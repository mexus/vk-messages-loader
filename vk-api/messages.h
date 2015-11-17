#pragma once

#include "communication-interface.h"

namespace vk_api {

class MessageAPI {
public:
    MessageAPI(CommunicationInterface* interface);

    std::vector<Message> GetMessages(uint64_t user_id, uint64_t start_message_id, uint64_t count = 0);

private:
    CommunicationInterface * const vk_interface_;
    static const std::string kInterfaceName;

};

}
