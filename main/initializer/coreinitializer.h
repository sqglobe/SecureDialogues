#ifndef COREINITIALIZER_H
#define COREINITIALIZER_H

#include <memory>

// gui implementations
class AbstractUserNotifier;

// containers
class ConnectionInfoContainer;
class ContactContainer;
class DialogManager;
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
                  const std::string& pass,
                  const EventQueueHolder& eventHolder);

  // getters for core objects
 public:
  std::shared_ptr<MessageActionHandler> getMessageActionHandler() const;
  std::shared_ptr<DialogActionHandler> getDialogActionHandler() const;
  std::shared_ptr<ConnectionInfoContainer> getConnectionInfocontainer() const;
  std::shared_ptr<DialogManager> getDialogManager() const;
  std::shared_ptr<MessageContainer> getMessageContainer() const;
  std::shared_ptr<ContactContainer> getContactContainer() const;
  std::shared_ptr<CryptoSystemImpl> getCryptoSystem() const;

  void saveFiles();

 private:
  void loadFromFiles(const std::string& connFile,
                     const std::string& contactFile,
                     const std::shared_ptr<const Decryptor>& decryptor);
  void saveToFiles(const std::string& connFile,
                   const std::string& contactFile,
                   const std::shared_ptr<const Encryptor>& encryptor);

 private:
  std::shared_ptr<ConnectionInfoContainer> mConnectionInfoContainer;
  std::shared_ptr<ContactContainer> mContactContainer;
  std::shared_ptr<DialogManager> mDialogManager;
  std::shared_ptr<MessageContainer> mMessageContainer;
  std::shared_ptr<AsymetricalKeyStore> mAsymetricalKeyStore;

  std::shared_ptr<CryptoSystemImpl> mCryptoSystem;
  std::shared_ptr<MessageDespatcher> mMessageDispatcher;
  std::shared_ptr<MessageActionHandler> mMessageActionHandler;
  std::shared_ptr<DialogActionHandler> mDialogActionHandler;
  std::shared_ptr<const SymetricalCipher> mPassCipher;
};

#endif  // COREINITIALIZER_H
