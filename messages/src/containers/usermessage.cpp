#include "usermessage.h"

UserMessage::UserMessage(UserMessage::Status status,
                         UserMessage::Type type,
                         std::string content) :
    mStatus(status),
    mType(type), mContent(std::move(content)) {}

UserMessage::UserMessage(UserMessage::Type type, std::string content) :
    UserMessage(UserMessage::Status::WaitDelivery, type, content) {}

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
