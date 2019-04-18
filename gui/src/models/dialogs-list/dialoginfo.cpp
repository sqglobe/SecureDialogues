#include "dialoginfo.h"

std::string DialogInfo::name() const {
  return mName;
}

std::string DialogInfo::address() const {
  return mAddress;
}

std::string DialogInfo::moniker() const {
  return mMoniker;
}

std::string DialogInfo::dialogId() const {
  return mDialogId;
}

Dialog::Status DialogInfo::status() const {
  return mStatus;
}

std::size_t DialogInfo::unreadMessages() const {
  return mUnreadMessages;
}

std::chrono::system_clock::time_point DialogInfo::lastUpdated() const {
  return mLastUpdated;
}

DialogInfo::DialogInfo(const std::shared_ptr<const Dialog>& elem) :
    mName(elem->getName()), mAddress(elem->getAdress()),
    mMoniker(elem->getChannelMoniker()), mDialogId(elem->getDialogId()),
    mStatus(elem->getStatus()), mLastUpdated(std::chrono::system_clock::now()) {
}

DialogInfo& DialogInfo::operator=(const DialogInfo& info) {
  this->mAddress = info.mAddress;
  this->mDialogId = info.mDialogId;
  this->mMoniker = info.mMoniker;
  this->mName = info.mName;
  this->mStatus = info.mStatus;
  this->mUnreadMessages = info.mUnreadMessages;
  this->mLastUpdated = std::chrono::system_clock::now();
  return *this;
}

DialogInfo& DialogInfo::operator=(const std::shared_ptr<const Dialog>& info) {
  this->mAddress = info->getAdress();
  this->mDialogId = info->getDialogId();
  this->mMoniker = info->getChannelMoniker();
  this->mName = info->getName();
  this->mStatus = info->getStatus();
  this->mUnreadMessages = 0;
  this->mLastUpdated = std::chrono::system_clock::now();
  return *this;
}

DialogInfo::DialogInfo() {}

void DialogInfo::messagesReaded() {
  mUnreadMessages = 0;
}

void DialogInfo::addUnreadMessage() {
  mUnreadMessages++;
  this->mLastUpdated = std::chrono::system_clock::now();
}
