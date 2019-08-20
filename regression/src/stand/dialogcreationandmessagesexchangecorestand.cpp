#include "dialogcreationandmessagesexchangecorestand.h"
#include "utils/messagemarshaller.h"

#include "utils/dbfactory.h"

#include <db_cxx.h>

#include "cryptosystemfake.h"
#include "fakenotifier.h"

#include "utils/systemmessagegenerator.h"

#include <QDir>

DialogCreationAndMessagesExchangeCoreStand::
    DialogCreationAndMessagesExchangeCoreStand(std::string folder) :
    mFolder(std::move(folder)) {
  QDir().mkdir(mFolder.c_str());
  dbstl::dbstl_startup();
  auto penv = make_db_env(mFolder, "test");

  mDialogStorage = make_dialog_storage(
      make_db("dialogs.db", "primary", penv),
      make_db("dialogs.db", "secondary", penv, DB_DUP), penv);
  mContactStorage = make_contact_storage(
      make_db("cotnacts.db", "primary", penv),
      make_db("contacts.db", "secondary", penv, DB_DUP), penv, mDialogStorage);
  mConnectionStorage = make_connection_storage(
      make_db("connections.db", "primary", penv), penv, mContactStorage);

  auto cryptoSystem = std::make_shared<CryptoSystemFake>();
  auto notifier = std::make_shared<FakeNotifier>();

  mMessageContainer = std::make_shared<MessageContainer>();

  mMessageDispatcher =
      std::make_shared<MessageDespatcher>(cryptoSystem, notifier);

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

  fillContainers();
}

DialogCreationAndMessagesExchangeCoreStand::
    ~DialogCreationAndMessagesExchangeCoreStand() {
  dbstl::dbstl_exit();
  QDir().rmdir(mFolder.c_str());
}

void DialogCreationAndMessagesExchangeCoreStand::addChannelAdapter(
    AbstractChannelAdapter* adapter,
    std::string name) {
  mMessageDispatcher->add(
      std::make_shared<Channel>(
          adapter, mMessageDispatcher, std::make_shared<MessageMarshaller>(),
          std::string(name), std::make_shared<Channel::EventQueue>(),
          std::chrono::seconds(5)),
      name);
}

std::shared_ptr<MessageActionHandler>
DialogCreationAndMessagesExchangeCoreStand::messageActionHandler() const {
  return mMessageActionHandler;
}

std::shared_ptr<DialogActionHandler>
DialogCreationAndMessagesExchangeCoreStand::dialogActionHandler() const {
  return mDialogActionHandler;
}

std::shared_ptr<DialogStorage>
DialogCreationAndMessagesExchangeCoreStand::dialogStorage() const {
  return mDialogStorage;
}

std::shared_ptr<MessageContainer>
DialogCreationAndMessagesExchangeCoreStand::messageContainer() const {
  return mMessageContainer;
}
