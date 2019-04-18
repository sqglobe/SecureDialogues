#include "coreinitializer.h"
#include "interfaces/abstractusernotifier.h"

#include "containers/connectioninfocontainer.h"
#include "containers/contactcontainer.h"
#include "containers/dialogmanager.h"
#include "containers/messagecontainer.h"

#include "communication/dialogactionhandler.h"
#include "communication/messageactionhandler.h"
#include "communication/messagedespatcher.h"

#include "containers/connectcontainerwatcher.h"
#include "interfaces/abstractchanneladapter.h"

#include "chaneladapterfactory.h"
#include "utils/messagemarshaller.h"

#include "utils/fileutils.h"

#include "builders/builders.h"

#include "utils/systemmessagegenerator.h"

#include <QMetaType>
#include <QVector>

#include "crypto/cryptosystemcontactupdateinformator.h"
#include "crypto/cryptosystemdialogremoveinformator.h"
#include "crypto/cryptosystemimpl.h"

#include "asymetrical/asymetricalkeystore.h"
#include "serialization/fileasynckeyserializer.h"

#include "asymetrical/asymetricalkeygenerator.h"

#include "interfaces/symetricalcipher.h"
#include "symetrical/symetricalsystemfactories.h"

template <typename Container>
void saveToFile(const std::string& name,
                Container& container,
                const std::shared_ptr<const Encryptor>& encryptor) {
  std::stringstream ss;
  auto builder = make_builder(ss, container);
  builder.serialize();

  std::fstream outFile(name, std::ios_base::out);
  outFile << encryptor->encrypt(ss.str()) << std::flush;
}

template <typename Container>
void loadFromFile(const std::string& name,
                  Container& container,
                  const std::shared_ptr<const Decryptor>& decryptor) {
  std::fstream outFile(name, std::ios_base::in);

  std::string str((std::istreambuf_iterator<char>(outFile)),
                  std::istreambuf_iterator<char>());

  std::stringstream ss;

  ss << decryptor->decrypt(str);

  auto tst = ss.str();

  auto builder = make_builder(ss, container);
  builder.unserialize();
}

static const std::string FILE_CONTACT = "conf/contacts.usr";
static const std::string FILE_DIALOGS = "conf/dialogs.usr";
static const std::string FILE_CONNECTIONS = "conf/connections.usr";
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
    const std::shared_ptr<AbstractUserNotifier>& notifier,
    const std::string& pass) :
    mConnectionInfoContainer(std::make_shared<ConnectionInfoContainer>()),
    mContactContainer(std::make_shared<ContactContainer>()),
    mDialogManager(std::make_shared<DialogManager>()),
    mMessageContainer(std::make_shared<MessageContainer>()),
    mPassCipher(makeForStringPass(pass)) {
  qRegisterMetaType<AbstractUserNotifier::Severity>(
      "AbstractUserNotifier::Severity");
  qRegisterMetaType<QVector<int>>("QVector<int>");
  qRegisterMetaType<std::string>("std::string");

  mAsymetricalKeyStore = loadKeys(FILE_KEY, mPassCipher);

  mCryptoSystem = std::make_shared<CryptoSystemImpl>(
      mAsymetricalKeyStore, std::make_shared<MessageMarshaller>());

  mMessageDispatcher =
      std::make_shared<MessageDespatcher>(mCryptoSystem, notifier);
  mMessageActionHandler = std::make_shared<MessageActionHandler>(
      mDialogManager, mMessageContainer, mMessageDispatcher, notifier,
      mCryptoSystem);
  mDialogActionHandler = std::make_shared<DialogActionHandler>(
      mDialogManager, mMessageDispatcher, notifier, mContactContainer,
      mCryptoSystem);

  mMessageDispatcher->add(mMessageActionHandler);
  mMessageDispatcher->add(mDialogActionHandler);

  auto connWatcher = std::make_shared<ConnectContainerWatcher>(
      mMessageDispatcher,
      std::function<std::unique_ptr<AbstractChannelAdapter>(
          const ConnectionHolder&)>(ChanelAdapterFactory(notifier)),
      std::make_shared<MessageMarshaller>());
  mConnectionInfoContainer->registerWatcher(connWatcher);
  mContactContainer->registerWatcher(
      std::make_shared<CryptoSystemContactUpdateInformator>(mCryptoSystem));

  mDialogManager->registerWatcher(
      std::make_shared<SystemMessageGenerator>(mMessageContainer));
  mDialogManager->registerWatcher(
      std::make_shared<CryptoSystemDialogRemoveInformator>(mCryptoSystem));

  loadFromFiles(FILE_CONNECTIONS, FILE_CONTACT, mPassCipher);
}

CoreInitializer::~CoreInitializer() {}

std::shared_ptr<MessageActionHandler> CoreInitializer::getMessageActionHandler()
    const {
  return mMessageActionHandler;
}

std::shared_ptr<DialogActionHandler> CoreInitializer::getDialogActionHandler()
    const {
  return mDialogActionHandler;
}

std::shared_ptr<ConnectionInfoContainer>
CoreInitializer::getConnectionInfocontainer() const {
  return mConnectionInfoContainer;
}

std::shared_ptr<DialogManager> CoreInitializer::getDialogManager() const {
  return mDialogManager;
}

std::shared_ptr<MessageContainer> CoreInitializer::getMessageContainer() const {
  return mMessageContainer;
}

std::shared_ptr<ContactContainer> CoreInitializer::getContactContainer() const {
  return mContactContainer;
}

std::shared_ptr<CryptoSystemImpl> CoreInitializer::getCryptoSystem() const {
  return mCryptoSystem;
}

void CoreInitializer::saveFiles() {
  saveToFiles(FILE_CONNECTIONS, FILE_CONTACT, mPassCipher);

  saveKeys(FILE_KEY, mAsymetricalKeyStore, mPassCipher);
}

void CoreInitializer::loadFromFiles(
    const std::string& connFile,
    const std::string& contactFile,
    const std::shared_ptr<const Decryptor>& decryptor) {
  loadFromFile(connFile, *(mConnectionInfoContainer.get()), decryptor);
  loadFromFile(contactFile, *(mContactContainer.get()), decryptor);
}

void CoreInitializer::saveToFiles(
    const std::string& connFile,
    const std::string& contactFile,
    const std::shared_ptr<const Encryptor>& encryptor) {
  saveToFile(connFile, *(mConnectionInfoContainer.get()), encryptor);
  saveToFile(contactFile, *(mContactContainer.get()), encryptor);
}
