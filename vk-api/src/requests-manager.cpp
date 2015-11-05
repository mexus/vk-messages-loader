#include <vk-api/requests-manager.h>
#include <iostream>
#include <cpr.h>

namespace {
enum HTTP_CODES {
    OK = 200
};
}

namespace vk_api {

RequestsManager::RequestsManager(IntervalsManager* intervals_manager)
        : intervals_manager_(intervals_manager) {
}

RequestsManager::Response RequestsManager::MakeRequest(const cpr::Url& url, const cpr::Parameters& parameters) {
    intervals_manager_->Wait();
    auto http_response = cpr::Get(url, parameters);
    if (http_response.status_code == HTTP_CODES::OK) {
        auto json = http_response.text;
        std::unique_ptr<rapidjson::Document> doc(new rapidjson::Document());
        doc->Parse(json.c_str());
        if (doc->HasParseError()) {
            std::cerr << "URL: [" << http_response.url << "]\n";
            std::cerr << "Received a reply:\n" << json << "\n";
            std::cerr << "Can't parse response, parse error #" << doc->GetParseError()
                      << "at" << doc->GetErrorOffset() << ":\n" << json << "\n";
            return {};
        }
        if (!doc->IsObject()) {
            std::cerr << "URL: [" << http_response.url << "]\n";
            std::cerr << "Received a reply:\n" << json << "\n";
            std::cerr << "Received json is not an object\n";
            return {};
        }
        return doc;
    }
    std::cerr << "Received an http error code: " << http_response.status_code << "\n";
    std::cerr << "URL: [" << http_response.url << "]\n";
    std::cerr << http_response.text << "\n\n";
    return {};
}

}
