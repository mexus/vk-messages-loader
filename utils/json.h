#pragma once

#include <easylogging++.h>
#include <rapidjson/document.h>
#include <cinttypes>
#include <string>
#include <type_traits>
#include <vector>
#include "json-exceptions.h"

namespace util {

using JsonAllocator = rapidjson::Document::AllocatorType;

template <class T>
void JsonToObject(const rapidjson::Value& json, T* object);

template <>
void JsonToObject<>(const rapidjson::Value& json, std::string* object);
template <>
void JsonToObject<>(const rapidjson::Value& json, uint64_t* object);
template <>
void JsonToObject<>(const rapidjson::Value& json, time_t* object);

template <class T>
void JsonToObject(const rapidjson::Value& json, std::vector<T>* object) {
  if (!json.IsArray()) {
    THROW_AT(json::NotAnArrayException);
  }
  auto size = json.Size();
  object->reserve(static_cast<size_t>(object->size() + size));
  for (decltype(size) i = 0; i != size; ++i) {
    auto& item = json[i];
    try {
      T new_object;
      JsonToObject(item, &new_object);
      object->push_back(std::move(new_object));
    } catch (const json::Exception& e) {
      LOG(WARNING) << "Can't convert item #" << i << " to object: " << e.what();
    }
  }
}

template <class T>
rapidjson::Value JsonFromObject(const T& object, JsonAllocator& allocator);

template <>
rapidjson::Value JsonFromObject<>(const std::string& object,
                                  JsonAllocator& allocator);
template <>
rapidjson::Value JsonFromObject<>(const uint64_t& object,
                                  JsonAllocator& allocator);
template <>
rapidjson::Value JsonFromObject<>(const time_t& object,
                                  JsonAllocator& allocator);

template <class T>
rapidjson::Value JsonFromObject(const std::vector<T>& object,
                                JsonAllocator& allocator) {
  rapidjson::Value json(rapidjson::kArrayType);
  for (auto& item : object) {
    json.PushBack(JsonFromObject(item, allocator), allocator);
  }
  return json;
}

template <class T>
void JsonAddMember(rapidjson::Value* json, const std::string& name,
                   const T& object, JsonAllocator& allocator) {
  json->AddMember(rapidjson::Value(rapidjson::kStringType)
                      .SetString(name.c_str(), name.size(), allocator),
                  JsonFromObject(object, allocator), allocator);
}

template <class T>
void JsonGetMember(const rapidjson::Value& json, const std::string& name,
                   T* object) {
  if (!json.IsObject()) {
    THROW_AT(json::NotAnObjectException);
  }
  auto it = json.FindMember(name.c_str());
  if (it == json.MemberEnd()) {
    THROW_AT(json::NoFieldException, name);
  }
  JsonToObject(it->value, object);
}

rapidjson::Document JsonFromFile(const std::string& file_name);
void JsonToFile(const std::string& file_name,
                const rapidjson::Document& document);

rapidjson::Document JsonFromStream(std::istream& stream);
void JsonToStream(std::ostream& stream, const rapidjson::Document& document);
}
