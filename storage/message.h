#pragma once
#include <cstdint>
#include <vector>
#include <string>

namespace storage {

struct Attachment {
    std::string name;
    std::string body;
};

struct Message {
    uint64_t message_id;
    time_t date;
    uint64_t from_user_id;
    uint64_t to_user_id;
    std::string body;
    std::vector<Attachment> attachments;
};

}
