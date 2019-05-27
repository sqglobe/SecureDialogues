#include "connectcontainerwatcher.h"
#include <algorithm>
#include <iterator>
#include "communication/messagedespatcher.h"
#include "interfaces/abstractchanneladapter.h"

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
    auto oldC = oldValue.getConnection<GmailConnaection>(),
         newC = newValue.getConnection<GmailConnaection>();
    return oldC.email != newC.email || oldC.accessToken != newC.accessToken;
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
  mDespatcher->add(
      std::make_shared<Channel>(mFabric(obj), mDespatcher, mMarshaller,
                                obj.getConnectionName(), mEventQueue),
      obj.getConnectionName());
  mCachedData.emplace(obj.getConnectionName(), obj);
}

void ConnectContainerWatcher::changed(const ConnectionHolder& obj) {
  if (mCachedData.count(obj.getConnectionName()) == 0 ||
      isImportantChanges(obj, mCachedData.at(obj.getConnectionName()))) {
    mDespatcher->removeChannel(obj.getConnectionName());
    mDespatcher->add(
        std::make_shared<Channel>(mFabric(obj), mDespatcher, mMarshaller,
                                  obj.getConnectionName(), mEventQueue),
        obj.getConnectionName());
    mCachedData.insert_or_assign(obj.getConnectionName(), obj);
  }
}

void ConnectContainerWatcher::removed(const ConnectionHolder& obj) {
  mDespatcher->removeChannel(obj.getConnectionName());
  mCachedData.erase(obj.getConnectionName());
}
