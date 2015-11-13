#pragma once
#include <rapidjson/document.h>
#include <vk-api/intervals-manager.h>
#include <cpr.h>

#include "data-types.h"

namespace vk_api {

class RequestsManager {
public:
    using Response = rapidjson::Document;

    RequestsManager() = default;
    Response MakeRequest(const cpr::Url& url, const cpr::Parameters& parameters);

private:
    IntervalsManager intervals_manager_;

    rapidjson::Document GetDocument(const cpr::Url& url, const cpr::Parameters& parameters);
    cpr::Response GetHttpResponse(const cpr::Url& url, const cpr::Parameters& parameters);
    bool HandleVkError(const VkError& vk_error);

};

}
