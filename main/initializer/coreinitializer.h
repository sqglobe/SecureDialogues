#ifndef COREINITIALIZER_H
#define COREINITIALIZER_H

#include <memory>
#include "containers/storages.h"

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

class CoreInitializer {
 public:
  CoreInitializer(const std::shared_ptr<AbstractUserNotifier>& notifier,
                  const std::string& pass);

  // getters for core objects
 public:
  std::shared_ptr<MessageActionHandler> getMessageActionHandler() const;
  std::shared_ptr<DialogActionHandler> getDialogActionHandler() const;
  std::shared_ptr<ConnectionStorage> getConnectionStorage() const;
  std::shared_ptr<DialogStorage> getDialogStorage() const;
  std::shared_ptr<MessageContainer> getMessageContainer() const;
  std::shared_ptr<ContactStorage> getContactStorage() const;
  std::shared_ptr<CryptoSystemImpl> getCryptoSystem() const;

  void startMessagesHandling(
      const std::shared_ptr<AbstractUserNotifier>& notifier,
      const std::shared_ptr<Channel::EventQueue>& eventQueue);
  void saveFiles();

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
};

#endif  // COREINITIALIZER_H
