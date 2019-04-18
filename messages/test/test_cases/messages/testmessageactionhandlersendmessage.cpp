#include "testmessageactionhandlersendmessage.h"
#include <QTest>

#include "messages/channel.h"
#include "messages/contact.h"
#include "messages/containers/dialogmanager.h"
#include "messages/dialog.h"
#include "messages/interfaces/abstractusernotifier.h"
#include "messages/messageactionhandler.h"
#include "messages/messagecontainer.h"
#include "messages/messagedespatcher.h"

#include "fakes/fakechanneladaptersendmessageonly.h"
#include "fakes/fakemessagecontainerhandler.h"
#include "fakes/fakenotifier.h"

#include <QString>

Q_DECLARE_METATYPE(std::string);

TestMessageActionHandlerSendMessage::TestMessageActionHandlerSendMessage(
    QObject* parent) :
    QObject(parent) {
  mMessageDispatcher = std::make_shared<MessageDespatcher>();
  mDialogManager = std::make_shared<DialogManager>();
  mFakeMessageContHandler =
      std::make_shared<FakeMessageContainerHandlerOnlyMesssageAdded>();
  mFakeNotifier = std::make_shared<FakeNotifier>();
  auto messageContainer = std::make_shared<MessageContainer>();
  messageContainer->registerHandler(mFakeMessageContHandler);

  mMessageActionHandler =
      std::shared_ptr<MessageActionHandler>(new MessageActionHandler(
          mDialogManager, messageContainer, mMessageDispatcher, mFakeNotifier));
  mMessageDispatcher->add(mMessageActionHandler);
}

void TestMessageActionHandlerSendMessage::testSendMesageToDialogWithBadState() {
  static const std::string DIALOG_ID = "test bad dialog";
  static const std::string CHANNEL_NAME = "test channel";

  auto contact = std::make_shared<Contact>(CHANNEL_NAME, "fake_name", "fail");
  auto dialog = std::make_shared<Dialog>(contact, DIALOG_ID);
  mDialogManager->add(dialog);

  auto fakeChannelAdapter = new FakeChannelAdapterSendMessageOnly;

  mMessageDispatcher->add(std::unique_ptr<Channel>(new Channel(
                              fakeChannelAdapter, mMessageDispatcher)),
                          CHANNEL_NAME);

  mMessageActionHandler->setActiveDialog(DIALOG_ID);
  mMessageActionHandler->sendMessage("hi me");
  QCOMPARE(mFakeNotifier->mSev, AbstractUserNotifier::Severity::ERROR);
  QCOMPARE(
      mFakeNotifier->mMessage,
      std::string("Не удалось отправить сообщение: ")
          .append("You cant send messages or close not activeated dialog"));
}

void TestMessageActionHandlerSendMessage::testSendMessageToMissedChannel() {
  static const std::string DIALOG_ID_OK = "test long dialog name with ok state";
  static const std::string CHANNEL_NAME_OK = "test channel long name";
  static const std::string CHANNEL_NAME_OTHER_OK =
      "other channel other name of channel";

  auto contact =
      std::make_shared<Contact>(CHANNEL_NAME_OK, "fake_name", "fail");
  auto dialog =
      std::make_shared<Dialog>(contact, DIALOG_ID_OK, Dialog::Status::ACTIVE);
  mDialogManager->add(dialog);

  auto fakeChannelAdapter = new FakeChannelAdapterSendMessageOnly;

  mMessageDispatcher->add(std::unique_ptr<Channel>(new Channel(
                              fakeChannelAdapter, mMessageDispatcher)),
                          CHANNEL_NAME_OTHER_OK);

  mMessageActionHandler->setActiveDialog(DIALOG_ID_OK);
  mMessageActionHandler->sendMessage("hi me");

  QString mess(mFakeNotifier->mMessage.c_str());
  QCOMPARE(mFakeNotifier->mSev, AbstractUserNotifier::Severity::ERROR);
  Q_ASSERT(mess.contains("map::at") &&
           mess.startsWith("Не удалось отправить сообщение:"));
}

void TestMessageActionHandlerSendMessage::testSendMessageOutOk() {
  QFETCH(std::string, dialog_id);
  QFETCH(std::string, adress);
  QFETCH(std::string, channel);
  QFETCH(std::string, content);
  QFETCH(std::string, message);

  auto contact = std::make_shared<Contact>(channel, "fake_name", adress);
  auto dialog =
      std::make_shared<Dialog>(contact, dialog_id, Dialog::Status::ACTIVE);
  mDialogManager->add(dialog);

  auto fakeChannelAdapter = new FakeChannelAdapterSendMessageOnly;

  mMessageDispatcher->add(std::unique_ptr<Channel>(new Channel(
                              fakeChannelAdapter, mMessageDispatcher)),
                          channel);

  mMessageActionHandler->setActiveDialog(dialog_id);
  mMessageActionHandler->sendMessage(content);

  QCOMPARE(mFakeMessageContHandler->mDialogId, dialog_id);
  QCOMPARE(mFakeMessageContHandler->mMessage, content);
  QCOMPARE(mFakeMessageContHandler->mIsIncomming, false);

  QCOMPARE(fakeChannelAdapter->mAdress, adress);
  QCOMPARE(fakeChannelAdapter->mMessage, message);
}

void TestMessageActionHandlerSendMessage::testSendMessageOutOk_data() {
  QTest::addColumn<std::string>("dialog_id");
  QTest::addColumn<std::string>("adress");
  QTest::addColumn<std::string>("channel");
  QTest::addColumn<std::string>("content");
  QTest::addColumn<std::string>("message");

  QTest::newRow("0") << std::string("fake id") << std::string("Fake adress")
                     << std::string("channel 1")
                     << std::string("my testmessage")
                     << std::string(
                            "SECURE DIALOG MESSAGE\n\
VERSION:1.0\n\
ACTION:MESSAGE\n\
DIALOG:fake id\n\
--------------------------------------------------------------\n\
my testmessage");

  QTest::newRow("1") << std::string("213123se231")
                     << std::string("test@mail.com")
                     << std::string("channel 22") << std::string("hi all data")
                     << std::string(
                            "SECURE DIALOG MESSAGE\n\
VERSION:1.0\n\
ACTION:MESSAGE\n\
DIALOG:213123se231\n\
--------------------------------------------------------------\n\
hi all data");

  QTest::newRow("2") << std::string("213213asxsd") << std::string("id21321333")
                     << std::string("channel 23") << std::string("information")
                     << std::string(
                            "SECURE DIALOG MESSAGE\n\
VERSION:1.0\n\
ACTION:MESSAGE\n\
DIALOG:213213asxsd\n\
--------------------------------------------------------------\n\
information");

  QTest::newRow("3") << std::string("13213213asd")
                     << std::string("test adress1") << std::string("channel 4")
                     << std::string("long long data")
                     << std::string(
                            "SECURE DIALOG MESSAGE\n\
VERSION:1.0\n\
ACTION:MESSAGE\n\
DIALOG:13213213asd\n\
--------------------------------------------------------------\n\
long long data");
}
