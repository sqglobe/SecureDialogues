#include "onechannelcoreinitializer.h"

#include <db_cxx.h>

#include "cryptosystemfake.h"
#include "utils/dbfactory.h"
#include "utils/messagemarshaller.h"
#include "utils/systemmessagegenerator.h"

#include "asymetrical/asymetricalkeygenerator.h"
#include "asymetrical/asymetricalkeystore.h"
#include "communication/despatcherrorssink.h"
#include "communication/dialogactionhandler.h"
#include "communication/discovercontactagent.h"
#include "communication/messageactionhandler.h"
#include "communication/messagedespatcher.h"
#include "containers/messagecontainer.h"
#include "crypto/cryptosystemimpl.h"
#include "fakenotifier.h"
#include "interfaces/symetricalcipher.h"
#include "stand-helpers/queuedactionschanneladapter.h"

std::shared_ptr<AsymetricalKeyStore> create_store() {
  std::shared_ptr<AsymetricalKeyStore> store =
      std::make_shared<AsymetricalKeyStore>();
  AsymetricalKeyGenerator(store).generate();
  return store;
}

class FakeCryptoSystemImpl : public CryptoSystemImpl {
 public:
  FakeCryptoSystemImpl() :
      CryptoSystemImpl(create_store(), std::shared_ptr<MessageMarshaller>()) {}

 public:
  bool isSignatureOk(const DialogMessage&) const noexcept(false) override {
    return true;
  }
  std::string createSignature(const DialogMessage&) const
      noexcept(false) override {
    return "1";
  }

  std::string encryptMessageBody(std::string_view,
                                 std::string_view message) const
      noexcept(false) {
    return std::string(message);
  }
  std::string decryptMessageBody(std::string_view,
                                 std::string_view message) const
      noexcept(false) override {
    return std::string(message);
  }

 public:
  std::string generateAndExportKey(std::string_view,
                                   std::string_view) noexcept(false) override {
    return "1";
  }
  std::string importKey(std::string_view,
                        std::string_view) noexcept(false) override {
    return "1";
  }
  bool checkVerificationString(std::string_view,
                               std::string_view) noexcept(false) override {
    return true;
  }
};

OneChannelCoreInitializer::OneChannelCoreInitializer(
    const std::string& folder) {
  auto penv = make_db_env(folder, "test");

  mDialogStorage = make_dialog_storage(
      make_db("dialogs.db", "primary", penv),
      make_db("dialogs.db", "secondary", penv, DB_DUP), penv);
  mContactStorage = make_contact_storage(
      make_db("cotnacts.db", "primary", penv),
      make_db("contacts.db", "secondary", penv, DB_DUP), penv, mDialogStorage);
  mConnectionStorage = make_connection_storage(
      make_db("connections.db", "primary", penv), penv, mContactStorage,
      std::shared_ptr<const plugin_support::PluginsContainer>{});
  mCryptoSystemImpl = std::make_shared<FakeCryptoSystemImpl>();
  mDiscoveredContactStorage = make_discovered_contact_storage(
      make_db("discover_contacts.db", "primary", penv), penv);
}

void OneChannelCoreInitializer::init(
    const std::shared_ptr<AbstractUserNotifier>& notifier) {
  mMessageContainer = std::make_shared<MessageContainer>();
  auto cryptoSystem = std::make_shared<CryptoSystemFake>();
  mMessageDispatcher = std::make_shared<MessageDespatcher>(
      cryptoSystem, notifier,
      std::make_shared<DespatchErrorsSink>(mDiscoveredContactStorage));

  mMessageActionHandler = std::make_shared<MessageActionHandler>(
      mDialogStorage, mContactStorage, mMessageContainer, mMessageDispatcher,
      notifier, cryptoSystem);
  mDialogActionHandler = std::make_shared<DialogActionHandler>(
      mDialogStorage, mMessageDispatcher, notifier, mContactStorage,
      cryptoSystem);

  mMessageDispatcher->add(mMessageActionHandler);
  mMessageDispatcher->add(mDialogActionHandler);

  mDialogStorage->appendPermanentListener(
      std::make_shared<SystemMessageGenerator>(mMessageContainer));
  mDiscoverContactAgent = std::make_shared<DiscoverContactAgent>(
      mCryptoSystemImpl, mMessageDispatcher);
}

