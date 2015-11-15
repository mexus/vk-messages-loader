#pragma once

#include <cinttypes>
#include <type_traits>
#include <iostream>
#include <string>
#include <vector>
#include <rapidjson/document.h>
#include "json-exceptions.h"

namespace util {

using JsonAllocator = rapidjson::Document::AllocatorType;

template<class T>
void JsonToObject(const rapidjson::Value& json, T* object);

template<> void JsonToObject<std::string>(const rapidjson::Value& json, std::string* object);
template<> void JsonToObject<uint64_t>(const rapidjson::Value& json, uint64_t* object);
template<> void JsonToObject<time_t>(const rapidjson::Value& json, time_t* object);

template<class T>
void JsonToObject(const rapidjson::Value& json, std::vector<T>* object) {
    if (!json.IsArray()) {
        THROW_AT(json::NotAnArrayException);
    }
    auto size = json.Size();
    object->reserve(static_cast<size_t>(object->size() + size));
    for (decltype(size) i = 0; i != size; ++i) {
        auto &item = json[i];
        try {
            T new_object;
            JsonToObject(item, &new_object);
            object->push_back(std::move(new_object));
        } catch (const json::Exception& e) {
            std::cerr << "Can't convert item #" << i << " to object: " << e.what() << "\n";
        }
    }
}

template<class T>
rapidjson::Value JsonFromObject(const T& object, JsonAllocator& allocator);

template<> rapidjson::Value JsonFromObject<std::string>(const std::string& object, JsonAllocator& allocator);
template<> rapidjson::Value JsonFromObject<uint64_t>(const uint64_t& object, JsonAllocator& allocator);
template<> rapidjson::Value JsonFromObject<time_t>(const time_t& object, JsonAllocator& allocator);

template<class T>
rapidjson::Value JsonFromObject(const std::vector<T>& object, JsonAllocator& allocator) {
    rapidjson::Value json(rapidjson::kArrayType);
    for (auto& item: object) {
        json.PushBack(JsonFromObject(item, allocator), allocator);
    }
    return json;
}

template<class T>
void JsonAddMember(rapidjson::Value* json, const std::string& name, const T& object, JsonAllocator& allocator) {
    json->AddMember(rapidjson::Value(rapidjson::kStringType).SetString(name.c_str(), name.size(), allocator),
                    JsonFromObject(object, allocator),
                    allocator);
}

template<class T>
void JsonGetMember(const rapidjson::Value& json, const std::string& name, T* object) {
    if (!json.IsObject()) {
        THROW_AT(json::NotAnObjectException);
    }
    auto it = json.FindMember(name.c_str());
    if (it == json.MemberEnd()) {
        THROW_AT(json::NoFieldException, name);
    }
    JsonToObject(it->value, object);
}

namespace json {
class Optional {
};
}

void JsonAddMembers(rapidjson::Value* json, JsonAllocator& allocator);
template<class T, class... Args>
void JsonAddMembers(rapidjson::Value* json, JsonAllocator& allocator, const std::string& name, const T& value, Args&&... args) {
    JsonAddMember(json, name, value, allocator);
    JsonAddMembers(json, allocator, std::forward<Args>(args)...);
}

template<class T, class... Args>
void JsonGetMembers(const rapidjson::Value& json, const std::string& name, T* value, json::Optional optional, Args&&... args);
template<class T, class... Args>
void JsonGetMembers(const rapidjson::Value& json, const std::string& name, T* value, Args&&... args);
void JsonGetMembers(const rapidjson::Value& json);

template<class T, class... Args>
void JsonGetMembers(const rapidjson::Value& json, const std::string& name, T* value, json::Optional /*optional*/, Args&&... args) {
    if (!json.IsObject()) {
        THROW_AT(json::NotAnObjectException);
    }
    JsonGetMembers(json, std::forward<Args>(args)...);
    try {
        JsonGetMember(json, name, value);
    } catch (const json::Exception& )
    {
        // Just skip it, it's optional after all
    }
}

template<class T, class... Args>
void JsonGetMembers(const rapidjson::Value& json, const std::string& name, T* value, Args&&... args) {
    if (!json.IsObject()) {
        THROW_AT(json::NotAnObjectException);
    }
    JsonGetMembers(json, std::forward<Args>(args)...);
    JsonGetMember(json, name, value);
}


rapidjson::Document JsonFromFile(const std::string& file_name);
void JsonToFile(const std::string& file_name, const rapidjson::Document& document);

rapidjson::Document JsonFromStream(std::istream& stream);
void JsonToStream(std::ostream& stream, const rapidjson::Document& document);

}
