#include <utils/json-exceptions.h>

namespace util {
namespace json {

Exception::Exception(const std::string& message) : ::util::BasicException(message) {
}

TypeException::TypeException(const std::string& type)
        : Exception("json value is of a wrong type, expected: " + type),
          type_(type) {
}

std::string TypeException::GetType() const {
    return type_;
}

NoFieldException::NoFieldException(const std::string& field)
        : Exception("no field '" + field + "' in a json object"),
          field_(field) {
}

std::string NoFieldException::GetField() const {
    return field_;
}

NotAnObjectException::NotAnObjectException()
        : TypeException("object") {
}

NotAnArrayException::NotAnArrayException()
        : TypeException("array") {
}

TypeStringException::TypeStringException()
        : TypeException("string") {
}

TypeInt64Exception::TypeInt64Exception()
        : TypeException("int64") {
}

TypeUInt64Exception::TypeUInt64Exception()
        : TypeException("uint64") {
}

}
}