void OneChannelCoreInitializer::saveFiles() {}

void OneChannelCoreInitializer::startMessagesHandling(
    const std::shared_ptr<AbstractUserNotifier>&,
    const std::shared_ptr<Channel::EventQueue>&) {
  fill();
}

std::shared_ptr<ConnectionStorage>
OneChannelCoreInitializer::getConnectionStorage() const {
  return mConnectionStorage;
}

std::shared_ptr<ContactStorage> OneChannelCoreInitializer::getContactStorage()
    const {
  return mContactStorage;
}

std::shared_ptr<CryptoSystemImpl> OneChannelCoreInitializer::getCryptoSystem()
    const {
  return mCryptoSystemImpl;
}

std::shared_ptr<DialogActionHandler>
OneChannelCoreInitializer::getDialogActionHandler() const {
  return mDialogActionHandler;
}

std::shared_ptr<DialogStorage> OneChannelCoreInitializer::getDialogStorage()
    const {
  return mDialogStorage;
}

std::shared_ptr<MessageActionHandler>
OneChannelCoreInitializer::getMessageActionHandler() const {
  return mMessageActionHandler;
}

std::shared_ptr<MessageContainer>
OneChannelCoreInitializer::getMessageContainer() const {
  return mMessageContainer;
}

std::shared_ptr<DiscoverContactAgent>
OneChannelCoreInitializer::getDiscoverContactAgent() const {
  return mDiscoverContactAgent;
}

std::shared_ptr<DiscoveredContactStorage>
OneChannelCoreInitializer::getDiscoveredContactStorage() const {
  return mDiscoveredContactStorage;
}

