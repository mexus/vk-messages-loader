#pragma once
#include <cstdint>
#include <vector>
#include <utils/json.h>

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

namespace util {

template<> bool JsonToObject<storage::Attachment>(const rapidjson::Value& json, storage::Attachment* object);
template<> rapidjson::Value JsonFromObject<storage::Attachment>(const storage::Attachment& object, JsonAllocator& allocator);

template<> bool JsonToObject<storage::Message>(const rapidjson::Value& json, storage::Message* object);
template<> rapidjson::Value JsonFromObject<storage::Message>(const storage::Message& object, JsonAllocator& allocator);

}
