#ifndef DIALOGINFO_H
#define DIALOGINFO_H

#include <chrono>
#include <memory>
#include <string>
#include "primitives/dialog.h"

class DialogInfo {
  std::string mName;
  std::string mAddress;
  std::string mMoniker;
  std::string mDialogId;
  Dialog::Status mStatus = Dialog::Status::NEW;
  std::size_t mUnreadMessages = 0;
  std::chrono::system_clock::time_point mLastUpdated;

 public:
  explicit DialogInfo(const std::shared_ptr<const Dialog>& elem);
  DialogInfo& operator=(const DialogInfo& info);
  DialogInfo& operator=(const std::shared_ptr<const Dialog>& info);
  DialogInfo() = default;

 public:
  void messagesReaded();
  void addUnreadMessage();

 public:
  std::string name() const;
  std::string address() const;
  std::string moniker() const;
  std::string dialogId() const;
  Dialog::Status status() const;
  std::size_t unreadMessages() const;
  std::chrono::system_clock::time_point lastUpdated() const;
};

#endif  // DIALOGINFO_H
