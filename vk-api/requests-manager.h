#pragma once
#include <cpr.h>
#include <rapidjson/document.h>
#include <vk-api/intervals-manager.h>

#include "data-types.h"

namespace vk_api {

class RequestsManager {
 public:
  using Response = rapidjson::Document;

  RequestsManager();
  Response MakeRequest(const cpr::Url& url, const cpr::Parameters& parameters);
  void IncreaseInterval();

 private:
  IntervalsManager intervals_manager_;

  rapidjson::Document GetDocument(const cpr::Url& url,
                                  const cpr::Parameters& parameters);
  cpr::Response GetHttpResponse(const cpr::Url& url,
                                const cpr::Parameters& parameters);
};
}
