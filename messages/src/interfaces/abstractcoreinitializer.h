#ifndef ABSTRACTCOREINITIALIZER_H
#define ABSTRACTCOREINITIALIZER_H

#include <memory>
#include "communication/channel.h"
#include "containers/storages.h"

class AbstractUserNotifier;
class CryptoSystemImpl;
class DialogActionHandler;
class MessageActionHandler;
class MessageContainer;
class DiscoverContactAgent;

class AbstractCoreInitializer {
 public:
  virtual ~AbstractCoreInitializer() = default;

 public:
  virtual void init(const std::shared_ptr<AbstractUserNotifier>& notifier) = 0;
  virtual void saveFiles() = 0;
  virtual void startMessagesHandling(
      const std::shared_ptr<AbstractUserNotifier>& notifier,
      const std::shared_ptr<Channel::EventQueue>& eventQueue) = 0;

 public:
  virtual std::shared_ptr<ConnectionStorage> getConnectionStorage() const = 0;
  virtual std::shared_ptr<ContactStorage> getContactStorage() const = 0;
  virtual std::shared_ptr<CryptoSystemImpl> getCryptoSystem() const = 0;
  virtual std::shared_ptr<DialogActionHandler> getDialogActionHandler()
      const = 0;
  virtual std::shared_ptr<DialogStorage> getDialogStorage() const = 0;
  virtual std::shared_ptr<MessageActionHandler> getMessageActionHandler()
      const = 0;
  virtual std::shared_ptr<MessageContainer> getMessageContainer() const = 0;
  virtual std::shared_ptr<DiscoverContactAgent> getDiscoverContactAgent()
      const = 0;
  virtual std::shared_ptr<DiscoveredContactStorage>
  getDiscoveredContactStorage() const = 0;
};

#endif  // ABSTRACTCOREINITIALIZER_H
