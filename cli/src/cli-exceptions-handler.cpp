#include <cli/cli-exceptions-handler.h>
#include <iostream>

namespace cli {

void CliExceptionsHandler::Handle(const util::FileWriteException& e) {
  LOG(ERROR) << "At `" << e.GetAt() << "`: "
             << "Write operation has failed for a file " << e.GetFileName();
}

void CliExceptionsHandler::Handle(const util::FileReadException& e) {
  LOG(ERROR) << "At `" << e.GetAt() << "`: "
             << "Read operation has failed for a file " << e.GetFileName();
}

void CliExceptionsHandler::Handle(const util::FileException& e) {
  auto& s = LOG(ERROR) << "At `" << e.GetAt() << "`: "
                       << "File operation '";
  auto operation = e.GetOperation();
  switch (operation) {
    case util::FileException::READ:
      s << "read";
      break;
    case util::FileException::WRITE:
      s << "write";
      break;
  }
  s << "' has failed for a file " << e.GetFileName();
}

void CliExceptionsHandler::Handle(const util::json::TypeUInt64Exception& e) {
  LOG(ERROR) << "At `" << e.GetAt() << "`: "
             << "Json value is not an unsigned int 64bit";
}

void CliExceptionsHandler::Handle(const util::json::TypeInt64Exception& e) {
  LOG(ERROR) << "At `" << e.GetAt() << "`: "
             << "Json value is not an int 64bit";
}

void CliExceptionsHandler::Handle(const util::json::TypeStringException& e) {
  LOG(ERROR) << "At `" << e.GetAt() << "`: "
             << "Json value is not a string";
}

void CliExceptionsHandler::Handle(const util::json::NotAnArrayException& e) {
  LOG(ERROR) << "At `" << e.GetAt() << "`: "
             << "Json value is not an array";
}

void CliExceptionsHandler::Handle(const util::json::NotAnObjectException& e) {
  LOG(ERROR) << "At `" << e.GetAt() << "`: "
             << "Json value is not an object";
}

void CliExceptionsHandler::Handle(const util::json::TypeException& e) {
  LOG(ERROR) << "At `" << e.GetAt() << "`: "
             << "Wrong type for a json conversion, expected " << e.GetType();
}

void CliExceptionsHandler::Handle(const util::json::NoFieldException& e) {
  LOG(ERROR) << "At `" << e.GetAt() << "`: "
             << "There is no field '" << e.GetField() << "' in a json object";
}

void CliExceptionsHandler::Handle(const util::json::Exception& e) {
  LOG(ERROR) << "At `" << e.GetAt() << "`: "
             << "Json exception: " << e.what();
}

void CliExceptionsHandler::Handle(const storage::PathCreateException& e) {
  LOG(ERROR) << "At `" << e.GetAt() << "`: "
             << "Can't create path " << e.GetPath();
}

void CliExceptionsHandler::Handle(const storage::PathIsFileException& e) {
  LOG(ERROR) << "At `" << e.GetAt() << "`: "
             << "Path '" << e.GetPath()
             << "' is a file, but should be a directory";
}

void CliExceptionsHandler::Handle(const storage::PathException& e) {
  LOG(ERROR) << "At `" << e.GetAt() << "`: "
             << "Storage path exception for a path '" << e.GetPath()
             << "': " << e.what();
}

void CliExceptionsHandler::Handle(const vk_api::CaptchaException& e) {
  auto& s = LOG(ERROR) << "At `" << e.GetAt() << "`: "
                       << "Captha parameter(s) is(are) missing: ";
  if (e.NoId()) {
    s << "identificator ";
  }
  if (e.NoImage()) {
    s << "image url ";
  }
}

void CliExceptionsHandler::Handle(const vk_api::RequestParseException& e) {
  LOG(ERROR) << "At `" << e.GetAt() << "`: "
             << "Json parsing error #" << e.GetErrorCode() << " at position "
             << e.GetErrorOffset() << " while parsing a response for "
             << e.GetUrl() << " (http status #" << e.GetStatusCode()
             << "): " << e.GetText();
}

void CliExceptionsHandler::Handle(const vk_api::RequestException& e) {
  LOG(ERROR) << "At `" << e.GetAt() << "`: "
             << "Vk API exception  while handling a response for " << e.GetUrl()
             << " (http status #" << e.GetStatusCode() << "): " << e.GetText();
}

void CliExceptionsHandler::Handle(const vk_api::ApiException& e) {
  auto& error = e.GetVkError();
  auto& s = LOG(ERROR) << "At `" << e.GetAt() << "`: "
                       << "Got a Vk error #" << error.error_msg << ": '"
                       << error.error_msg << "'\n"
                       << "Request parameters:\n";
  for (auto& parameter : error.request_params) {
    s << "\t'" << parameter.key << "' => '" << parameter.value << "'\n";
  }
  if (!error.additional_data.empty()) {
    s << "Additional data:\n";
    for (auto& pair : error.additional_data) {
      s << "\t'" << pair.first << "' => '" << pair.second << "'\n";
    }
  }
}

void CliExceptionsHandler::Handle(const manager::NoApplicationIdException& e) {
  LOG(ERROR) << "At `" << e.GetAt() << "`: "
             << "No application id provided";
}

void CliExceptionsHandler::Handle(const manager::NoTokenException& e) {
  LOG(ERROR) << "At `" << e.GetAt() << "`: "
             << "No access token provided";
}

void CliExceptionsHandler::Handle(const manager::cache::NoDataException& e) {
  LOG(ERROR) << "At `" << e.GetAt() << "`: "
             << "No data in a cache for id #" << e.GetId();
}

void CliExceptionsHandler::Handle(const util::BasicException& e) {
  LOG(ERROR) << "At `" << e.GetAt() << "`: "
             << "Received a basic application exception: " << e.GetMessage();
}

void CliExceptionsHandler::Handle(const std::runtime_error& e) {
  LOG(ERROR) << "Received a runtime exception: " << e.what();
}

void CliExceptionsHandler::Handle(const std::exception& e) {
  LOG(ERROR) << "Received an exception: " << e.what();
}

void CliExceptionsHandler::Handle() {
  LOG(ERROR) << "Received an unknown exception";
}
}
