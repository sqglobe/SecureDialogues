#include "dialoginfo.h"

#include "primitives/contact.h"

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

std::string DialogInfo::contactId() const {
  return mContactId;
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

DialogInfo::DialogInfo(const Dialog& elem, const Contact& contact) :
    mName(contact.name()), mAddress(contact.adress()),
    mMoniker(contact.channelMoniker()), mDialogId(elem.getDialogId()),
    mContactId(contact.id()), mStatus(elem.getStatus()),
    mLastUpdated(std::chrono::system_clock::now()) {}

DialogInfo& DialogInfo::operator=(const DialogInfo& info) {
  this->mAddress = info.mAddress;
  this->mDialogId = info.mDialogId;
  this->mMoniker = info.mMoniker;
  this->mName = info.mName;
  this->mStatus = info.mStatus;
  this->mUnreadMessages = info.mUnreadMessages;
  this->mContactId = info.mContactId;
  this->mLastUpdated = std::chrono::system_clock::now();
  return *this;
}

DialogInfo& DialogInfo::operator=(const Dialog& info) {
  this->mStatus = info.getStatus();
  this->mDialogId = info.getDialogId();
  this->mContactId = info.getContactId();
  this->mLastUpdated = std::chrono::system_clock::now();
  return *this;
}

DialogInfo& DialogInfo::operator=(const Contact& info) {
  this->mAddress = info.adress();
  this->mMoniker = info.channelMoniker();
  this->mName = info.name();
  this->mContactId = info.id();
  return *this;
}

void DialogInfo::messagesReaded() {
  mUnreadMessages = 0;
}

void DialogInfo::addUnreadMessage() {
  mUnreadMessages++;
  this->mLastUpdated = std::chrono::system_clock::now();
}
