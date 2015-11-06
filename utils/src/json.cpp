#include <utils/json.h>
#include <iostream>
#include <fstream>
#include <rapidjson/prettywriter.h>

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

    size_t Tell() const {
        return static_cast<size_t>(is_.tellg());
    }

    Ch* PutBegin();
    void Put(Ch);
    void Flush();
    size_t PutEnd(Ch*);

private:
    std::istream& is_;

};

class OStreamWrapper {
public:
    typedef char Ch;
    OStreamWrapper(std::ostream& os) : os_(os) {}

    Ch Peek() const;
    Ch Take() ;
    size_t Tell() const;
    Ch* PutBegin();
    void Put(Ch c) { os_.put(c); }
    void Flush() { os_.flush(); }
    size_t PutEnd(Ch*);

private:
    std::ostream& os_;

};

void JsonAddMembers(rapidjson::Value* /*json*/, JsonAllocator& /*allocator*/) {
}

bool JsonGetMembers(const rapidjson::Value& /*json*/) {
    return true;
}

template<>
bool JsonToObject<std::string>(const rapidjson::Value& json, std::string* object) {
    if (!json.IsString()) {
        std::cerr << "Json value is not a string\n";
        return false;
    }
    *object = json.GetString();
    return true;
}

template<>
bool JsonToObject<uint64_t>(const rapidjson::Value& json, uint64_t* object) {
    if (!json.IsUint64()) {
        std::cerr << "Json value is not a uint64\n";
        return false;
    }
    *object = json.GetUint64();
    return true;
}

template<>
bool JsonToObject<time_t>(const rapidjson::Value& json, time_t* object) {
    if (!json.IsInt64()) {
        std::cerr << "Json value is not a int64\n";
        return false;
    }
    *object = json.GetInt64();
    return true;
}

template<>
rapidjson::Value JsonFromObject<std::string>(const std::string& object, JsonAllocator& allocator) {
    rapidjson::Value json(rapidjson::kStringType);
    json.SetString(object.c_str(), object.size(), allocator);
    return json;
}

template<>
rapidjson::Value JsonFromObject<uint64_t>(const uint64_t& object, JsonAllocator& /*allocator*/) {
    rapidjson::Value json(rapidjson::kNumberType);
    json.SetUint64(object);
    return json;
}

template<>
rapidjson::Value JsonFromObject<time_t>(const time_t& object, JsonAllocator& /*allocator*/) {
    rapidjson::Value json(rapidjson::kNumberType);
    json.SetInt64(object);
    return json;
}

rapidjson::Document JsonFromFile(const std::string& file_name) {
    std::ifstream f(file_name);
    if (!f) {
        std::cerr << "Can't open file '" << file_name << "' for reading\n";
        return {};
    }
    return JsonFromStream(f);
}

bool JsonToFile(const std::string& file_name, const rapidjson::Document& document) {
    std::ofstream f;
    if (!f) {
        std::cerr << "Can't open file '" << file_name << "' for writing\n";
        return {};
    }
    JsonToStream(f, document);
    return true;
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
