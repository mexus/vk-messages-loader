#pragma once
#include <utils/json.h>

namespace manager {

struct Settings {
    std::string application_id;
    std::string storage_path;
    std::vector<uint64_t> users;
};

}

namespace util {
template<> void JsonToObject<manager::Settings>(const rapidjson::Value& json, manager::Settings* object);
template<> rapidjson::Value JsonFromObject<manager::Settings>(const manager::Settings& object, JsonAllocator& allocator);
}
