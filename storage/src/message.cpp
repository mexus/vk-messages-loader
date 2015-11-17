#include <storage/message.h>
#include <iostream>
#include <utils/json.h>

namespace util {

template<>
void JsonToObject<>(const rapidjson::Value& json, storage::Attachment* object) {
    uint64_t type;
    JsonGetMembers(json,
                   "type", &type,
                   "body", &object->body);
    object->type = static_cast<storage::AttachmentType>(type);
}

template<>
rapidjson::Value JsonFromObject<>(const storage::Attachment& object, JsonAllocator& allocator) {
    rapidjson::Value json(rapidjson::kObjectType);
    JsonAddMembers(&json, allocator,
                  "type", static_cast<uint64_t>(object.type),
                  "body", object.body);
    return json;
}

template<>
void JsonToObject<>(const rapidjson::Value& json, storage::Message* object) {
    JsonGetMembers(json,
                   "message_id", &object->message_id,
                   "date", &object->date,
                   "from_user_id", &object->from_user_id,
                   "to_user_id", &object->to_user_id, util::json::Optional{},
                   "body", &object->body,
                   "attachments", &object->attachments);
}

template<>
rapidjson::Value JsonFromObject<>(const storage::Message& object, JsonAllocator& allocator) {
    rapidjson::Value json(rapidjson::kObjectType);
    JsonAddMembers(&json, allocator,
                   "message_id", object.message_id,
                   "date", object.date,
                   "from_user_id", object.from_user_id,
                   "to_user_id", object.to_user_id,
                   "body", object.body,
                   "attachments", object.attachments
                   );
    return json;
}

}
