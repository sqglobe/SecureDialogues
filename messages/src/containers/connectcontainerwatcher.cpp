#include "connectcontainerwatcher.h"
#include <algorithm>
#include <iterator>
#include "communication/messagedespatcher.h"
#include "interfaces/abstractchanneladapter.h"

ConnectContainerWatcher::ConnectContainerWatcher(
    const std::shared_ptr<MessageDespatcher>& dispatcher,
    const std::function<std::unique_ptr<AbstractChannelAdapter>(
        const ConnectionHolder&)>& fabric,
    const std::shared_ptr<AbstractMessageMarshaller>& marshaller,
    const std::shared_ptr<Channel::EventQueue>& eventQueue) :
    mDespatcher(dispatcher),
    mFabric(fabric), mMarshaller(marshaller), mEventQueue(eventQueue) {}

void ConnectContainerWatcher::added(const ConnectionHolder& obj) {
  mDespatcher->add(
      std::make_shared<Channel>(mFabric(obj), mDespatcher, mMarshaller,
                                obj.getConnectionName(), mEventQueue),
      obj.getConnectionName());
}

void ConnectContainerWatcher::changed(const ConnectionHolder& obj) {
  mDespatcher->removeChannel(obj.getConnectionName());
  mDespatcher->add(
      std::make_shared<Channel>(mFabric(obj), mDespatcher, mMarshaller,
                                obj.getConnectionName(), mEventQueue),
      obj.getConnectionName());
}

void ConnectContainerWatcher::removed(const ConnectionHolder& obj) {
  mDespatcher->removeChannel(obj.getConnectionName());
}
