#include <vk-api/messages.h>
#include <vk-api/data-types.h>

namespace util {

template<>
bool JsonToObject<vk_api::MessageAPI::Message>(const rapidjson::Value& json, vk_api::MessageAPI::Message* object) {
    bool res = JsonGetMembers(json,
                              "id", &object->id,
                              "date", &object->date,
                              "from_id", &object->from_id,
                              "user_id", &object->user_id,
                              "body", &object->body);
    if (!res) {
        std::cerr << "Can't extract a message from a json value\n";
    }
    return res;
}

// This function is needed to avoid a linker error
template<>
bool JsonToObject<vk_api::List<vk_api::MessageAPI::Message>>(const rapidjson::Value& json, vk_api::List<vk_api::MessageAPI::Message>* object) {
    // This call will be resolved to a template function for vk_api::List<T>,
    // so no loops will be created
    return JsonToObject<vk_api::MessageAPI::Message>(json, object);
}

}

namespace vk_api {

const std::string MessageAPI::kInterfaceName = "messages";

MessageAPI::MessageAPI(CommunicationInterface* interface) : vk_interface_(interface) {
}

std::vector<MessageAPI::Message> MessageAPI::GetMessages(uint64_t user_id, uint64_t start_message_id, uint64_t total_count) {
    static const unsigned max_messages_per_request = 200;
    std::vector<MessageAPI::Message> total_messages;
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
        RequestsManager::Response reply = vk_interface_->SendRequest(kInterfaceName, "getHistory", std::move(params));
        if (reply.error.status != Error::OK) {
            std::cerr << "Can't process with a response due to error(s)\n";
            break ;
        }

        List<Message> response;
        bool res = util::JsonGetMember(reply.json, "response", &response);
        if (!res) {
            std::cerr << "Unable to convert response to a list of messages\n";
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
