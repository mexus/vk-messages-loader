#include <ctime>
#include <iostream>
#include <vk-api/communication-interface.h>
#include <vk-api/friends.h>
#include <vk-api/messages.h>

std::string GetToken(const std::string& auto_url);
int GetFriends(vk_api::CommunicationInterface* iface);
int GetMessages(vk_api::CommunicationInterface* iface);

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

int GetFriends(vk_api::CommunicationInterface* iface) {
    vk_api::FriendsAPI friends_api(iface);
    auto friends = friends_api.GetFriends();
    if (friends.empty()) {
        return 1;
    }
    for (auto& user: friends) {
        std::cout << "User id #" << user.user_id << ": " << user.first_name << ", " << user.last_name << "\n";
    }
    return 0;
}

int GetMessages(vk_api::CommunicationInterface* iface) {
    std::cout << "Enter user id to retreive a message history: ";
    unsigned user_id;
    std::cin >> user_id;
    vk_api::MessageAPI messages_api(iface);
    auto messages = messages_api.GetMessages(user_id, 0, 100);
    if (messages.empty()) {
        return 1;
    }
    for (auto& message: messages) {
        std::cout << "At " << message.date << " id #" << message.id << " "
                  << "from #" << message.from_id << " to #" << message.user_id
                  << ": " << message.body << "\n";
    }
    return 0;
}

int main() {
    std::string client_id;
    std::cout << "Enter application id (client id): ";
    std::cin >> client_id;
    vk_api::CommunicationInterface iface(client_id, GetToken);

    return GetFriends(&iface) | GetMessages(&iface);
    //return GetMessages(&iface);
}
