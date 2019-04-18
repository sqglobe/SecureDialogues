#ifndef FAKEMESSAGEDESPATHER_H
#define FAKEMESSAGEDESPATHER_H
#include "delivery-handlers/deliveryhandler.h"
#include "interfaces/abstractmessagedespatcher.h"
#include "primitives/dialogmessage.h"

class FakeMessageDespather : public AbstractMessageDespatcher {
 public:
  FakeMessageDespather() :
      mMessage(DialogMessage::Action::ACCEPT_DIALOG, "", "") {}
  void dispatch(const DialogMessage& message,
                const std::string& channelName) noexcept override {
    mMessage = message;
    mChannelName = channelName;
  }
  virtual void sendMessage(
      [[maybe_unused]] const DialogMessage& message,
      [[maybe_unused]] const std::string& channelName,
      [[maybe_unused]] const std::shared_ptr<DeliveryHandler>& deliveryHandler)
      const override {}

  virtual void sendAndForget(
      [[maybe_unused]] const DialogMessage& message,
      [[maybe_unused]] const std::string& channelName) const override {}

 public:
  DialogMessage mMessage;
  std::string mChannelName;
};

class FakeMessageSendDespatcher : public AbstractMessageDespatcher {
 public:
  enum class Resolve { REMOVED, TIMEOUTED, NONE };

 public:
  FakeMessageSendDespatcher() :
      mMessage(DialogMessage::Action::ACCEPT_DIALOG, "", ""),
      mResolve(Resolve::NONE) {}
  FakeMessageSendDespatcher(Resolve resolve) :
      mMessage(DialogMessage::Action::ACCEPT_DIALOG, "", ""),
      mResolve(resolve) {}

  void dispatch(
      [[maybe_unused]] const DialogMessage& message,
      [[maybe_unused]] const std::string& channelName) noexcept override {}
  virtual void sendMessage(
      const DialogMessage& message,
      const std::string& channelName,
      const std::shared_ptr<DeliveryHandler>& deliveryHandler) const override {
    mMessage = message;
    mChannel = channelName;
    mSendFunction = "sendMessage";

    if (mResolve == Resolve::REMOVED) {
      deliveryHandler->removed();
    } else if (mResolve == Resolve::TIMEOUTED) {
      deliveryHandler->timeouted();
    }
  }

  virtual void sendAndForget(const DialogMessage& message,
                             const std::string& channelName) const override {
    mMessage = message;
    mChannel = channelName;
    mSendFunction = "sendAndForget";
  }

 public:
  mutable DialogMessage mMessage;
  mutable std::string mChannel;
  mutable std::string mSendFunction;
  Resolve mResolve;
};

#endif  // FAKEMESSAGEDESPATHER_H
