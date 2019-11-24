#include "queuedactionschanneladapter.h"
#include <condition_variable>
#include <mutex>

#include <iostream>

class QueuedActionsChannelAdapter::Sender {
 public:
  void sendToApplication(const MessageData& data);
  std::pair<std::string, std::string> recieve();

 private:
  std::optional<MessageData> mData;
  std::condition_variable mVar;
  std::mutex mMutex;
};

void QueuedActionsChannelAdapter::Sender::sendToApplication(
    const MessageData& data) {
  [[maybe_unused]] std::unique_lock<std::mutex> lock(mMutex);
  mData = data;
  mVar.notify_all();
}

std::pair<std::string, std::string>
QueuedActionsChannelAdapter::Sender::recieve() {
  std::unique_lock<std::mutex> lock(mMutex);
  if (mData) {
    auto res = std::make_pair(mData->address, mData->message);
    mData.reset();
    return res;
  }
  mVar.wait_for(lock, std::chrono::milliseconds(1000));
  if (!mData) {
    return std::make_pair(std::string(), std::string());
  }
  auto res = std::make_pair(mData->address, mData->message);
  mData.reset();
  return res;
}

class QueuedActionsChannelAdapter::Reciever {
 public:
  std::optional<MessageData> recieveFromApplication(int milliseconds);
  void send(const std::string& message, const std::string& address);

 private:
  std::optional<MessageData> mData;
  std::condition_variable mVar;
  std::mutex mMutex;
};

std::optional<QueuedActionsChannelAdapter::MessageData>
QueuedActionsChannelAdapter::Reciever::recieveFromApplication(
    int milliseconds) {
  std::unique_lock<std::mutex> lock(mMutex);
  if (mData.has_value()) {
    std::optional<MessageData> copy = mData;
    mData.reset();
    return copy;
  }
  std::chrono::time_point<std::chrono::system_clock> waitPoint =
      std::chrono::system_clock::now() +
      std::chrono::milliseconds(milliseconds);

  while (std::cv_status::timeout != mVar.wait_until(lock, waitPoint)) {
    if (mData.has_value()) {
      std::optional<MessageData> copy = mData;
      mData.reset();
      return copy;
    }
  }
  return {};
}

void QueuedActionsChannelAdapter::Reciever::send(const std::string& message,
                                                 const std::string& address) {
  std::unique_lock<std::mutex> lock(mMutex);
  mData = {address, message};
  mVar.notify_all();
}

QueuedActionsChannelAdapter::QueuedActionsChannelAdapter(
    std::shared_ptr<AbstractUserNotifier> notifier,
    ConnectionHolder conn) :
    AbstractChannelAdapter(std::move(notifier), std::move(conn)),
    mSender(std::make_unique<Sender>()),
    mReciever(std::make_unique<Reciever>()) {}

void QueuedActionsChannelAdapter::send(const std::string& message,
                                       const std::string& adress) {
  mReciever->send(message, adress);
}

std::pair<std::string, std::string> QueuedActionsChannelAdapter::receive() {
  return mSender->recieve();
}

bool QueuedActionsChannelAdapter::connect() {
  return true;
}

std::optional<QueuedActionsChannelAdapter::MessageData>
QueuedActionsChannelAdapter::recieveMessageFromApplication(int milliseconds) {
  return mReciever->recieveFromApplication(milliseconds);
}

void QueuedActionsChannelAdapter::sendMessageToApplication(
    const QueuedActionsChannelAdapter::MessageData& data) {
  mSender->sendToApplication(data);
}
