#include <vk-api/requests-manager.h>
#include <iostream>
#include <cpr.h>

namespace {

enum HTTP_CODES {
    OK = 200
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
    METHOD_DISABLER = 23,
    USER_CONFIRMATION_REQUIRED = 24,
    PARAMETER_ERROR = 100,
    WRONG_APP_ID = 101,
    WRONG_USER_ID = 113,
    WRONG_TIMESTAMP = 150
};

}


namespace vk_api {

RequestsManager::RequestsManager() {
}

RequestsManager::Response RequestsManager::MakeRequest(const cpr::Url& url, const cpr::Parameters& parameters) {
    static const std::chrono::milliseconds request_timeout(1000);
    intervals_manager_.Wait();
    auto http_response = cpr::Get(url, parameters, cpr::Timeout(request_timeout.count()));
    if (http_response.status_code == HTTP_CODES::OK) {
        auto json = http_response.text;
        rapidjson::Document doc;
        doc.Parse(json.c_str());
        if (doc.HasParseError()) {
            std::cerr << "URL: [" << http_response.url << "]\n";
            std::cerr << "Received a reply:\n" << json << "\n";
            std::cerr << "Can't parse response, parse error #" << doc.GetParseError()
                      << "at" << doc.GetErrorOffset() << ":\n" << json << "\n";
            return {{}, Error{Error::PARSE_ERROR, "", 0}};
        }
        if (!doc.IsObject()) {
            std::cerr << "URL: [" << http_response.url << "]\n";
            std::cerr << "Received a reply:\n" << json << "\n";
            std::cerr << "Received json is not an object\n";
            return {{}, {Error::PARSE_ERROR, "Received json is not an object", 0}};
        }
        if (doc.HasMember("error")){
            Error error;
            Handling action = HandleVkError(doc["error"], &error);
            switch (action) {
                case PASS:
                    return {std::move(doc), error};
                case RETRY:
                    std::cout << "Make another attempt with url " << http_response.url << "\n";
                    return MakeRequest(url, parameters);
                case BREAK:
                    return {{}, error};
            }
        }
        return {std::move(doc), {Error::OK, "", 0}};
    } else if (http_response.status_code == 0) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return MakeRequest(url, parameters);
    }
    std::cerr << "URL: [" << http_response.url << "]\n";
    std::cerr << "Received an http error code: " << http_response.status_code << "\n";
    std::cerr << http_response.text << "\n";
    return {{}, {Error::HTTP_ERROR, std::to_string(http_response.status_code), 0}};
}

RequestsManager::Handling RequestsManager::HandleVkError(const rapidjson::Value& json, Error* error_object) {
    if (!util::JsonToObject(json, error_object)) {
        error_object->status = Error::PARSE_ERROR;
        return Handling::BREAK;
    }
    if (error_object->status == Error::VK_ERROR) {
        if (error_object->vk_error_code == VK_ERRORS::TOO_FAST_QUERIES) {
            intervals_manager_.IncreaseInterval();
            return Handling::RETRY;
        }
    }
    return Handling::PASS;
}

}
