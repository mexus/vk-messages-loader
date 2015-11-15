#pragma once

#include "communication-interface.h"
#include "attachment.h"

namespace vk_api {

class MessageAPI {
public:
    struct Message {
        uint64_t id;
        time_t date;
        uint64_t from_id;
        uint64_t user_id;
        std::string body;
        std::vector<std::unique_ptr<Attachment>> attachments;
    };

    MessageAPI(CommunicationInterface* interface);

    std::vector<Message> GetMessages(uint64_t user_id, uint64_t start_message_id, uint64_t count = 0);

private:
    CommunicationInterface * const vk_interface_;
    static const std::string kInterfaceName;

};

}
