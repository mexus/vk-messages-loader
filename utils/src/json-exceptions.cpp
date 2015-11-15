#include <utils/json-exceptions.h>

namespace util {
namespace json {

Exception::Exception(const std::string& at, const std::string& message)
        : BasicException(at, message) {
}

TypeException::TypeException(const std::string& at, const std::string& type)
        : Exception(at, "json value is of a wrong type, expected: " + type),
          type_(type) {
}

std::string TypeException::GetType() const {
    return type_;
}

NoFieldException::NoFieldException(const std::string& at, const std::string& field)
        : Exception(at, "no field '" + field + "' in a json object"),
          field_(field) {
}

std::string NoFieldException::GetField() const {
    return field_;
}

NotAnObjectException::NotAnObjectException(const std::string& at)
        : TypeException(at, "object") {
}

NotAnArrayException::NotAnArrayException(const std::string& at)
        : TypeException(at, "array") {
}

TypeStringException::TypeStringException(const std::string& at)
        : TypeException(at, "string") {
}

TypeInt64Exception::TypeInt64Exception(const std::string& at)
        : TypeException(at, "int64") {
}

TypeUInt64Exception::TypeUInt64Exception(const std::string& at)
        : TypeException(at, "uint64") {
}

}
}
