#include <ctime>
#include <iostream>
#include <vk-api/communication-interface.h>

std::string GetToken(const std::string& auto_url);

typedef bool(*ResponseProcessor)(const rapidjson::Value& response);
int ProcessReply(std::unique_ptr<rapidjson::Document>&& document, ResponseProcessor processor);

int GetFriends(vk_api::CommunicationInterface* iface);
bool ProcessUsers(const rapidjson::Value& response);
bool ProcessUser(const rapidjson::Value& user);

int GetMessages(vk_api::CommunicationInterface* iface);
bool ProcessMessages(const rapidjson::Value& response);
bool ProcessMessage(const rapidjson::Value& message);

int ProcessReply(std::unique_ptr<rapidjson::Document>&& document, ResponseProcessor processor) {
    auto end = document->MemberEnd();
    auto response_it = document->FindMember("response");
    if (response_it == end) {
        auto error_it = document->FindMember("error");
        if (error_it == end) {
            std::cerr << "Can't find neither 'response' nor 'error' fields\n";
            return 1;
        }
        // Error handling
        return 2;
    }
    return processor(response_it->value) ? 0 : 3;
}


std::string GetToken(const std::string& auto_url) {
    std::cout << "Visit " << auto_url << " and paste URL to which you will be redirected here\n";
    std::string url;
    std::cin >> url;
    std::cout << "Processing [" << url << "]\n";

    static const std::string needle = "access_token=";
    size_t begin = url.find(needle);
    if (begin == std::string::npos) {
        std::cerr << "Can't find 'access_token'\n";
        return "";
    }
    begin += needle.length();
    size_t end = url.find('&', begin);
    if (end == std::string::npos)
        end = url.length();
    std::string token = url.substr(begin, end - begin);
    std::cout << "Token is: [" << token << "]\n";
    return token;
}

bool ProcessUsers(const rapidjson::Value& response) {
    if (!response.IsObject()) {
        std::cerr << "Response is not an object\n";
        return false;
    }
    auto end = response.MemberEnd();
    auto count_it = response.FindMember("count");
    if (count_it == end) {
        std::cerr << "Can't extract friends count\n";
        return false;
    } else if (!count_it->value.IsUint()) {
        std::cerr << "Friends count is not unsigned integer\n";
        return false;
    }
    unsigned count = count_it->value.GetUint();
    std::cout << "Total " << count << " friends\n";
    auto items_it = response.FindMember("items");
    if (items_it == end) {
        std::cerr << "Can't obtain users information\n";
        return false;
    } else if (!items_it->value.IsArray()) {
        std::cerr << "Friends list is not an array\n";
        return false;
    }

    auto items_size = items_it->value.Size();
    for (decltype(items_size) i = 0; i != items_size; ++i) {
        auto &user = items_it->value[i];
        if (!ProcessUser(user)) {
            return false;
        }
    }
    return true;
}

bool ProcessUser(const rapidjson::Value& user) {
    if (!user.IsObject()) {
        std::cerr << "'friend' is not an object\n";
        return false;
    }
    auto end = user.MemberEnd();
    auto user_id_it = user.FindMember("id");
    if (user_id_it == end) {
        std::cerr << "Can't find id field in friend's description\n";
        return false;
    } else if (!user_id_it->value.IsUint()) {
        std::cerr << "User id is not an unsigned int\n";
        return false;
    }
    unsigned user_id = user_id_it->value.GetUint();

    auto first_name_it = user.FindMember("first_name");
    if (first_name_it == end) {
        std::cerr << "Can't find 'first_name' field\n";
        return false;
    } else if (!first_name_it->value.IsString()) {
        std::cerr << "First name is not a string\n";
        return false;
    }
    std::string first_name = first_name_it->value.GetString();

    auto last_name_it = user.FindMember("last_name");
    if (last_name_it == end) {
        std::cerr << "Can't find 'last_name' field\n";
        return false;
    } else if (!last_name_it->value.IsString()) {
        std::cerr << "Last name is not a string\n";
        return false;
    }
    std::string last_name = last_name_it->value.GetString();

    std::cout << "User #" << user_id << ": " << first_name << " " << last_name << "\n";

    return true;
}

