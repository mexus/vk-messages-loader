#include <cpr.h>
#include <vk-api/exceptions.h>
#include <vk-api/requests-manager.h>
#include <iostream>

namespace {

enum HTTP_CODES { OK = 200 };
}

namespace vk_api {

RequestsManager::RequestsManager() {}

cpr::Response RequestsManager::GetHttpResponse(
    const cpr::Url& url, const cpr::Parameters& parameters) {
  static const std::chrono::milliseconds request_timeout(1000);
  static const unsigned max_attempts = 3;
  unsigned attempt = 0;
  while (true) {
    intervals_manager_.Wait();
    auto http_response =
        cpr::Get(url, parameters, cpr::Timeout(request_timeout.count()));
    LOG(DEBUG) << http_response.url;
    long status_code = http_response.status_code;
    if (status_code == 0) {
      LOG(DEBUG) << "Got status code zero";
      if (attempt == max_attempts) {
        LOG(DEBUG) << "Attempts exceeded (" << attempt << "/" << max_attempts
                   << ")";
        THROW_AT(RequestException, http_response);
      }
      ++attempt;
      continue;
    } else if (status_code != HTTP_CODES::OK) {
      THROW_AT(RequestException, http_response);
    }
    return http_response;
  }
}

rapidjson::Document RequestsManager::GetDocument(
    const cpr::Url& url, const cpr::Parameters& parameters) {
  static const unsigned max_attempts = 3;
  unsigned attempt = 0;
  while (true) {
    auto http_response = GetHttpResponse(url, parameters);
    auto json = http_response.text;
    rapidjson::Document doc;
    doc.Parse(json.c_str());
    if (doc.HasParseError()) {
      if (attempt == max_attempts) {
        LOG(DEBUG) << "Attempts exceeded (" << attempt << "/" << max_attempts
                   << "), "
                   << "json text: " << json;
        THROW_AT(RequestParseException, http_response, doc);
      }
      ++attempt;
      continue;
    }

    if (!doc.IsObject()) {
      if (attempt == max_attempts) {
        LOG(DEBUG) << "Attempts exceeded (" << attempt << "/" << max_attempts
                   << "), "
                   << "json text: " << json;
        THROW_AT(util::json::NotAnObjectException);
      }
      ++attempt;
      continue;
    }
    return doc;
  }
}

RequestsManager::Response RequestsManager::MakeRequest(
    const cpr::Url& url, const cpr::Parameters& parameters) {
  auto doc = GetDocument(url, parameters);
  if (doc.HasMember("error")) {
    VkError vk_error;
    util::JsonToObject(doc["error"], &vk_error);
    THROW_AT(ApiException, std::move(vk_error));
  }
  return doc;
}

void RequestsManager::IncreaseInterval() {
  intervals_manager_.IncreaseInterval();
}
}
