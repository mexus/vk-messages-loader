#pragma once

#include <utils/json.h>
#include <map>
#include <string>
#include "exceptions.h"

namespace manager {
namespace cache {

template <class T>
uint64_t GetIdentificator(const T& data);

template <class T>
class MappedCache {
  static_assert(std::is_default_constructible<T>::value,
                "T should have a default constructor");
  static_assert(std::is_move_assignable<T>::value,
                "T should be move assignable");

 public:
  MappedCache(const std::string& file_name) : file_name_(file_name) {}

  ~MappedCache() {
    try {
      Save();
    } catch (const util::BasicException& e) {
      LOG(ERROR) << "Caught an exception at `" << e.GetAt() << "` "
                 << "while saving a data: " << e.GetText();
    }
  }

  void AddData(T&& data) {
    uint64_t id = cache::GetIdentificator(data);
    data_[id] = std::move(data);
  }

  void AddData(const T& data) {
    uint64_t id = cache::GetIdentificator(data);
    data_[id] = data;
  }

  T GetData(uint64_t id) const {
    auto it = data_.find(id);
    if (it == data_.end()) {
      THROW_AT(NoDataException, id);
    }
    return it->second;
  }

  const std::vector<T> GetDataAsVector() const {
    std::vector<T> result;
    result.reserve(data_.size());
    for (auto& pair : data_) {
      result.push_back(pair.second);
    }
    return result;
  }

  void Load() {
    auto doc = util::JsonFromFile(file_name_);
    if (!doc.IsObject()) {
      THROW_AT(util::json::NotAnObjectException);
    }
    auto it = doc.FindMember(kJsonField.c_str());
    if (it == doc.MemberEnd()) {
      THROW_AT(util::json::NoFieldException, kJsonField);
    }
    auto& json_vector = it->value;
    if (!json_vector.IsArray()) {
      THROW_AT(util::json::NotAnArrayException);
    }
    auto size = json_vector.Size();
    for (decltype(size) i = 0; i != size; ++i) {
      T data;
      try {
        util::JsonToObject(json_vector[i], &data);
        AddData(std::move(data));
      } catch (const util::json::Exception& e) {
        LOG(ERROR)
            << i << "-th element of an array can not be converted to a 'User': "
            << e.what();
      }
    }
  }

  void Save() const {
    rapidjson::Document doc;
    auto& allocator = doc.GetAllocator();
    doc.SetObject();
    auto vector = GetDataAsVector();
    util::JsonAddMember(&doc, kJsonField, vector, allocator);
    util::JsonToFile(file_name_, doc);
  }

 private:
  static const std::string kJsonField;
  const std::string file_name_;
  std::map<uint64_t, T> data_;
};

template <class T>
const std::string MappedCache<T>::kJsonField = "data";
}
}
