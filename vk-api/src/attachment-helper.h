#pragma once
#include <utils/json.h>

namespace vk_api {

template<class T>
bool FirstFieldToObject(const rapidjson::Value& json, const std::vector<const char*>& fields, T* object) {
    auto end = json.MemberEnd();
    for (auto& field: fields) {
        auto it = json.FindMember(field);
        if (it != end) {
            util::JsonToObject(it->value, object);
            return true;
        }
    }
    return false;
}

}
