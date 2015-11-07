#pragma once
#include <iostream>
#include <memory>
#include <rapidjson/document.h>
#include <type_traits>
#include <utils/json.h>
#include "message.h"

namespace storage {

class History {
public:
    History(const std::string& file_name);
    ~History();

    void AddMessage(Message&& message);
    const std::vector<Message>& GetData() const;
    uint64_t LastMessageId() const;

private:
    const std::string file_name_;
    std::vector<Message> data_;

    void LoadData();
    void WriteData() const;

};

}
