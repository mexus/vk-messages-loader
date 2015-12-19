#pragma once

#include <cinttypes>
#include <vector>
#include <map>
#include <utils/json.h>
#include "attachments.h"
#include "errors.h"

namespace vk_api {

struct User {
    uint64_t user_id;
    std::string first_name;
    std::string last_name;
};

struct Message {
    uint64_t id;
    time_t date;
    uint64_t from_id;
    uint64_t chat_id;
    uint64_t user_id;
    std::string body;
    std::vector<std::unique_ptr<Attachment>> attachments;
};

template<class T>
struct List {
    uint64_t count;
    std::vector<T> items;
};

struct VkError {
    struct RequestParam {
        std::string key;
        std::string value;
    };

    uint64_t error_code;
    std::string error_msg;
    std::vector<RequestParam> request_params;
    std::map<std::string, std::string> additional_data;
};

}

namespace util {

template<class T>
void JsonToObject(const rapidjson::Value& json, vk_api::List<T>* list) {
    JsonGetMembers(json,
                   "count", &list->count,
                   "items", &list->items);
}

}
