#include <manager/users-cache.h>

namespace manager {
namespace cache {

template<>
uint64_t GetIdentificator<User>(const User& data) {
    return data.id;
}

}
}

namespace util {

template<>
void JsonToObject<manager::cache::User>(const rapidjson::Value& json, manager::cache::User* object) {
    JsonGetMembers(json,
                   "id", &object->id,
                   "first_name", &object->first_name,
                   "last_name", &object->last_name);
}

template<>
rapidjson::Value JsonFromObject<manager::cache::User>(const manager::cache::User& object, JsonAllocator& allocator) {
    rapidjson::Value json(rapidjson::kObjectType);
    JsonAddMembers(&json, allocator,
                   "id", object.id,
                   "first_name", object.first_name,
                   "last_name", object.last_name);
    return json;
}

}
