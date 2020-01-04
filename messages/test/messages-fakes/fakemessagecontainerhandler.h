#ifndef FAKEMESSAGECONTAINERHANDLER_H
#define FAKEMESSAGECONTAINERHANDLER_H
#include <iostream>
#include "containers/usermessage.h"
#include "interfaces/messageconteinereventhandler.h"

class FakeMessageContainerHandlerOnlyMesssageAdded
    : public MessageContainerEventHandler {
 public:
  void messageAdded(
      const std::string& dialogId,
      const std::shared_ptr<const UserMessage>& message) override {
    mIsIncomming = message->type() == UserMessage::Type::In;
    mMessage = message->content();
    mDialogId = dialogId;
  }

  void activeDialogChanged(const std::string&) override {}

  void peekMessage(const std::shared_ptr<const UserMessage>&) override {}

  void invalidateData(const std::string&) override {}

 public:
  bool mIsIncomming;
  std::string mMessage;
  std::string mDialogId;
};

class FakeMessageContainerHandlerUpdateActiveDialog
    : public MessageContainerEventHandler {
 public:
  void messageAdded(const std::string&,
                    const std::shared_ptr<const UserMessage>&) override {}

  void activeDialogChanged(const std::string& newActiveDialogId) override {
    mActiveDialogId = newActiveDialogId;
  }
  void peekMessage(const std::shared_ptr<const UserMessage>& message) override {
    mIsIncomming = message->type() == UserMessage::Type::In;
    mMessage = message->content();
  }
  void invalidateData(const std::string&) override {}

 public:
  std::string mActiveDialogId;
  bool mIsIncomming;
  std::string mMessage;
};

#endif  // FAKEMESSAGECONTAINERHANDLER_H
