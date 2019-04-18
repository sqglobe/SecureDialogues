#include <algorithm>
#include <iterator>
#include <utility>

#include "assert.h"
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

std::shared_ptr<spdlog::logger> MessageDespatcher::LOGGER =
    spdlog::stdout_color_mt("message-despatcher");

MessageDespatcher::MessageDespatcher(
    const std::shared_ptr<const CryptoSystem>& cryptoSystem,
    const std::shared_ptr<AbstractUserNotifier>& notifier) :
    mCryptoSystem(cryptoSystem),
    mNotifier(notifier),
    mRepo(
        TimeoutedRrepository<std::shared_ptr<DeliveryHandler>, unsigned long>::
            make(std::chrono::seconds(WAIT_ACK),
                 std::chrono::milliseconds(CHECK_IDLE_ACK))) {}

void MessageDespatcher::dispatch(const DialogMessage& message,
                                 const std::string& channelName) noexcept {
  if (DialogMessage::Action::ACK == message.action()) {
    if (!mRepo->remove(message.sequential())) {
      LOGGER->warn(
          "Get message from {0} with action {1} sequental {2}. Not found seq "
          "in repo",
          message.adress(), static_cast<int>(message.action()),
          message.sequential());
    }
  } else {
    sendAck(message, channelName);
    if (!isSignatureValid(message)) {
      LOGGER->error(
          "Get invalid message signature for message {0} for address {1} "
          "channel {2}",
          message.content(), message.adress(), channelName);
    } else if (get_timestamp() - message.timestamp() > MAX_TIMESTAMP_DIFF) {
      LOGGER->error("Get old message {0} for address {1} channel {2}",
                    message.content(), message.adress(), channelName);
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
    const DialogMessage& message,
    const std::string& channelName,
    const std::shared_ptr<DeliveryHandler>& deliveryHandler) const {
  DialogMessage mess(message.action(), message.content(), message.dialogId(),
                     message.adress(), getNextSequential());
  mess.setSignature(mCryptoSystem->createSignature(mess));
  mRepo->store(deliveryHandler, mess.sequential());
  {
    std::shared_lock<std::shared_mutex> guard(mMutex);
    mChannels.at(channelName)->sendMessage(mess);
  }
}

void MessageDespatcher::sendAndForget(const DialogMessage& message,
                                      const std::string& channelName) const {
  std::shared_lock<std::shared_mutex> guard(mMutex);
  mChannels.at(channelName)->sendMessage(message);
}

void MessageDespatcher::add(std::shared_ptr<AbstractMessageHandler> handler) {
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

unsigned long MessageDespatcher::getNextSequential() const noexcept {
  return ++mSequentialNumber;
}

void MessageDespatcher::sendAck(const DialogMessage& message,
                                const std::string& channel) {
  if (message.action() != DialogMessage::Action::ABORT &&
      message.sequential() > 0) {
    auto ack = make_ack(message);
    LOGGER->debug(
        "send ack for dialog {0} adress {1} channel {2} sequental {3}",
        message.dialogId(), message.adress(), channel, message.sequential());
    sendAndForget(ack, channel);
  }
}

bool MessageDespatcher::isSignatureValid(const DialogMessage& message) const
    noexcept {
  try {
    if (mCryptoSystem->isSignatureOk(message)) {
      return true;
    } else {
      LOGGER->error(
          "Подпись для адреса {0} не верна. Возможно публичный ключ изменился "
          "или это является следствием атаки",
          message.adress());
    }
  } catch (std::range_error&) {
    LOGGER->error("Не найден публичный ключ для пользователя {0} ",
                  message.adress());
  } catch (std::exception& ex) {
    LOGGER->error("при проверки подписи сообщения от {0} произошла ошибка {1}",
                  message.adress(), ex.what());
  }
  return false;
}

void MessageDespatcher::removeChannel(const std::string& channel) {
  std::unique_lock<std::shared_mutex> guard(mMutex);
  mChannels.erase(channel);
}
