#pragma once
#include "exceptions.h"

namespace util {
namespace json {

class Exception: public ::util::BasicException {
public:
    Exception(const std::string& message);
};

class TypeException: public Exception {
public:
    TypeException(const std::string& type);
    std::string GetType() const;

private:
    const std::string type_;
};

class NoFieldException: public Exception {
public:
    NoFieldException(const std::string& field);
    std::string GetField() const;

private:
    const std::string field_;
};

class NotAnObjectException: public TypeException {
public:
    NotAnObjectException();
};

class NotAnArrayException: public TypeException {
public:
    NotAnArrayException();
};

class TypeStringException: public TypeException {
public:
    TypeStringException();
};

class TypeInt64Exception: public TypeException {
public:
    TypeInt64Exception();
};

class TypeUInt64Exception: public TypeException {
public:
    TypeUInt64Exception();
};

}
}
