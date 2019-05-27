#ifndef CONNECTCONTAINERWATCHER_H
#define CONNECTCONTAINERWATCHER_H

#include <functional>
#include <map>
#include <memory>
#include "communication/channel.h"
#include "interfaces/changewatcher.h"
#include "primitives/connectionholder.h"

class AbstractMessageMarshaller;
class MessageDespatcher;
class AbstractChannelAdapter;

/**
 * Класс используется для создания/удаления/изменения каналов, если пользователь
 * обновил для них информацию (добавил/удалил).
 */
class ConnectContainerWatcher : public ChangeWatcher<ConnectionHolder> {
 public:
  ConnectContainerWatcher(
      const std::shared_ptr<MessageDespatcher>& dispatcher,
      const std::function<std::unique_ptr<AbstractChannelAdapter>(
          const ConnectionHolder&)>& fabric,
      const std::shared_ptr<AbstractMessageMarshaller>& marshaller,
      const std::shared_ptr<Channel::EventQueue>& eventQueue);

 public:
  virtual void added(const ConnectionHolder& obj) override;
  virtual void changed(const ConnectionHolder& obj) override;
  virtual void removed(const ConnectionHolder& obj) override;

 private:
  std::shared_ptr<MessageDespatcher> mDespatcher;
  std::function<std::unique_ptr<AbstractChannelAdapter>(
      const ConnectionHolder&)>
      mFabric;
  std::shared_ptr<AbstractMessageMarshaller> mMarshaller;
  std::shared_ptr<Channel::EventQueue> mEventQueue;
  std::map<std::string, ConnectionHolder> mCachedData;
};
#endif  // CONNECTCONTAINERWATCHER_H
