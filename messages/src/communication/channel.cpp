#include "channel.h"

#include "interfaces/abstractchanneladapter.h"
#include "interfaces/abstractmessagedespatcher.h"
#include "interfaces/abstractmessagemarshaller.h"
#include "primitives/dialogmessage.h"

#include <assert.h>
#include <thread>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <iostream>
#include "spdlog/spdlog.h"

#include "exception/disconectedexception.h"
#include "exception/notauthorizedexception.h"

Channel::Channel(std::unique_ptr<AbstractChannelAdapter>&& adater,
                 const std::shared_ptr<AbstractMessageDespatcher>& handler,
                 std::shared_ptr<AbstractMessageMarshaller> marshaller,
                 const std::string& name,
                 const std::shared_ptr<EventQueue>& eventQueue,
                 std::chrono::seconds waitAck) :
    mAdapter(std::move(adater)),
    mHandler(handler), mMarshaller(std::move(marshaller)), mName(name),
    mEventQueue(eventQueue), mWaitAckInterval(waitAck) {}

Channel::Channel(AbstractChannelAdapter* adapter,
                 const std::shared_ptr<AbstractMessageDespatcher>& handler,
                 std::shared_ptr<AbstractMessageMarshaller> marshaller,
                 const std::string& name,
                 const std::shared_ptr<EventQueue>& eventQueue,
                 std::chrono::seconds waitAck) :
    Channel(std::unique_ptr<AbstractChannelAdapter>(adapter),
            handler,
            std::move(marshaller),
            name,
            eventQueue,
            waitAck) {}

Channel::~Channel() {
  mIsEnds = true;
  try {
    if (mThread.joinable()) {
      mThread.join();
    }
  } catch (std::exception ex) {
    spdlog::get("root_logger")->error("Get exception: {0}", ex.what());
  }
}

void Channel::messsageCycle() {
  bool isDisconected{true};
  while (!mIsEnds) {
    try {
      if (isDisconected) {
        [[maybe_unused]] std::lock_guard<std::mutex> guard(mMutex);
        mAdapter->connect();
        isDisconected = false;
        mEventQueue->enqueue(ChannelStatus::CONNECTED, mName, "");
      }

      auto msg = mAdapter->receive();
      if (msg.first.empty() || msg.second.empty())
        continue;

      if (auto dialogMessage = mMarshaller->unmarshall(msg.second, msg.first)) {
        if (auto hLock = mHandler.lock()) {
          try {
            hLock->dispatch(std::move(dialogMessage).value(), mName);
          } catch (std::exception& ex) {
            spdlog::get("root_logger")
                ->error(
                    "Get Exception {0} when dispatch message {1} for chanel "
                    "{2} "
                    "from {3}",
                    ex.what(), msg.first, mName, msg.first);
          }
        }
      }

    } catch (const DisconectedException& ex) {
      mEventQueue->enqueue(ChannelStatus::FAILED_CONNECT, mName,
                           std::string(ex.what()));
      isDisconected = true;
    } catch (const NotAuthorizedException& ex) {
      mEventQueue->enqueue(ChannelStatus::AUTHORIZATION_FAILED, mName,
                           std::string(ex.what()));
      isDisconected = true;
    } catch (const std::exception& ex) {
      spdlog::get("root_logger")
          ->debug("Get exception when recieve message: {0}", ex.what());
    }
  }
}

void Channel::sendMessage(const DialogMessage& message) {
  try {
    [[maybe_unused]] std::lock_guard<std::mutex> guard(mMutex);
    mAdapter->send(mMarshaller->marshall(message),
                   std::string(message.adress()));
  } catch (const DisconectedException& ex) {
    mEventQueue->enqueue(ChannelStatus::FAILED_CONNECT, mName,
                         std::string(ex.what()));
    throw ex;
  } catch (const NotAuthorizedException& ex) {
    mEventQueue->enqueue(ChannelStatus::AUTHORIZATION_FAILED, mName,
                         std::string(ex.what()));
    throw ex;
  }
}

void Channel::startCycle() {
  mThread = std::thread(&Channel::messsageCycle, this);
}

std::chrono::seconds Channel::getWaitAckInterval() const {
  return mWaitAckInterval;
}
