#pragma once

#include <cinttypes>
#include <iostream>
#include <vector>
#include <utils/json.h>

namespace vk_api {

template<class T>
struct List {
    uint64_t count;
    std::vector<T> items;
};

struct Error {
    enum Status {
        OK = 0,
        HTTP_ERROR,
        PARSE_ERROR,
        VK_ERROR
    };
    Status status;
    std::string error_message;
    uint64_t vk_error_code;
};

}

namespace util {

template<class T>
bool JsonToObject(const rapidjson::Value& json, vk_api::List<T>* list) {
    if (!json.IsObject()) {
        std::cerr << "Provided json value is not an object\n";
        return false;
    }
    bool res = JsonGetMembers(json,
                              "count", &list->count,
                              "items", &list->items);
    if (!res) {
        std::cerr << "Can't extract list from a json value\n";
    }
    return res;
}

template<>
bool JsonToObject<vk_api::Error>(const rapidjson::Value& json, vk_api::Error* error);

}
