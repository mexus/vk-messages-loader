#include <storage/history.h>

namespace storage {

History::History(const std::string& file_name) : file_name_(file_name) {
    LoadData();
}

History::~History() {
    WriteData();
}

void History::AddMessage(Message&& message) {
    data_.push_back(std::move(message));
}

const std::vector<Message>& History::GetData() const {
    return data_;
}

void History::LoadData() {
    try {
        rapidjson::Document document = util::JsonFromFile(file_name_);
        util::JsonGetMember(document, "data", &data_);
    } catch (const util::FileReadException& e) {
        std::cout << "Can't read a history file: " << e.GetFileName() << "\n";
    } catch (const util::json::Exception& e) {
        std::cerr << "Caught a json exception while reading a history file '" << file_name_<< "': "
                  << e.what() << ". The history file will be overwritten on save.\n";
    }
}

void History::WriteData() const {
    rapidjson::Document document;
    document.SetObject();
    auto& allocator = document.GetAllocator();
    util::JsonAddMember(&document, "data", data_, allocator);
    util::JsonToFile(file_name_, document);
}

uint64_t History::LastMessageId() const {
    if (data_.empty()) {
        return 0;
    }
    return data_.back().message_id;
}

}
