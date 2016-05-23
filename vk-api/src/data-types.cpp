#include <vk-api/data-types.h>

namespace util {

template <>
void JsonToObject<>(const rapidjson::Value& json, vk_api::User* object) {
  JsonMembers(json)("id", &object->user_id)("first_name", &object->first_name)(
      "last_name", &object->last_name);
}

template <>
rapidjson::Value JsonFromObject<>(const vk_api::User& object,
                                  JsonAllocator& allocator) {
  rapidjson::Value json(rapidjson::kObjectType);
  JsonMembers(json, allocator)("id", object.user_id)(
      "first_name", object.first_name)("last_name", object.last_name);
  return json;
}

// This function is needed to avoid a linker error
template <>
void JsonToObject<vk_api::List<vk_api::User>>(
    const rapidjson::Value& json, vk_api::List<vk_api::User>* object) {
  // This call will be resolved to a template function for vk_api::List<T>,
  // so no loops will be created
  JsonToObject<vk_api::User>(json, object);
}

template <>
void JsonToObject<>(const rapidjson::Value& json, vk_api::VkError* error) {
  JsonMembers(json)("error_code", &error->error_code)("error_msg",
                                                      &error->error_msg);
  if (error->error_code == vk_api::VK_ERRORS::CAPTURE_REQUIRED) {
    if (json.HasMember("captcha_sid") && json.HasMember("captcha_img")) {
      std::string sid, img;
      JsonMembers(json)("captcha_sid", &sid)("captcha_img", &img);
      error->additional_data = {{"captcha_sid", sid}, {"captcha_img", img}};
    }
  }
}
}
