#pragma once

#include "communication-interface.h"
#include "data-types.h"

namespace vk_api {

class MessageAPI {
public:
    MessageAPI(CommunicationInterface* interface);

    std::vector<Message> GetMessages(uint64_t user_id, uint64_t start_message_id, uint64_t count = 0) const;
    std::vector<Message> GetLastMessages() const;

private:
    static const std::string kInterfaceName;
    CommunicationInterface * const vk_interface_;

};

}
