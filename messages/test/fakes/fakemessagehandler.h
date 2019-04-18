#ifndef FAKEMESSAGEHANDLER_H
#define FAKEMESSAGEHANDLER_H

#include "interfaces/abstractmessagehandler.h"

class FakeMessageHandler : public AbstractMessageHandler {
 public:
  FakeMessageHandler(DialogMessage::Action action, DialogMessage& message) :
      mAction(action), mMessage(message) {}
  virtual void handle(const DialogMessage& message,
                      const std::string& channel) noexcept override {
    mMessage = message;
    mChannel = channel;
  }
  virtual bool isItYouAction(DialogMessage::Action action) const
      noexcept override {
    return action == mAction;
  }

 public:
  DialogMessage::Action mAction;
  DialogMessage& mMessage;
  std::string mChannel;
};

#endif  // FAKEMESSAGEHANDLER_H
