#pragma once

#include <cinttypes>
#include <vector>
#include <map>
#include <utils/json.h>

namespace vk_api {

template<class T>
struct List {
    uint64_t count;
    std::vector<T> items;
};

enum VK_ERRORS {
    UNKNOWN = 1,
    APP_IS_OFF = 2,
    UNKNOWN_METHOD = 3,
    WRONG_SIGNATURE = 4,
    AUTHORIZATION_FAILED = 5,
    TOO_FAST_QUERIES = 6,
    PERMISSION_DENIED = 7,
    INCORRECT_QUERY = 8,
    TOO_MANY_SIMILAR_QUERIES = 9,
    INTERNAL_ERROR = 10,
    TEST_APPLICATION = 11,
    CAPTURE_REQUIRED = 14,
    ACCESS_DENIED = 15,
    HTTPS_REQUIRED = 16,
    VALIDATION_REQUIRED = 17,
    METHOD_DISABLED = 23,
    USER_CONFIRMATION_REQUIRED = 24,
    PARAMETER_ERROR = 100,
    WRONG_APP_ID = 101,
    WRONG_USER_ID = 113,
    WRONG_TIMESTAMP = 150
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
    if (!json.IsObject()) {
        throw util::json::NotAnObjectException();
    }
    JsonGetMembers(json,
                   "count", &list->count,
                   "items", &list->items);
}

}
