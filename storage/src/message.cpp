#include <storage/message.h>
#include <iostream>
#include <utils/json.h>

namespace util {

template<>
void JsonToObject<storage::Attachment>(const rapidjson::Value& json, storage::Attachment* object) {
    JsonGetMembers(json,
                   "name", &object->name,
                   "body", &object->body);
}

template<>
rapidjson::Value JsonFromObject<storage::Attachment>(const storage::Attachment& object, JsonAllocator& allocator) {
    rapidjson::Value json(rapidjson::kObjectType);
    JsonAddMembers(&json, allocator,
                  "name", object.name,
                  "body", object.body);
    return json;
}

template<>
void JsonToObject<storage::Message>(const rapidjson::Value& json, storage::Message* object) {
    JsonGetMembers(json,
                   "message_id", &object->message_id,
                   "date", &object->date,
                   "from_user_id", &object->from_user_id,
                   "to_user_id", &object->to_user_id,
                   "body", &object->body,
                   "attachments", &object->attachments);
}

template<>
rapidjson::Value JsonFromObject<storage::Message>(const storage::Message& object, JsonAllocator& allocator) {
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
