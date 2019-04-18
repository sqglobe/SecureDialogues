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

std::shared_ptr<spdlog::logger> Channel::LOGGER =
    spdlog::stdout_color_mt("channel");

Channel::Channel(std::unique_ptr<AbstractChannelAdapter>&& adater,
                 const std::shared_ptr<AbstractMessageDespatcher>& handler,
                 const std::shared_ptr<AbstractMessageMarshaller>& marshaller,
                 const std::string& name) :
    mAdapter(std::move(adater)),
    mHandler(handler), mMarshaller(marshaller), mName(name) {}

Channel::Channel(AbstractChannelAdapter* adapter,
                 const std::shared_ptr<AbstractMessageDespatcher>& handler,
                 const std::shared_ptr<AbstractMessageMarshaller>& marshaller,
                 const std::string& name) :
    Channel(std::unique_ptr<AbstractChannelAdapter>(adapter),
            handler,
            marshaller,
            name) {}

Channel::~Channel() {
  mIsEnds = true;
}

void Channel::messsageCycle(std::weak_ptr<Channel>&& shared) {
  std::weak_ptr<Channel> weak(shared);
  while (!mIsEnds) {
    auto th = weak.lock();
    if (!th || !th->mAdapter)
      break;
    try {
      auto msg = th->mAdapter->receive();

      if (msg.first.empty() || msg.second.empty())
        continue;

      if (auto dialogMessage = mMarshaller->unmarshall(msg.second, msg.first)) {
        if (auto hLock = th->mHandler.lock()) {
          try {
            hLock->dispatch(dialogMessage.value(), mName);
          } catch (std::exception& ex) {
            LOGGER->error(
                "Get Exception {0} when dispatch message {1} for chanel {2} "
                "from {3}",
                ex.what(), msg.first, th->mName, msg.first);
          }
        }
      }
    } catch (const SendMessageException&) {  // TODO: необходимо обрабатывать и
                                             // другие исключения
      checkConnectionProcess(weak);
    } catch (const std::exception& ex) {
      LOGGER->debug("Get exception when recieve message: {0}", ex.what());
    }
  }
}

void Channel::sendMessage(const DialogMessage& message) {
  mAdapter->send(mMarshaller->marshall(message), message.adress());
}

void Channel::startCycle() {
  std::thread th(&Channel::messsageCycle, this,
                 std::weak_ptr<Channel>(shared_from_this()));
  th.detach();
}

void Channel::checkConnectionProcess(const std::weak_ptr<Channel>& channel) {
  auto timeout = std::chrono::seconds(2);
  static const auto MAX_TIMEOUT = std::chrono::minutes(10);

  while (1) {
    auto ch = channel.lock();
    if (!ch || ch->mAdapter->isConnected()) {
      return;
    }
    std::this_thread::sleep_for(timeout);
    timeout = timeout * 10;
    if (MAX_TIMEOUT < timeout) {
      timeout = MAX_TIMEOUT;
    }
  }
}
