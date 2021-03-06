#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace storage {

enum AttachmentType : uint64_t { PHOTO, VIDEO, STICKER };

struct Attachment {
  AttachmentType type;
  std::string body;
};

struct Message {
  uint64_t message_id;
  time_t date;
  uint64_t from_user_id;
  std::string body;
  std::vector<Attachment> attachments;
};
}
