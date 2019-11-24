#ifndef DIALOGINFO_H
#define DIALOGINFO_H

#include <chrono>
#include <memory>
#include <string>
#include "primitives/dialog.h"

class Contact;

class DialogInfo {
  std::string mName;
  std::string mAddress;
  std::string mMoniker;
  std::string mDialogId;
  std::string mContactId;
  Dialog::Status mStatus = Dialog::Status::NEW;
  std::size_t mUnreadMessages = 0;
  std::chrono::system_clock::time_point mLastUpdated;

 public:
  explicit DialogInfo(const Dialog& elem, const Contact& contact);
  DialogInfo& operator=(const DialogInfo& info);
  DialogInfo& operator=(const Dialog& info);
  DialogInfo& operator=(const Contact& info);
  DialogInfo() = default;

 public:
  void messagesReaded();
  void addUnreadMessage();

 public:
  std::string name() const;
  std::string address() const;
  std::string moniker() const;
  std::string dialogId() const;
  std::string contactId() const;
  Dialog::Status status() const;
  std::size_t unreadMessages() const;
  std::chrono::system_clock::time_point lastUpdated() const;
};

#endif  // DIALOGINFO_H
