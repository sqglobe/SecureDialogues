#include "connectcontainerwatcher.h"
#include <algorithm>
#include <iterator>
#include "communication/messagedespatcher.h"
#include "exception/disconectedexception.h"
#include "exception/notauthorizedexception.h"
#include "interfaces/abstractchanneladapter.h"

std::chrono::seconds waitAck(ConnectionType type) {
  switch (type) {
    case ConnectionType::VK:
      return std::chrono::seconds(10);
    case ConnectionType::UDP:
      return std::chrono::seconds(10);
    case ConnectionType::GMAIL:
      return std::chrono::seconds(30);
    case ConnectionType::EMAIL:
      return std::chrono::seconds(360);
    default:
      assert(false);
  }
}

bool isImportantChanges(const ConnectionHolder& newValue,
                        const ConnectionHolder& oldValue) {
  if (oldValue.getType() != newValue.getType())
    return true;

  if (auto type = newValue.getType(); type == ConnectionType::UDP) {
    return false;
  } else if (type == ConnectionType::VK) {
    auto oldC = oldValue.getConnection<VkConnection>(),
         newC = newValue.getConnection<VkConnection>();
    return oldC.userId != newC.userId || oldC.accessToken != newC.accessToken;
  } else if (type == ConnectionType::GMAIL) {
    auto oldC = oldValue.getConnection<GmailConnection>(),
         newC = newValue.getConnection<GmailConnection>();
    return oldC.email != newC.email || oldC.accessToken != newC.accessToken;
  } else if (type == ConnectionType::EMAIL) {
    auto oldC = oldValue.getConnection<EmailConnection>(),
         newC = newValue.getConnection<EmailConnection>();
    return oldC.from != newC.from || oldC.tlsUsed != newC.tlsUsed ||
           oldC.imapAddr != newC.imapAddr || oldC.imapPort != newC.imapPort ||
           oldC.password != newC.password || oldC.smtpAddr != newC.smtpAddr ||
           oldC.smtpPort != newC.smtpPort || oldC.userName != newC.userName;
  }
  return false;
}

ConnectContainerWatcher::ConnectContainerWatcher(
    const std::shared_ptr<MessageDespatcher>& dispatcher,
    const std::function<std::unique_ptr<AbstractChannelAdapter>(
        const ConnectionHolder&)>& fabric,
    const std::shared_ptr<AbstractMessageMarshaller>& marshaller,
    const std::shared_ptr<Channel::EventQueue>& eventQueue) :
    mDespatcher(dispatcher),
    mFabric(fabric), mMarshaller(marshaller), mEventQueue(eventQueue) {}

void ConnectContainerWatcher::added(const ConnectionHolder& obj) {
  try {
    mDespatcher->add(
        std::make_shared<Channel>(mFabric(obj), mDespatcher, mMarshaller,
                                  obj.getConnectionName(), mEventQueue,
                                  waitAck(obj.getType())),
        obj.getConnectionName());
    mCachedData.emplace(obj.getConnectionName(), obj);
  } catch (const DisconectedException& ex) {
    mEventQueue->enqueue(Channel::ChannelStatus::FAILED_CONNECT,
                         obj.getConnectionName(), std::string(ex.what()));
  } catch (const NotAuthorizedException& ex) {
    mEventQueue->enqueue(Channel::ChannelStatus::AUTHORIZATION_FAILED,
                         obj.getConnectionName(), std::string(ex.what()));
  }
}

void ConnectContainerWatcher::changed(const ConnectionHolder& obj) {
  if (mCachedData.count(obj.getConnectionName()) == 0 ||
      isImportantChanges(obj, mCachedData.at(obj.getConnectionName()))) {
    try {
      mDespatcher->removeChannel(obj.getConnectionName());
      mDespatcher->add(
          std::make_shared<Channel>(mFabric(obj), mDespatcher, mMarshaller,
                                    obj.getConnectionName(), mEventQueue,
                                    waitAck(obj.getType())),
          obj.getConnectionName());
      mCachedData.insert_or_assign(obj.getConnectionName(), obj);
    } catch (const DisconectedException& ex) {
      mEventQueue->enqueue(Channel::ChannelStatus::FAILED_CONNECT,
                           obj.getConnectionName(), std::string(ex.what()));
    } catch (const NotAuthorizedException& ex) {
      mEventQueue->enqueue(Channel::ChannelStatus::AUTHORIZATION_FAILED,
                           obj.getConnectionName(), std::string(ex.what()));
    }
  }
}

void ConnectContainerWatcher::removed(const ConnectionHolder& obj) {
  mDespatcher->removeChannel(obj.getConnectionName());
  mCachedData.erase(obj.getConnectionName());
}
