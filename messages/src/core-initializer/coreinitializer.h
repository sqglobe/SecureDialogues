#ifndef COREINITIALIZER_H
#define COREINITIALIZER_H

#include <memory>
#include "containers/storages.h"
#include "interfaces/abstractcoreinitializer.h"
#include "pluginscontainer.h"

// gui implementations
class AbstractUserNotifier;

// containers
class MessageContainer;

// messages core
class MessageDespatcher;
class MessageActionHandler;
class DialogActionHandler;
class CryptoSystemImpl;
class AsymetricalKeyStore;
class Decryptor;

class Encryptor;
class SymetricalCipher;
class EventQueueHolder;
class AbstractDespatchErrorsSink;

class CoreInitializer : public AbstractCoreInitializer {
 public:
  CoreInitializer(
      const std::string& pass,
      std::shared_ptr<const plugin_support::PluginsContainer> container);

 public:
  void init(const std::shared_ptr<AbstractUserNotifier>& notifier) override;

  // getters for core objects
 public:
  std::shared_ptr<MessageActionHandler> getMessageActionHandler()
      const override;
  std::shared_ptr<DialogActionHandler> getDialogActionHandler() const override;
  std::shared_ptr<ConnectionStorage> getConnectionStorage() const override;
  std::shared_ptr<DialogStorage> getDialogStorage() const override;
  std::shared_ptr<MessageContainer> getMessageContainer() const override;
  std::shared_ptr<ContactStorage> getContactStorage() const override;
  std::shared_ptr<CryptoSystemImpl> getCryptoSystem() const override;

  void startMessagesHandling(
      const std::shared_ptr<AbstractUserNotifier>& notifier,
      const std::shared_ptr<Channel::EventQueue>& eventQueue) override;
  void saveFiles() override;
  virtual std::shared_ptr<DiscoverContactAgent> getDiscoverContactAgent()
      const override;
  virtual std::shared_ptr<DiscoveredContactStorage>
  getDiscoveredContactStorage() const override;

 private:
  void initDatabases(const std::string& pass);

 private:
  std::shared_ptr<ConnectionStorage> mConnectionStorage;
  std::shared_ptr<ContactStorage> mContactStorage;
  std::shared_ptr<DialogStorage> mDialogStorage;
  std::shared_ptr<MessageContainer> mMessageContainer;
  std::shared_ptr<AsymetricalKeyStore> mAsymetricalKeyStore;

  std::shared_ptr<CryptoSystemImpl> mCryptoSystem;
  std::shared_ptr<MessageDespatcher> mMessageDispatcher;
  std::shared_ptr<MessageActionHandler> mMessageActionHandler;
  std::shared_ptr<DialogActionHandler> mDialogActionHandler;
  std::shared_ptr<const SymetricalCipher> mPassCipher;
  std::shared_ptr<DiscoveredContactStorage> mDiscoveredContactStorage;
  std::shared_ptr<DiscoverContactAgent> mDiscoverContactAgent;
  std::shared_ptr<const plugin_support::PluginsContainer> mPluginContainer;
};

#endif  // COREINITIALIZER_H
