#include <rapidjson/prettywriter.h>
#include <utils/json.h>
#include <fstream>
#include <iostream>

namespace util {

class IStreamWrapper {
 public:
  typedef char Ch;

  IStreamWrapper(std::istream& is) : is_(is) {}

  Ch Peek() const {
    int c = is_.peek();
    return c == std::char_traits<char>::eof() ? '\0' : static_cast<Ch>(c);
  }

  Ch Take() {
    int c = is_.get();
    return c == std::char_traits<char>::eof() ? '\0' : static_cast<Ch>(c);
  }

  size_t Tell() const { return static_cast<size_t>(is_.tellg()); }

  Ch* PutBegin() { return nullptr; }
  void Put(Ch) {}
  void Flush();
  size_t PutEnd(Ch*) { return 0; }

 private:
  std::istream& is_;
};

class OStreamWrapper {
 public:
  typedef char Ch;
  OStreamWrapper(std::ostream& os) : os_(os) {}

  Ch Peek() const;
  Ch Take();
  size_t Tell() const;
  Ch* PutBegin();
  void Put(Ch c) { os_.put(c); }
  void Flush() { os_.flush(); }
  size_t PutEnd(Ch*);

 private:
  std::ostream& os_;
};

void JsonAddMembers(rapidjson::Value* /*json*/, JsonAllocator& /*allocator*/) {}

JsonMembersGetter::JsonMembersGetter(const rapidjson::Value& json)
    : json_(json) {
  if (!json.IsObject()) {
    THROW_AT(json::NotAnObjectException);
  }
}

const JsonMembersGetter& JsonMembersGetter::operator()() const { return *this; }

JsonMembersGetter JsonGetMembers(const rapidjson::Value& json) {
  return JsonMembersGetter(json);
}

template <>
void JsonToObject<>(const rapidjson::Value& json, std::string* object) {
  if (!json.IsString()) {
    THROW_AT(json::TypeStringException);
  }
  *object = std::string(json.GetString(), json.GetStringLength());
}

template <>
void JsonToObject<>(const rapidjson::Value& json, uint64_t* object) {
  if (!json.IsUint64()) {
    THROW_AT(json::TypeUInt64Exception);
  }
  *object = json.GetUint64();
}

template <>
void JsonToObject<>(const rapidjson::Value& json, time_t* object) {
  if (!json.IsInt64()) {
    THROW_AT(json::TypeInt64Exception);
  }
  *object = json.GetInt64();
}

template <>
rapidjson::Value JsonFromObject<>(const std::string& object,
                                  JsonAllocator& allocator) {
  rapidjson::Value json(rapidjson::kStringType);
  json.SetString(object.c_str(), object.size(), allocator);
  return json;
}

template <>
rapidjson::Value JsonFromObject<>(const uint64_t& object,
                                  JsonAllocator& /*allocator*/) {
  rapidjson::Value json(rapidjson::kNumberType);
  json.SetUint64(object);
  return json;
}

template <>
rapidjson::Value JsonFromObject<>(const time_t& object,
                                  JsonAllocator& /*allocator*/) {
  rapidjson::Value json(rapidjson::kNumberType);
  json.SetInt64(object);
  return json;
}

rapidjson::Document JsonFromFile(const std::string& file_name) {
  std::ifstream f(file_name);
  if (!f) {
    THROW_AT(FileReadException, file_name);
  }
  return JsonFromStream(f);
}

void JsonToFile(const std::string& file_name,
                const rapidjson::Document& document) {
  std::ofstream f(file_name);
  if (!f) {
    THROW_AT(FileWriteException, file_name);
  }
  JsonToStream(f, document);
}

rapidjson::Document JsonFromStream(std::istream& stream) {
  rapidjson::Document doc;
  IStreamWrapper wrapper(stream);
  doc.ParseStream(wrapper);
  return doc;
}

void JsonToStream(std::ostream& stream, const rapidjson::Document& document) {
  OStreamWrapper wrapper(stream);
  rapidjson::PrettyWriter<OStreamWrapper> writer(wrapper);
  document.Accept(writer);
}
}
