#include <vk-api/data-types.h>

namespace util {

template<>
void JsonToObject<vk_api::VkError>(const rapidjson::Value& json, vk_api::VkError* error) {
    JsonGetMembers(json,
                   "error_code", &error->error_code,
                   "error_msg", &error->error_msg);
    if (error->error_code == vk_api::VK_ERRORS::CAPTURE_REQUIRED) {
        if (json.HasMember("captcha_sid") && json.HasMember("captcha_img")) {
            std::string sid, img;
            JsonGetMembers(json,
                           "captcha_sid", &sid,
                           "captcha_img", &img);
            error->additional_data = {{"captcha_sid", sid},
                                      {"captcha_img", img}};
        }
    }
}

}
