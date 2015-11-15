#include <vk-api/messages.h>
#include "attachment-helper.h"

namespace util {

using VideoAttachment = vk_api::VideoAttachment;

template<>
void JsonToObject<VideoAttachment>(const rapidjson::Value& json, VideoAttachment* video) {
    if (!json.IsObject()) {
        THROW_AT(json::NotAnObjectException);
    }
    bool res = vk_api::FirstFieldToObject(json,
                                  {"photo_640", "photo_320", "photo_130"},
                                   &video->preview_url);
    if (!res) {
        THROW_AT(json::NoFieldException, "photo_*");
    }
    JsonGetMembers(json,
                   "title", &video->title,
                   "description", &video->description);
}

}