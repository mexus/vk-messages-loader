#include "operators.h"
#include <iostream>

namespace storage {

bool operator==(const Attachment& lhs, const Attachment& rhs) {
  return lhs.type == rhs.type && lhs.body == rhs.body;
}

bool operator!=(const Message& lhs, const Message& rhs) {
  return !(lhs == rhs);
}

bool operator==(const Message& lhs, const Message& rhs) {
  return lhs.from_user_id == rhs.from_user_id &&
         lhs.attachments == rhs.attachments && lhs.date == rhs.date &&
         lhs.to_user_id == rhs.to_user_id && lhs.message_id == rhs.message_id &&
         lhs.body == rhs.body;
}
}

namespace std {
ostream& operator<<(ostream& s, const storage::Attachment& attachment) {
  s << "\t\ttype: " << attachment.type << "\n";
  s << "\t\tbody: " << attachment.body << "\n";
  return s;
}

ostream& operator<<(ostream& s, const storage::Message& message) {
  s << "message_id: " << message.message_id << "\n";
  s << "date: " << message.date << "\n";
  s << "from_user_id: " << message.from_user_id << "\n";
  s << "to_user_id: " << message.to_user_id << "\n";
  s << "body: " << message.body << "\n";
  s << "attachments: {\n";
  for (auto& attachment : message.attachments) {
    s << "\t{\n" << attachment << "\t}\n";
  }
  s << "}\n";
  return s;
}
}
