#include <vk-api/data-types.h>

namespace util {

template<>
bool JsonToObject<vk_api::Error>(const rapidjson::Value& json, vk_api::Error* error) {
    if (!json.IsObject()) {
        std::cerr << "Json value is not an object\n";
        return false;
    }
    bool res = JsonGetMember(json, "error_code", &error->vk_error_code);
    if (!res) {
        std::cerr << "Can't extract an error object from a json value\n";
    } else {
        error->status = vk_api::Error::VK_ERROR;
    }
    return res;
}

}
