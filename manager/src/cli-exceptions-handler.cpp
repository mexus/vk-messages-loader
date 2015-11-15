#include <manager/cli-exceptions-handler.h>
#include <iostream>

void CliExceptionsHandler::Handle(const util::FileWriteException& e) {
    std::cerr << "Write operation has failed for a file " << e.GetFileName() << "\n";
}

void CliExceptionsHandler::Handle(const util::FileReadException& e) {
    std::cerr << "Read operation has failed for a file " << e.GetFileName() << "\n";
}

void CliExceptionsHandler::Handle(const util::FileException& e) {
    auto& s = std::cerr << "File operation '";
    auto operation = e.GetOperation();
    switch (operation) {
        case util::FileException::READ:
            s << "read";
            break;
        case util::FileException::WRITE:
            s << "write";
            break;
    }
    s << "' has failed for a file " << e.GetFileName() << "\n";
}

void CliExceptionsHandler::Handle(const util::json::TypeUInt64Exception& /*e*/) {
    std::cerr << "Json value is not an unsigned int 64bit\n";
}

void CliExceptionsHandler::Handle(const util::json::TypeInt64Exception& /*e*/) {
    std::cerr << "Json value is not an int 64bit\n";
}

void CliExceptionsHandler::Handle(const util::json::TypeStringException& /*e*/) {
    std::cerr << "Json value is not a string\n";
}

void CliExceptionsHandler::Handle(const util::json::NotAnArrayException& /*e*/) {
    std::cerr << "Json value is not an array\n";
}

void CliExceptionsHandler::Handle(const util::json::NotAnObjectException& /*e*/) {
    std::cerr << "Json value is not an object\n";
}

void CliExceptionsHandler::Handle(const util::json::TypeException& e) {
    std::cerr << "Wrong type for a json conversion, expected " << e.GetType() << "\n";
}

void CliExceptionsHandler::Handle(const util::json::NoFieldException& e) {
    std::cerr << "There is no field '" << e.GetField() << "' in a json object\n";
}

void CliExceptionsHandler::Handle(const util::json::Exception& e) {
    std::cerr << "Json exception: " << e.what() << "\n";
}

void CliExceptionsHandler::Handle(const storage::PathCreateException& e) {
    std::cerr << "Can't create path " << e.GetPath() << "\n";
}

void CliExceptionsHandler::Handle(const storage::PathIsFileException& e) {
    std::cerr << "Path '" << e.GetPath() << "' is a file, but should be a directory\n";
}

void CliExceptionsHandler::Handle(const storage::PathException& e) {
    std::cerr << "Storage path exception for a path '" << e.GetPath() << "': " << e.what() << "\n";
}

void CliExceptionsHandler::Handle(const vk_api::RequestParseException& e) {
    std::cerr << "Json parsing error #" << e.GetErrorCode() << " at position " << e.GetErrorOffset()
              << " while parsing a response for " << e.GetUrl() << " (http status #" << e.GetStatusCode()
              << "): " << e.GetText() << "\n";
}

void CliExceptionsHandler::Handle(const vk_api::RequestException& e) {
    std::cerr << "Vk API exception  while handling a response for " << e.GetUrl()
              << " (http status #" << e.GetStatusCode() << "): " << e.GetText() << "\n";
}

void CliExceptionsHandler::Handle(const vk_api::ApiException& e) {
    auto &error = e.GetVkError();
    auto &s = std::cerr << "Got a Vk error #" << error.error_msg << ": '" << error.error_msg<< "'\n"
                        << "Request parameters:\n";
    for (auto& parameter: error.request_params) {
        s << "\t'" << parameter.key << "' => '" << parameter.value << "'\n";
    }
    if (!error.additional_data.empty()) {
        s << "Additional data: \n";
        for (auto& pair: error.additional_data) {
            s << "\t'" << pair.first << "' => '" << pair.second << "'\n";
        }
    }
    s << "\n";
}

void CliExceptionsHandler::Handle(const util::BasicException& e) {
    std::cerr << "Received a basic application exception: " << e.what() << "\n";
}

void CliExceptionsHandler::Handle(const std::runtime_error& e) {
    std::cerr << "Received a runtime exception: " << e.what() << "\n";
}

void CliExceptionsHandler::Handle(const std::exception& e) {
    std::cerr << "Received an exception: " << e.what() << "\n";
}

void CliExceptionsHandler::Handle() {
    std::cerr << "Received an unknown exception\n";
}
