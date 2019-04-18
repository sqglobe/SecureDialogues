#ifndef FAKEMESSAGECONTAINERHANDLER_H
#define FAKEMESSAGECONTAINERHANDLER_H
#include <iostream>
#include "containers/usermessage.h"
#include "interfaces/messageconteinereventhandler.h"

class FakeMessageContainerHandlerOnlyMesssageAdded
    : public MessageContainerEventHandler {
 public:
  virtual void messageAdded(
      const std::string& dialogId,
      const std::shared_ptr<const UserMessage>& message) override {
    mIsIncomming = message->type() == UserMessage::Type::IN;
    mMessage = message->content();
    mDialogId = dialogId;
  }

  virtual void activeDialogChanged([
      [maybe_unused]] const std::string& newActiveDialogId) override {}

  virtual void peekMessage([
      [maybe_unused]] const std::shared_ptr<const UserMessage>& message)
      override {}

  virtual void invalidateData([
      [maybe_unused]] const std::string& dialogId) override {}

 public:
  bool mIsIncomming;
  std::string mMessage;
  std::string mDialogId;
};

class FakeMessageContainerHandlerUpdateActiveDialog
    : public MessageContainerEventHandler {
 public:
  virtual void messageAdded(
      [[maybe_unused]] const std::string& dialogId,
      [[maybe_unused]] const std::shared_ptr<const UserMessage>& message)
      override {}

  virtual void activeDialogChanged(
      const std::string& newActiveDialogId) override {
    mActiveDialogId = newActiveDialogId;
  }
  virtual void peekMessage(
      const std::shared_ptr<const UserMessage>& message) override {
    mIsIncomming = message->type() == UserMessage::Type::IN;
    mMessage = message->content();
  }
  virtual void invalidateData([
      [maybe_unused]] const std::string& dialogId) override {}

 public:
  std::string mActiveDialogId;
  bool mIsIncomming;
  std::string mMessage;
};

#endif  // FAKEMESSAGECONTAINERHANDLER_H