std::pair<QueuedActionsChannelAdapter*, int>
OneChannelCoreInitializer::make_dialog(const std::string& addres,
                                       const std::string& dialogId,
                                       const std::string& channel) {
  QueuedActionsChannelAdapter* adapter(new QueuedActionsChannelAdapter(
      std::make_shared<FakeNotifier>(), ConnectionHolder(channel)));
  mMessageDispatcher->add(
      std::make_shared<Channel>(
          adapter, mMessageDispatcher, std::make_shared<MessageMarshaller>(),
          std::string(channel), std::make_shared<Channel::EventQueue>(),
          std::chrono::seconds(5)),
      channel);

  std::optional<QueuedActionsChannelAdapter::MessageData> messageFromApp;
  std::optional<DialogMessage> parsedMessageFromApp;
  MessageMarshaller marshaller;

  adapter->sendMessageToApplication(
      {addres,
       "SECURE DIALOG MESSAGE\n"
       "VERSION:1.0\n"
       "ACTION:CREATE_DIALOG\n"
       "SEQUENTAL_NUMBER:1\n"
       "DIALOG:" +
           dialogId +
           "\n"
           "TIMESTAMP: " +
           std::to_string(get_timestamp()) +
           "\n"
           "SIGNATURE: 1\n"
           "--------------------------------------------------------------\n"
           "\n"});
  messageFromApp = adapter->recieveMessageFromApplication(2000);
  {
    if (!messageFromApp.has_value()) {
      std::cerr << "Expected ack for CREATE_DIALOG, but message is empty"
                << std::endl;
      return std::pair<QueuedActionsChannelAdapter*, int>(nullptr, 0);
    }
    parsedMessageFromApp =
        marshaller.unmarshall(messageFromApp->message, messageFromApp->address);
    if (!parsedMessageFromApp.has_value())
      return std::pair<QueuedActionsChannelAdapter*, int>(nullptr, 0);

    if (parsedMessageFromApp->action() != DialogMessage::Action::ACK) {
      std::cerr << "Message action is not ACK for CREATE_DIALOG:"
                << static_cast<int>(parsedMessageFromApp->action())
                << std::endl;
      return std::pair<QueuedActionsChannelAdapter*, int>(nullptr, 0);
    }
  }

  messageFromApp = adapter->recieveMessageFromApplication(2000);
  {
    if (!messageFromApp.has_value()) {
      std::cerr << "Expected VERIFY_KEY, but message is empty" << std::endl;
      return std::pair<QueuedActionsChannelAdapter*, int>(nullptr, 0);
    }
    parsedMessageFromApp =
        marshaller.unmarshall(messageFromApp->message, messageFromApp->address);
    if (!parsedMessageFromApp.has_value())
      return std::pair<QueuedActionsChannelAdapter*, int>(nullptr, 0);

    if (parsedMessageFromApp->action() != DialogMessage::Action::VERIFY_KEY) {
      std::cerr << "Message action is not VERIFY_KEY:"
                << static_cast<int>(parsedMessageFromApp->action())
                << std::endl;
      return std::pair<QueuedActionsChannelAdapter*, int>(nullptr, 0);
    }
  }

  adapter->sendMessageToApplication(
      {addres,
       "SECURE DIALOG MESSAGE\n"
       "VERSION:1.0\n"
       "ACTION:ACK\n"
       "SEQUENTAL_NUMBER:" +
           std::to_string(parsedMessageFromApp->sequential()) +
           "\n"
           "DIALOG:" +
           dialogId +
           "\n"
           "TIMESTAMP: " +
           std::to_string(get_timestamp()) +
           "\n"
           "SIGNATURE: 1\n"
           "--------------------------------------------------------------\n"
           "\n"});

  std::this_thread::sleep_for(std::chrono::milliseconds(200));

  adapter->sendMessageToApplication(
      {addres,
       "SECURE DIALOG MESSAGE\n"
       "VERSION:1.0\n"
       "ACTION:KEY_VERIFICATION\n"
       "SEQUENTAL_NUMBER:2\n"
       "DIALOG:" +
           dialogId +
           "\n"
           "TIMESTAMP: " +
           std::to_string(get_timestamp()) +
           "\n"
           "SIGNATURE: 1\n"
           "--------------------------------------------------------------\n"
           "1\n"});

  messageFromApp = adapter->recieveMessageFromApplication(200);
  {
    if (!messageFromApp.has_value()) {
      std::cerr << "Expected ACK for KEY_VERIFICATION, but message is empty"
                << std::endl;
      return std::pair<QueuedActionsChannelAdapter*, int>(nullptr, 0);
    }
    parsedMessageFromApp =
        marshaller.unmarshall(messageFromApp->message, messageFromApp->address);
    if (!parsedMessageFromApp.has_value())
      return std::pair<QueuedActionsChannelAdapter*, int>(nullptr, 0);

    if (parsedMessageFromApp->action() != DialogMessage::Action::ACK) {
      std::cerr << "Message action is not ACK for KEY_VERIFICATION:"
                << static_cast<int>(parsedMessageFromApp->action())
                << std::endl;
      return std::pair<QueuedActionsChannelAdapter*, int>(nullptr, 0);
    }
  }

  messageFromApp = adapter->recieveMessageFromApplication(2000);
  {
    if (!messageFromApp.has_value()) {
      std::cerr << "Expected ACCEPT_DIALOG, but message is empty" << std::endl;
      return std::pair<QueuedActionsChannelAdapter*, int>(nullptr, 0);
    }
    parsedMessageFromApp =
        marshaller.unmarshall(messageFromApp->message, messageFromApp->address);
    if (!parsedMessageFromApp.has_value())
      return std::pair<QueuedActionsChannelAdapter*, int>(nullptr, 0);

    if (parsedMessageFromApp->action() !=
        DialogMessage::Action::ACCEPT_DIALOG) {
      std::cerr << "Message action is not ACK for ACCEPT_DIALOG:"
                << static_cast<int>(parsedMessageFromApp->action())
                << std::endl;
      return std::pair<QueuedActionsChannelAdapter*, int>(nullptr, 0);
    }
  }

  adapter->sendMessageToApplication(
      {addres,
       "SECURE DIALOG MESSAGE\n"
       "VERSION:1.0\n"
       "ACTION:ACK\n"
       "SEQUENTAL_NUMBER:" +
           std::to_string(parsedMessageFromApp->sequential()) +
           "\n"
           "DIALOG:" +
           dialogId +
           "\n"
           "TIMESTAMP: " +
           std::to_string(get_timestamp()) +
           "\n"
           "SIGNATURE: 1\n"
           "--------------------------------------------------------------\n"
           "\n"});
  std::this_thread::sleep_for(std::chrono::milliseconds(200));
  return std::make_pair(adapter, 3);
}
