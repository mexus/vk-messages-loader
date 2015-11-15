#pragma once
#include <utils/exceptions-handling.h>

class CliExceptionsHandler: public util::ExceptionsHandler {
    void Handle(const util::FileWriteException& e) override;
    void Handle(const util::FileReadException& e) override;
    void Handle(const util::FileException& e) override;
    void Handle(const util::json::TypeUInt64Exception& e) override;
    void Handle(const util::json::TypeInt64Exception& e) override;
    void Handle(const util::json::TypeStringException& e) override;
    void Handle(const util::json::NotAnArrayException& e) override;
    void Handle(const util::json::NotAnObjectException& e) override;
    void Handle(const util::json::TypeException& e) override;
    void Handle(const util::json::NoFieldException& e) override;
    void Handle(const util::json::Exception& e) override;
    void Handle(const storage::PathCreateException& e) override;
    void Handle(const storage::PathIsFileException& e) override;
    void Handle(const storage::PathException& e) override;
    void Handle(const vk_api::RequestParseException& e) override;
    void Handle(const vk_api::RequestException& e) override;
    void Handle(const vk_api::ApiException& e) override;
    void Handle(const manager::NoApplicationIdException& e) override;
    void Handle(const manager::NoTokenException& e) override;
    void Handle(const manager::cache::NoDataException& e) override;
    void Handle(const util::BasicException& e) override;
    void Handle(const std::runtime_error& e) override;
    void Handle(const std::exception& e) override;
    void Handle() override; // Handle unknown exception
};
