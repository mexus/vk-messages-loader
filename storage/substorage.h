#pragma once
#include "message.h"

namespace storage {

class HistoryStorage;

class Substorage {
public:
    void AddMessage(Message&&);
    const std::vector<Message>& GetHistory() const;

private:
    Substorage(const std::string& file_name);

    const std::string file_name;
    std::vector<Message> history_;

    friend class HistoryStorage;
};

}
