#pragma once
#include <rapidjson/document.h>
#include <vk-api/intervals-manager.h>
#include <cpr.h>

namespace vk_api {

class RequestsManager {
public:
    typedef std::unique_ptr<rapidjson::Document> Response;
    RequestsManager(IntervalsManager* intervals_manager);
    Response MakeRequest(const cpr::Url& url, const cpr::Parameters& parameters);

private:
    IntervalsManager * const intervals_manager_;

};

}