int GetFriends(vk_api::CommunicationInterface* iface) {
    cpr::Parameters params;
    params.AddParameter({"count", "5"});
    params.AddParameter({"offset", "0"});
    params.AddParameter({"fields", "uid,first_name,last_name"});
    params.AddParameter({"order", "hints"});
    auto reply = iface->SendRequest("friends", "get", std::move(params));
    if (!reply) {
        std::cerr << "Can't get a response\n";
        return 1;
    }
    return ProcessReply(std::move(reply), ProcessUsers);
}

bool ProcessMessages(const rapidjson::Value& response) {
    if (!response.IsObject()) {
        std::cerr << "Response is not an object\n";
        return false;
    }
    auto end = response.MemberEnd();
    auto count_it = response.FindMember("count");
    if (count_it == end) {
        std::cerr << "Can't extract message count\n";
        return false;
    } else if (!count_it->value.IsUint()) {
        std::cerr << "Message count is not unsigned integer\n";
        return false;
    }
    unsigned count = count_it->value.GetUint();
    std::cout << "Total " << count << " messages\n";
    auto items_it = response.FindMember("items");
    if (items_it == end) {
        std::cerr << "Can't obtain messages information\n";
        return false;
    } else if (!items_it->value.IsArray()) {
        std::cerr << "Messages list is not an array\n";
        return false;
    }

    auto items_size = items_it->value.Size();
    for (decltype(items_size) i = 0; i != items_size; ++i) {
        auto &message = items_it->value[i];
        if (!ProcessMessage(message)) {
            return false;
        }
    }
    return true;
}

bool ProcessMessage(const rapidjson::Value& message) {
    if (!message.IsObject()) {
        std::cerr << "Message is not an object\n";
        return false;
    }

    auto end = message.MemberEnd();
    auto date_it = message.FindMember("date");
    if (date_it == end) {
        std::cerr << "Can't find 'date' member\n";
        return false;
    } else if (!date_it->value.IsInt64()) {
        std::cerr << "Date is not int64\n";
        return false;
    }
    std::time_t date = date_it->value.GetInt64();

    auto from_id_it = message.FindMember("from_id");
    if (from_id_it == end) {
        std::cerr << "Can't find 'from_id' member\n";
        return false;
    } else if (!from_id_it->value.IsUint()) {
        std::cerr << "'from_id' is not an unsigned int";
        return false;
    }
    unsigned from_id = from_id_it->value.GetUint();

    auto to_id_it = message.FindMember("user_id");
    if (to_id_it == end) {
        std::cerr << "Can't find 'user_id' member\n";
        return false;
    } else if (!to_id_it->value.IsUint()) {
        std::cerr << "'user_id' is not an unsigned int";
        return false;
    }
    unsigned to_id = to_id_it->value.GetUint();

    auto body_it = message.FindMember("body");
    if (body_it == end) {
        std::cerr << "Can't find body\n";
        return false;
    } else if (!body_it->value.IsString()) {
        std::cerr << "Body is not a string\n";
        return false;
    }
    std::string body = body_it->value.GetString();

    std::cout << "At " << date << " from #" << from_id << " to #" << to_id << ": " << body << "\n";

    return true;
}

int GetMessages(vk_api::CommunicationInterface* iface) {
    std::cout << "Enter user id to retreive a message history: ";
    unsigned user_id;
    std::cin >> user_id;
    cpr::Parameters params;
    params.AddParameter({"offset", "0"});
    params.AddParameter({"count", "5"});
    params.AddParameter({"user_id", std::to_string(user_id)});
    params.AddParameter({"rev", "0"});
    auto reply = iface->SendRequest("messages", "getHistory", std::move(params));
    if (!reply) {
        std::cerr << "Can't get a response\n";
        return 1;
    }
    return ProcessReply(std::move(reply), ProcessMessages);
}

int main() {

    vk_api::IntervalsManager intervals_manager;
    vk_api::RequestsManager requests_manager(&intervals_manager);

    std::string client_id;
    std::cout << "Enter application id (client id): ";
    std::cin >> client_id;
    vk_api::CommunicationInterface iface(client_id, GetToken, &requests_manager);

    return GetFriends(&iface) | GetMessages(&iface);
}
