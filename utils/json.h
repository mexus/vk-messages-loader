#pragma once

#include <cinttypes>
#include <iostream>
#include <string>
#include <vector>
#include <rapidjson/document.h>

namespace util {

using JsonAllocator = rapidjson::Document::AllocatorType;

template<class T>
bool JsonToObject(const rapidjson::Value& json, T* object);

template<> bool JsonToObject<std::string>(const rapidjson::Value& json, std::string* object);
template<> bool JsonToObject<uint64_t>(const rapidjson::Value& json, uint64_t* object);
template<> bool JsonToObject<time_t>(const rapidjson::Value& json, time_t* object);

template<class T>
bool JsonToObject(const rapidjson::Value& json, std::vector<T>* object) {
    if (!json.IsArray()) {
        std::cerr << "Json value is not an array\n";
        return false;
    }
    auto size = json.Size();
    for (decltype(size) i = 0; i != size; ++i) {
        auto &item = json[i];
        T new_object;
        if (!JsonToObject(item, &new_object)) {
            std::cerr << "Can't convert item #" << i << " to object\n";
        } else {
            object->push_back(std::move(new_object));
        }
    }
    return true;
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
bool JsonGetMember(const rapidjson::Value& json, const std::string& name, T* value) {
    auto it = json.FindMember(name.c_str());
    if (it == json.MemberEnd()) {
        std::cerr << "Can't find field '" << name << "'\n";
        return false;
    } else if (!JsonToObject(it->value, value)) {
        std::cerr << "Member '" << name << "' can not be created from json\n";
        return false;
    }
    return true;
}

void JsonAddMembers(rapidjson::Value* json, JsonAllocator& allocator);
template<class T, class... Args>
void JsonAddMembers(rapidjson::Value* json, JsonAllocator& allocator, const std::string& name, const T& value, Args&&... args) {
    JsonAddMember(json, name, value, allocator);
    JsonAddMembers(json, allocator, std::forward<Args>(args)...);
}

bool JsonGetMembers(const rapidjson::Value& json);
template<class T, class... Args>
bool JsonGetMembers(const rapidjson::Value& json, const std::string& name, T* value, Args&&... args) {
    if (!json.IsObject()) {
        std::cerr << "Passed 'json' value is not an object\n";
        return false;
    }
    if (!JsonGetMember(json, name, value)) {
        return false;
    }
    return JsonGetMembers(json, std::forward<Args>(args)...);
}

rapidjson::Document JsonFromFile(const std::string& file_name);
bool JsonToFile(const std::string& file_name, const rapidjson::Document& document);

rapidjson::Document JsonFromStream(std::istream& stream);
void JsonToStream(std::ostream& stream, const rapidjson::Document& document);

}
