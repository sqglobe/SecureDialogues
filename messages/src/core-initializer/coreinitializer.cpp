#include "coreinitializer.h"
#include "interfaces/abstractusernotifier.h"

#include "containers/messagecontainer.h"

#include "communication/dialogactionhandler.h"
#include "communication/messageactionhandler.h"
#include "communication/messagedespatcher.h"

#include "interfaces/abstractchanneladapter.h"

#include "chaneladapterfactory.h"
#include "utils/messagemarshaller.h"

#include "utils/fileutils.h"

#include "utils/systemmessagegenerator.h"

#include "crypto/cryptosystemcontactupdateinformator.h"
#include "crypto/cryptosystemdialogremoveinformator.h"
#include "crypto/cryptosystemimpl.h"

#include "asymetrical/asymetricalkeystore.h"
#include "serialization/fileasynckeyserializer.h"

#include "asymetrical/asymetricalkeygenerator.h"

#include "interfaces/symetricalcipher.h"
#include "symetrical/symetricalsystemfactories.h"

#include <db_cxx.h>
#include "containers/connectstoragelistener.h"

#include "utils/dbfactory.h"

#include "communication/despatcherrorssink.h"
#include "communication/discovercontactagent.h"

static const std::string FILE_KEY = "conf/keys";

std::shared_ptr<AsymetricalKeyStore> loadKeys(
    const std::string& fileTempl,
    const std::shared_ptr<const SymetricalCipher>& cipher) {
  auto deserializer = FileAsyncKeySerializer(fileTempl, cipher);
  auto keyStore = std::make_shared<AsymetricalKeyStore>();
  try {
    deserializer.deserialize(*(keyStore.get()));
  } catch (std::exception& ex) {
    std::cout << "get exception " << ex.what() << std::endl;
    AsymetricalKeyGenerator(keyStore).generate();
  }
  return keyStore;
}

void saveKeys(const std::string& fileTempl,
              const std::shared_ptr<AsymetricalKeyStore>& keyStore,
              const std::shared_ptr<const SymetricalCipher>& cipher) {
  auto deserializer = FileAsyncKeySerializer(fileTempl, cipher);
  deserializer.serialize(*(keyStore.get()));
}

CoreInitializer::CoreInitializer(
    const std::string& pass,
    std::shared_ptr<plugin_support::PluginsContainer> container) :
    mMessageContainer(std::make_shared<MessageContainer>()),
    mPassCipher(makeForStringPass(pass)),
    mPluginContainer(std::move(container)) {
  initDatabases(pass);

  mAsymetricalKeyStore = loadKeys(FILE_KEY, mPassCipher);

  mCryptoSystem = std::make_shared<CryptoSystemImpl>(
      mAsymetricalKeyStore, std::make_shared<MessageMarshaller>());
}

void CoreInitializer::init(
    const std::shared_ptr<AbstractUserNotifier>& notifier) {
  mMessageDispatcher = std::make_shared<MessageDespatcher>(
      mCryptoSystem, notifier,
      std::make_shared<DespatchErrorsSink>(mDiscoveredContactStorage));

  mMessageActionHandler = std::make_shared<MessageActionHandler>(
      mDialogStorage, mContactStorage, mMessageContainer, mMessageDispatcher,
      notifier, mCryptoSystem);
  mDialogActionHandler = std::make_shared<DialogActionHandler>(
      mDialogStorage, mMessageDispatcher, notifier, mContactStorage,
      mCryptoSystem);

  mMessageDispatcher->add(mMessageActionHandler);
  mMessageDispatcher->add(mDialogActionHandler);

  mDiscoverContactAgent =
      std::make_shared<DiscoverContactAgent>(mCryptoSystem, mMessageDispatcher);

  mContactStorage->appendPermanentListener(
      std::make_shared<CryptoSystemContactUpdateInformator>(mCryptoSystem));

  mDialogStorage->appendPermanentListener(
      std::make_shared<SystemMessageGenerator>(mMessageContainer));
  mDialogStorage->appendPermanentListener(
      std::make_shared<CryptoSystemDialogRemoveInformator>(mCryptoSystem));
}

std::shared_ptr<MessageActionHandler> CoreInitializer::getMessageActionHandler()
    const {
  return mMessageActionHandler;
}

std::shared_ptr<DialogActionHandler> CoreInitializer::getDialogActionHandler()
    const {
  return mDialogActionHandler;
}

std::shared_ptr<ConnectionStorage> CoreInitializer::getConnectionStorage()
    const {
  return mConnectionStorage;
}

std::shared_ptr<DialogStorage> CoreInitializer::getDialogStorage() const {
  return mDialogStorage;
}

std::shared_ptr<MessageContainer> CoreInitializer::getMessageContainer() const {
  return mMessageContainer;
}

std::shared_ptr<ContactStorage> CoreInitializer::getContactStorage() const {
  return mContactStorage;
}

std::shared_ptr<CryptoSystemImpl> CoreInitializer::getCryptoSystem() const {
  return mCryptoSystem;
}

void CoreInitializer::startMessagesHandling(
    const std::shared_ptr<AbstractUserNotifier>& notifier,
    const std::shared_ptr<Channel::EventQueue>& eventQueue) {
  auto connWatcher = std::make_shared<ConnectStorageListener>(
      mMessageDispatcher,
      std::function<std::unique_ptr<AbstractChannelAdapter>(
          const ConnectionHolder&)>(ChanelAdapterFactory(notifier)),
      std::make_shared<MessageMarshaller>(), eventQueue,
      mConnectionStorage->getAllElements());
  mConnectionStorage->appendPermanentListener(connWatcher);
}

void CoreInitializer::saveFiles() {
  saveKeys(FILE_KEY, mAsymetricalKeyStore, mPassCipher);
}

std::shared_ptr<DiscoverContactAgent> CoreInitializer::getDiscoverContactAgent()
    const {
  return mDiscoverContactAgent;
}

std::shared_ptr<DiscoveredContactStorage>
CoreInitializer::getDiscoveredContactStorage() const {
  return mDiscoveredContactStorage;
}

void CoreInitializer::initDatabases(const std::string& pass) {
  auto penv = make_db_env("conf", pass);

  mDialogStorage = make_dialog_storage(
      make_db("dialogs.db", "primary", penv),
      make_db("dialogs.db", "secondary", penv, DB_DUP), penv);
  mContactStorage = make_contact_storage(
      make_db("cotnacts.db", "primary", penv),
      make_db("contacts.db", "secondary", penv, DB_DUP), penv, mDialogStorage);
  mConnectionStorage = make_connection_storage(
      make_db("connections.db", "primary", penv), penv, mContactStorage);

  mDiscoveredContactStorage = make_discovered_contact_storage(
      make_db("discover_contacts.db", "primary", penv), penv);
}
