#pragma once

#include <string>

namespace vk_api {

enum AttachmentKind {
  PHOTO,
  VIDEO,
  STICKER,
  // TODO: implement other attachments
  // AUDIO,
  // DOC,
  // WALL,
  // WALL_REPLY,
};

struct Attachment {
  virtual ~Attachment() = default;
  virtual AttachmentKind GetKind() const = 0;
};

struct PhotoAttachment : public Attachment {
  AttachmentKind GetKind() const override { return PHOTO; }
  std::string url;
};

struct VideoAttachment : public Attachment {
  AttachmentKind GetKind() const override { return VIDEO; }
  std::string preview_url;
  std::string title;
  std::string description;
};

struct StickerAttachment : public Attachment {
  AttachmentKind GetKind() const override { return STICKER; }
  std::string url;
};
}
