#include "connectstoragelistener.h"
#include <algorithm>
#include <iterator>
#include "communication/messagedespatcher.h"
#include "exception/disconectedexception.h"
#include "exception/notauthorizedexception.h"

#include "interfaces/abstractchanneladapter.h"

std::chrono::seconds waitAck(ConnectionType type) {
  switch (type) {
    case ConnectionType::VK:
      return std::chrono::seconds(30);
    case ConnectionType::UDP:
      return std::chrono::seconds(30);
    case ConnectionType::GMAIL:
      return std::chrono::seconds(40);
    case ConnectionType::EMAIL:
      return std::chrono::seconds(360);
    default:
      assert(false);
  }
}

ConnectStorageListener::ConnectStorageListener(
    std::shared_ptr<MessageDespatcher> dispatcher,
    std::function<std::unique_ptr<AbstractChannelAdapter>(
        const ConnectionHolder&)> fabric,
    std::shared_ptr<AbstractMessageMarshaller> marshaller,
    std::shared_ptr<Channel::EventQueue> eventQueue,
    const std::vector<ConnectionHolder>& elements) :
    mDespatcher(std::move(dispatcher)),
    mFabric(std::move(fabric)), mMarshaller(std::move(marshaller)),
    mEventQueue(std::move(eventQueue)) {
  std::for_each(
      std::cbegin(elements), std::cend(elements),
      [this](const ConnectionHolder& holder) mutable { this->added(holder); });
}

void ConnectStorageListener::added(const ChangeListener::element& obj) {
  try {
    mDespatcher->add(
        std::make_shared<Channel>(mFabric(obj), mDespatcher, mMarshaller,
                                  obj.getConnectionName(), mEventQueue,
                                  waitAck(obj.getType())),
        obj.getConnectionName());
  } catch (const DisconectedException& ex) {
    mEventQueue->enqueue(Channel::ChannelStatus::FAILED_CONNECT,
                         obj.getConnectionName(), std::string(ex.what()));
  } catch (const NotAuthorizedException& ex) {
    mEventQueue->enqueue(Channel::ChannelStatus::AUTHORIZATION_FAILED,
                         obj.getConnectionName(), std::string(ex.what()));
  }
}

void ConnectStorageListener::changed(const ChangeListener::element& obj) {
  try {
    mDespatcher->removeChannel(obj.getConnectionName());
    mDespatcher->add(
        std::make_shared<Channel>(mFabric(obj), mDespatcher, mMarshaller,
                                  obj.getConnectionName(), mEventQueue,
                                  waitAck(obj.getType())),
        obj.getConnectionName());
  } catch (const DisconectedException& ex) {
    mEventQueue->enqueue(Channel::ChannelStatus::FAILED_CONNECT,
                         obj.getConnectionName(), std::string(ex.what()));
  } catch (const NotAuthorizedException& ex) {
    mEventQueue->enqueue(Channel::ChannelStatus::AUTHORIZATION_FAILED,
                         obj.getConnectionName(), std::string(ex.what()));
  }
}

void ConnectStorageListener::removed(const ChangeListener::element& obj) {
  mDespatcher->removeChannel(obj.getConnectionName());
}
