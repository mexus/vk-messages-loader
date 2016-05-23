#pragma once

#include "json-exceptions.h"
#include "json.h"

namespace util {

namespace json {
class Optional {};
}

template <class Allocator>
struct OptionalAllocator {
    Allocator& allocator_;
};

template<>
struct OptionalAllocator<void> {
};

template <class Json, class Allocator = void>
class JsonMembersWrapper : protected OptionalAllocator<Allocator> {
 public:
  template <class U = Allocator>
  JsonMembersWrapper(std::enable_if_t<std::is_void<U>::value, Json&> json)
      : json_(json) {}

  template <class U = Allocator>
  JsonMembersWrapper(std::enable_if_t<!std::is_void<U>::value, Json&> json,
                     U& allocator)
      : OptionalAllocator<Allocator>{allocator}, json_(json) {}

  const JsonMembersWrapper& operator()() const { return *this; }

  template <class T>
  const JsonMembersWrapper& operator()(const std::string& name, T* value,
                                   json::Optional /*optional*/) const {
    try {
      JsonGetMember(json_, name, value);
    } catch (const json::Exception&) {
      // Just skip it, it's optional after all
    }
    return *this;
  }

  template <class T>
  const JsonMembersWrapper& operator()(const std::string& name, T* value) const {
    JsonGetMember(json_, name, value);
    return *this;
  }

  template <class T>
  typename std::enable_if_t<!std::is_const<T>::value, const JsonMembersWrapper&>
  operator()(const std::string& name, const T& value) const {
    JsonAddMember(&json_, name, value, OptionalAllocator<Allocator>::allocator_);
    return *this;
  }

 private:
  Json& json_;
};

template<class Json>
JsonMembersWrapper<Json> JsonMembers(Json& json) {
    return JsonMembersWrapper<Json>(json);
}

template<class Json, class Allocator>
JsonMembersWrapper<Json, Allocator> JsonMembers(Json& json, Allocator& allocator) {
    return JsonMembersWrapper<Json, Allocator>(json, allocator);
}

}
