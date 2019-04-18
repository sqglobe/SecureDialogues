#include "usermessage.h"

UserMessage::UserMessage(UserMessage::Status status,
                         UserMessage::Type type,
                         const std::string& content) :
    mStatus(status),
    mType(type), mContent(content) {}

UserMessage::UserMessage(UserMessage::Type type, const std::string& content) :
    UserMessage(UserMessage::Status::WAIT_DELIVERY, type, content) {}

void UserMessage::updateStatus(UserMessage::Status status) noexcept {
  mStatus = status;
}

UserMessage::Status UserMessage::status() const noexcept {
  return mStatus;
}

UserMessage::Type UserMessage::type() const noexcept {
  return mType;
}

std::string UserMessage::content() const {
  return mContent;
}
