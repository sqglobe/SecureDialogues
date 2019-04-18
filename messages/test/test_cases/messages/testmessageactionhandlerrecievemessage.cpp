#include <QTest>
#include <iostream>

#include "communication/channel.h"
#include "communication/messageactionhandler.h"
#include "communication/messagedespatcher.h"
#include "containers/dialogmanager.h"
#include "containers/messagecontainer.h"
#include "interfaces/abstractusernotifier.h"
#include "primitives/contact.h"
#include "primitives/dialog.h"

#include "fakechanneladapter.h"
#include "fakemessagecontainerhandler.h"
#include "fakenotifier.h"
#include "utils/messagemarshaller.h"

#include "cryptosystemfake.h"
#include "utils.h"

#include <QString>

#include <memory>
#include <thread>

Q_DECLARE_METATYPE(std::string);

class MessageDespatcher;
class DialogManager;
class MessageActionHandler;
class FakeMessageContainerHandlerOnlyMesssageAdded;
class FakeNotifier;

class TestMessageActionHandlerRecieveMessage : public QObject {
  Q_OBJECT
 public:
  explicit TestMessageActionHandlerRecieveMessage(QObject* parent = nullptr);

 signals:

 private slots:
  void testMessageRecieveOk();
  void testMessageRecieveOk_data();

 private:
  std::shared_ptr<MessageDespatcher> mMessageDispatcher;
  std::shared_ptr<DialogManager> mDialogManager;
  std::shared_ptr<MessageActionHandler> mMessageActionHandler;
  std::shared_ptr<FakeMessageContainerHandlerOnlyMesssageAdded>
      mFakeMessageContHandler;
  std::shared_ptr<FakeNotifier> mFakeNotifier;
};

TestMessageActionHandlerRecieveMessage::TestMessageActionHandlerRecieveMessage(
    QObject* parent) :
    QObject(parent) {
  mMessageDispatcher = std::make_shared<MessageDespatcher>(
      std::make_shared<CryptoSystemFake>(), std::make_shared<FakeNotifier>());
  mDialogManager = std::make_shared<DialogManager>();
  mFakeMessageContHandler =
      std::make_shared<FakeMessageContainerHandlerOnlyMesssageAdded>();
  mFakeNotifier = std::make_shared<FakeNotifier>();
  auto messageContainer = std::make_shared<MessageContainer>();
  messageContainer->registerHandler(mFakeMessageContHandler);

  mMessageActionHandler =
      std::shared_ptr<MessageActionHandler>(new MessageActionHandler(
          mDialogManager, messageContainer, mMessageDispatcher, mFakeNotifier,
          std::make_shared<CryptoSystemFake>()));
  mMessageDispatcher->add(mMessageActionHandler);
}

void TestMessageActionHandlerRecieveMessage::testMessageRecieveOk() {
  QFETCH(std::string, dialog_id);
  QFETCH(std::string, adress);
  QFETCH(std::string, channel);
  QFETCH(std::string, content);
  QFETCH(std::string, message);

  auto contact = std::make_shared<Contact>(channel, "fake_name", adress, "");
  auto dialog =
      std::make_shared<Dialog>(contact, dialog_id, Dialog::Status::ACTIVE);
  mDialogManager->add(dialog);

  mMessageDispatcher->add(
      std::unique_ptr<Channel>(
          new Channel(new FakeChannelAdapter(message), mMessageDispatcher,
                      std::make_shared<MessageMarshaller>(), channel)),
      channel);

  std::this_thread::sleep_for(std::chrono::seconds(1));

  QCOMPARE(mFakeMessageContHandler->mDialogId, dialog_id);
  QCOMPARE(mFakeMessageContHandler->mMessage, content);
  QCOMPARE(mFakeMessageContHandler->mIsIncomming, true);
}

void TestMessageActionHandlerRecieveMessage::testMessageRecieveOk_data() {
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
SEQUENTAL_NUMBER:2\n\
DIALOG:fake id\n\
TIMESTAMP:" + std::to_string(get_timestamp()) +
                            "\n\
SIGNATURE: 1\n\
--------------------------------------------------------------\n\
my testmessage");

  QTest::newRow("1") << std::string("213123se231")
                     << std::string("test@mail.com")
                     << std::string("channel 22") << std::string("hi all data")
                     << std::string(
                            "SECURE DIALOG MESSAGE\n\
VERSION:1.0\n\
ACTION:MESSAGE\n\
SEQUENTAL_NUMBER:2\n\
DIALOG:213123se231\n\
TIMESTAMP:" + std::to_string(get_timestamp()) +
                            "\n\
SIGNATURE: 1\n\
--------------------------------------------------------------\n\
hi all data");

  QTest::newRow("2") << std::string("213213asxsd") << std::string("id21321333")
                     << std::string("channel 23") << std::string("information")
                     << std::string(
                            "SECURE DIALOG MESSAGE\n\
VERSION:1.0\n\
ACTION:MESSAGE\n\
SEQUENTAL_NUMBER:2\n\
DIALOG:213213asxsd\n\
TIMESTAMP:" + std::to_string(get_timestamp()) +
                            "\n\
SIGNATURE: 1\n\
--------------------------------------------------------------\n\
information");

  QTest::newRow("3") << std::string("13213213asd")
                     << std::string("test adress1") << std::string("channel 4")
                     << std::string("long long data")
                     << std::string(
                            "SECURE DIALOG MESSAGE\n\
VERSION:1.0\n\
ACTION:MESSAGE\n\
SEQUENTAL_NUMBER:2\n\
DIALOG:13213213asd\n\
TIMESTAMP:" + std::to_string(get_timestamp()) +
                            "\n\
SIGNATURE: 1\n\
--------------------------------------------------------------\n\
long long data");
}

QTEST_APPLESS_MAIN(TestMessageActionHandlerRecieveMessage)

#include "testmessageactionhandlerrecievemessage.moc"
