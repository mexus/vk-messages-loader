#include <vk-api/messages.h>
#include "attachment-helper.h"

namespace util {

using PhotoAttachment = vk_api::PhotoAttachment;

template<>
void JsonToObject<PhotoAttachment>(const rapidjson::Value& json, PhotoAttachment* photo) {
    if (!json.IsObject()) {
        THROW_AT(json::NotAnObjectException);
    }
    bool res = vk_api::FirstFieldToObject(json,
                                  {"photo_2560", "photo_1280", "photo_807",
                                   "photo_604", "photo_130", "photo_75"},
                                   &photo->url);
    if (!res) {
        THROW_AT(json::NoFieldException, "photo_*");
    }
}

}
