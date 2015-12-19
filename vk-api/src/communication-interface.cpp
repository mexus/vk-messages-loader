#include <vk-api/communication-interface.h>
#include <exception>
#include <sstream>
#include <vector>
#include <iostream>

#include <cpr.h>
#include <manager/exceptions.h>
#include <vk-api/data-types.h>
#include <vk-api/exceptions.h>

namespace vk_api {

CommunicationInterface::CommunicationInterface(Callbacks* callbacks)
        : callbacks_(callbacks) {
}

RequestsManager::Response CommunicationInterface::SendRequest(const std::string& interface, const std::string& method,
                                                              const cpr::Parameters& params) {
    static const size_t max_attempts = 3;
    size_t attempt = 0;
    rapidjson::Document doc;
    while (true) {
        ++attempt;
        cpr::Parameters local_params = params;
        cpr::Url url = GetUrl(interface + "." + method, &local_params);
        try {
            auto reply = requests_manager_.MakeRequest(url, local_params);
            doc = std::move(reply);
        } catch (const ApiException& e) {
            auto& error = e.GetVkError();
            if (attempt == max_attempts || !HandleVkError(error)) {
                throw ;
            }
            LOG(INFO) << "Trying to recover after an API error #" << error.error_code << ", "
                      << "attempt " << attempt << " out of " << max_attempts;
            continue;
        }
        if (!doc.HasMember("response")) {
            if (attempt == max_attempts) {
                LOG(ERROR) << "Max attempts exceeded";
                THROW_AT(util::json::NoFieldException, "response");
            }
            continue;
        }
        break ;
    }
    return doc;
}

std::string CommunicationInterface::GetUrl(const std::string& method_name, cpr::Parameters* params) {
    static const std::string
        address("https://api.vk.com/method/"),
        version("5.40");
    std::string access_token = callbacks_->GetToken();
    cpr::Url url = address + method_name;
    params->AddParameter({"access_token", access_token});
    params->AddParameter({"v", version});
    params->AddParameter({"test_mode", "1"});
    if (!captcha_data_.id.empty()) {
        params->AddParameter({"captcha_sid", captcha_data_.id});
        params->AddParameter({"captcha_key", captcha_data_.key});
        captcha_data_ = {};
    }
    return url;
}

bool CommunicationInterface::HandleVkError(const VkError& vk_error) {
    switch (vk_error.error_code) {
        case VK_ERRORS::TOO_FAST_QUERIES:
            requests_manager_.IncreaseInterval();
            return true;
        case VK_ERRORS::CAPTURE_REQUIRED:
            HandleCaptcha(vk_error.additional_data);
            return true;
        default:
            return false;
    }
}

void CommunicationInterface::HandleCaptcha(const std::map<std::string, std::string>& data) {
    auto end = data.end();
    auto identificator_it = data.find("captcha_sid");
    auto img_it = data.find("captcha_img");

    bool no_id = identificator_it == end;
    bool no_image = img_it == end;
    if (no_id || no_image) {
        THROW_AT(CaptchaException, no_id, no_image);
    }
    auto& id = identificator_it->second;
    auto& image = img_it->second;
    captcha_data_.id = id;
    captcha_data_.key = callbacks_->GetCaptcha(id, image);
}

}
