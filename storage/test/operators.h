#pragma once
#include <storage/message.h>

namespace storage {
bool operator==(const Attachment& lhs, const Attachment& rhs);
bool operator!=(const Message& lhs, const Message& rhs);
bool operator==(const Message& lhs, const Message& rhs);
}

namespace std {
ostream& operator<<(ostream& s, const storage::Message&);
}
