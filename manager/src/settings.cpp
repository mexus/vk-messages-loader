#include <manager/settings.h>

namespace util {

template<>
bool JsonToObject<manager::Settings>(const rapidjson::Value& json, manager::Settings* object) {
    if (!json.IsObject()) {
        std::cerr << "Json value is not an object\n";
        return false;
    }
    bool res = JsonGetMembers(json,
                              "application_id", &object->application_id,
                              "storage_path", &object->storage_path,
                              "users", &object->users);
    if (!res) {
        std::cerr << "Can't load settings from a json\n";
    }
    return res;
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
