#pragma once
#include <rapidjson/document.h>
#include <vk-api/intervals-manager.h>
#include <cpr.h>

#include "data-types.h"

namespace vk_api {

class RequestsManager {
public:
    struct Response {
        rapidjson::Document json;
        Error error;
    };
    RequestsManager();
    Response MakeRequest(const cpr::Url& url, const cpr::Parameters& parameters);

private:
    IntervalsManager intervals_manager_;

    enum Handling {
        RETRY,
        BREAK,
        PASS
    };

    Handling HandleVkError(const rapidjson::Value& json, Error* error);

};

}
