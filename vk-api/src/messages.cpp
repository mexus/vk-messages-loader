#include <vk-api/messages.h>
#include <vk-api/data-types.h>

namespace util {

using Attachments = decltype(vk_api::Message::attachments);
using Attachment = Attachments::value_type;

template<>
void JsonToObject<Attachment>(const rapidjson::Value& json, Attachment* attachment) {
    if (!json.IsObject()) {
        THROW_AT(json::NotAnObjectException);
    }
    auto it = json.FindMember("type");
    if (it == json.MemberEnd()) {
        THROW_AT(json::NoFieldException, "type");
    }
    std::string type;
    JsonToObject(it->value, &type);
    if (type == "photo") {
        auto photo = new vk_api::PhotoAttachment();
        JsonGetMember(json, "photo", photo);
        attachment->reset(photo);
    } else if (type == "video") {
        auto video = new vk_api::VideoAttachment();
        JsonGetMember(json, "video", video);
        attachment->reset(video);
    } else if (type == "sticker") {
        auto sticker = new vk_api::StickerAttachment();
        JsonGetMember(json, "sticker", sticker);
        attachment->reset(sticker);
    } else {
        LOG(WARNING) << "Unknown type " << type;
    }
}

void ParseAttachments(const rapidjson::Value& json, Attachments* attachments) {
    if (!json.IsArray()) {
        THROW_AT(json::NotAnArrayException);
    }
    auto size = json.Size();
    for (decltype(size) i = 0; i != size; ++i) {
        auto& item = json[i];
        Attachment attachment;
        try {
            JsonToObject(item, &attachment);
            if (attachment) {
                attachments->push_back(std::move(attachment));
            }
        } catch (json::Exception& e) {
            LOG(ERROR) << "Json exception at `" << e.GetAt()
                       << "` while extracting an attachment: "
                       << e.GetMessage();
        }
    }
}

template<>
void JsonToObject<>(const rapidjson::Value& json, vk_api::Message* object) {
    JsonGetMembers(json,
                   "id", &object->id,
                   "date", &object->date,
                   "from_id", &object->from_id,
                   "user_id", &object->user_id,
                   "body", &object->body);
    auto it = json.FindMember("attachments");
    if (it != json.MemberEnd()) {
        auto& attachments = it->value;
        ParseAttachments(attachments, &object->attachments);
    }
}

// This function is needed to avoid a linker error
template<>
void JsonToObject<vk_api::List<vk_api::Message>>(const rapidjson::Value& json, vk_api::List<vk_api::Message>* object) {
    // This call will be resolved to a template function for vk_api::List<T>,
    // so no loops will be created
    JsonToObject<vk_api::Message>(json, object);
}

}

namespace vk_api {

const std::string MessageAPI::kInterfaceName = "messages";

MessageAPI::MessageAPI(CommunicationInterface* interface) : vk_interface_(interface) {
}

std::vector<Message> MessageAPI::GetMessages(uint64_t user_id, uint64_t start_message_id, uint64_t total_count) {
    static const unsigned max_messages_per_request = 200;
    std::vector<Message> total_messages;
    cpr::Parameters const_params {
            {"user_id", std::to_string(user_id)}
        };
    uint64_t last_message_id = start_message_id;
    while (true) {
        auto params = const_params;
        unsigned count = max_messages_per_request;
        if (total_count != 0) {
            count = std::min<uint64_t>(total_count - total_messages.size(), max_messages_per_request);
        }
        bool chronological = false;
        params.AddParameter({"count", std::to_string(count)});
        if (last_message_id != 0) {
            params.AddParameter({"start_message_id", std::to_string(last_message_id)});
            params.AddParameter({"offset", std::to_string(-count)});
        } else {
            params.AddParameter({"rev", "1"});
            params.AddParameter({"offset", "0"});
            chronological = true;
        }
        rapidjson::Document doc;
        try {
            auto reply = vk_interface_->SendRequest(kInterfaceName, "getHistory", std::move(params));
            doc = std::move(reply);
        } catch (const util::BasicException& e) {
            LOG(ERROR) << "Caught an exception during a request: " << e.what();
            break ;
        }

        List<Message> response;
        try {
            util::JsonGetMember(doc, "response", &response);
        } catch (util::json::Exception& e) {
            LOG(ERROR) << "Unable to convert a response to a list of messages: " << e.what();
            break ;
        }
        size_t received = response.items.size();
        if (total_count == 0) {
            total_count = response.count;
        }

        if (chronological) {
            total_messages.insert(total_messages.end(),
                                  std::make_move_iterator(response.items.begin()),
                                  std::make_move_iterator(response.items.end()));
        } else {
            total_messages.insert(total_messages.end(),
                                  std::make_move_iterator(response.items.rbegin()),
                                  std::make_move_iterator(response.items.rend()));
        }
        if (received == 0 || total_messages.size() >= total_count || total_messages.size() >= response.count)
            break;
        last_message_id = total_messages.back().id;
    }
    return total_messages;
}

}
