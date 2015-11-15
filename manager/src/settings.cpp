#include <manager/settings.h>

namespace util {

template<>
void JsonToObject<manager::Settings>(const rapidjson::Value& json, manager::Settings* object) {
    if (!json.IsObject()) {
        throw util::json::NotAnObjectException();
    }
    JsonGetMembers(json,
                   "application_id", &object->application_id,
                   "storage_path", &object->storage_path,
                   "users", &object->users, util::json::Optional{});
}

template<>
rapidjson::Value JsonFromObject<manager::Settings>(const manager::Settings& object, JsonAllocator& allocator) {
    rapidjson::Value json(rapidjson::kObjectType);
    JsonAddMembers(&json, allocator,
                              "application_id", object.application_id,
                              "storage_path", object.storage_path,
                              "users", object.users);
    return json;
}

}
