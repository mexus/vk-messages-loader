#include <vk-api/messages.h>
#include "attachment-helper.h"

namespace util {

using StickerAttachment = vk_api::StickerAttachment;

template<>
void JsonToObject<>(const rapidjson::Value& json, StickerAttachment* photo) {
    if (!json.IsObject()) {
        THROW_AT(json::NotAnObjectException);
    }
    bool res = vk_api::FirstFieldToObject(json,
                                  {"photo_352", "photo_256", "photo_128", "photo_64"},
                                   &photo->url);
    if (!res) {
        THROW_AT(json::NoFieldException, "photo_*");
    }
}

}
