#include <algorithm>
#include <iterator>
#include <utility>

#include <cassert>
#include "channel.h"
#include "constants.h"
#include "delivery-handlers/deliveryhandler.h"
#include "interfaces/abstractchanneladapter.h"
#include "interfaces/abstractmessagehandler.h"
#include "interfaces/abstractusernotifier.h"
#include "interfaces/cryptosystem.h"
#include "messagedespatcher.h"
#include "utils.h"
#include "utils/timeoutedrepository.h"

#include "spdlog/spdlog.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <iostream>
#include "interfaces/abstractdespatcherrorssink.h"

MessageDespatcher::MessageDespatcher(
    std::shared_ptr<const CryptoSystem> cryptoSystem,
    std::shared_ptr<AbstractUserNotifier> notifier,
    std::shared_ptr<AbstractDespatchErrorsSink> errorSink) :
    mCryptoSystem(std::move(cryptoSystem)),
    mNotifier(std::move(notifier)), mErrorSink(errorSink),
    mRepo(TimeoutedRrepository<std::shared_ptr<DeliveryHandler>,
                               std::pair<std::string, unsigned long> >::
              make(std::chrono::seconds(WAIT_ACK),
                   std::chrono::milliseconds(CHECK_IDLE_ACK))) {}

void MessageDespatcher::dispatch(DialogMessage&& message,
                                 const std::string& channelName) noexcept {
  if (DialogMessage::Action::ACK == message.action()) {
    if (!mRepo->remove(std::make_pair(std::string(message.dialogId()),
                                      message.sequential()))) {
      spdlog::get("root_logger")
          ->warn(
              "Get message from {0} with action {1} sequental {2}. Not found "
              "seq "
              "in repo",
              message.adress(), static_cast<int>(message.action()),
              message.sequential());
    }
  } else {
    sendAck(message, channelName);
    if (!isSignatureValid(message, channelName)) {
      mErrorSink->error(DespatchError::SignatureBroken, message, channelName);
    } else if (get_timestamp() - message.timestamp() > MAX_TIMESTAMP_DIFF) {
      mErrorSink->error(DespatchError::MessageExpired, message, channelName);
    } else {
      std::shared_lock<std::shared_mutex> guard(mMutex);
      for (auto& handler : mHandlers) {
        if (handler->isItYouAction(message.action())) {
          handler->handle(message, channelName);
        }
      }
    }
  }
}

void MessageDespatcher::sendMessage(
    DialogMessage&& message,
    std::string_view channelName,
    std::shared_ptr<DeliveryHandler>&& deliveryHandler) const {
  std::shared_lock<std::shared_mutex> guard(mMutex);

  auto iter = mChannels.find(channelName);

  if (iter == mChannels.end()) {
    spdlog::get("root_logger")->warn("Not found channel {0}", channelName);
    return;
  }

  message.setSignature(mCryptoSystem->createSignature(message));
  mRepo->store(
      std::move(deliveryHandler),
      std::make_pair(std::string(message.dialogId()), message.sequential()),
      iter->second->getWaitAckInterval());
  iter->second->sendMessage(std::move(message));
}

void MessageDespatcher::sendAndForget(DialogMessage&& message,
                                      std::string_view channelName) const {
  std::shared_lock<std::shared_mutex> guard(mMutex);
  message.setSignature(mCryptoSystem->createSignature(message));
  if (auto iter = mChannels.find(channelName); iter != mChannels.end()) {
    iter->second->sendMessage(std::move(message));
  }
}

void MessageDespatcher::add(
    const std::shared_ptr<AbstractMessageHandler>& handler) {
  std::unique_lock<std::shared_mutex> guard(mMutex);
  mHandlers.push_back(handler);
}

void MessageDespatcher::add(AbstractMessageHandler* handler) {
  add(std::shared_ptr<AbstractMessageHandler>(handler));
}

void MessageDespatcher::add(std::unique_ptr<Channel>&& channel,
                            const std::string& channelName) {
  add(std::shared_ptr<Channel>(std::move(channel)), channelName);
}

void MessageDespatcher::add(const std::shared_ptr<Channel>& channel,
                            const std::string& channelName) {
  std::unique_lock<std::shared_mutex> guard(mMutex);
  auto it = mChannels.emplace(channelName, channel);
  assert(it.second);
  it.first->second->startCycle();
}

void MessageDespatcher::add(Channel* channel, const std::string& channelName) {
  add(std::shared_ptr<Channel>(channel), channelName);
}

std::vector<std::string> MessageDespatcher::getChannelsNames() const {
  std::shared_lock<std::shared_mutex> guard(mMutex);
  std::vector<std::string> res;
  res.reserve(mChannels.size());
  for (const auto& el : mChannels) {
    res.push_back(el.first);
  }
  return res;
}

void MessageDespatcher::sendAck(const DialogMessage& message,
                                const std::string& channel) {
  if (message.action() != DialogMessage::Action::ABORT &&
      message.sequential() > 0 && mChannels.count(channel) > 0) {
    sendAndForget(make_ack(message), channel);
  }
}

bool MessageDespatcher::isSignatureValid(const DialogMessage& message,
                                         const std::string& channelName) const
    noexcept {
  try {
    if (mCryptoSystem->isSignatureOk(message)) {
      return true;
    }
  } catch (std::range_error&) {
    mErrorSink->error(DespatchError::ContactNotFound, message, channelName);
  } catch (std::exception&) {
    mErrorSink->error(DespatchError::ExceptionThrown, message, channelName);
  }
  return false;
}

void MessageDespatcher::removeChannel(const std::string& channel) {
  std::unique_lock<std::shared_mutex> guard(mMutex);

  mChannels.erase(channel);
}
