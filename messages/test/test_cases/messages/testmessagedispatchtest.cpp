#include "interfaces/abstractchanneladapter.h"
#include "interfaces/abstractmessagehandler.h"
#include "interfaces/abstractusernotifier.h"

#include "fakechanneladapter.h"
#include "fakemessagehandler.h"

#include <QtTest>
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
#include "communication/channel.h"
#include "communication/messagedespatcher.h"
#include "primitives/dialogmessage.h"

#include "utils/messagemarshaller.h"

#include "cryptosystemfake.h"
#include "despatcherrorsinkfake.h"
#include "fakenotifier.h"
#include "utils.h"

Q_DECLARE_METATYPE(std::string);
Q_DECLARE_METATYPE(DialogMessage::Action);

class TestMessageDispatchTest : public QObject {
  Q_OBJECT

 public:
  TestMessageDispatchTest();

 private Q_SLOTS:
  void testDispatchMessageForActionMessage();
  void testDispatchMessageForActionMessage_data();
};

TestMessageDispatchTest::TestMessageDispatchTest() {}

void TestMessageDispatchTest::testDispatchMessageForActionMessage() {
  QFETCH(DialogMessage::Action, action);
  QFETCH(std::string, dialog_id);
  QFETCH(std::string, content);
  QFETCH(std::string, check);
  std::shared_ptr<MessageDespatcher> dsp(new MessageDespatcher(
      std::make_shared<CryptoSystemFake>(), std::make_shared<FakeNotifier>(),
      std::make_shared<DespatchErrorSinkFake>()));

  DialogMessage notUsedDialog(DialogMessage::Action::ACCEPT_DIALOG, "FAKE",
                              "FAKE_ID", "non");

  DialogMessage outMessage = DialogMessage(DialogMessage::Action::ACK, "", "");

  Channel channel(
      std::unique_ptr<AbstractChannelAdapter>(new FakeChannelAdapter(check)),
      dsp, std::make_shared<MessageMarshaller>(), "test",
      std::make_shared<Channel::EventQueue>(), std::chrono::seconds(5));
  dsp->add(std::unique_ptr<FakeMessageHandler>(
      new FakeMessageHandler(DialogMessage::Action::MESSAGE, outMessage)));
  dsp->add(std::unique_ptr<FakeMessageHandler>(new FakeMessageHandler(
      DialogMessage::Action::ACCEPT_DIALOG, notUsedDialog)));
  dsp->add(
      std::make_shared<Channel>(std::unique_ptr<AbstractChannelAdapter>(
                                    new FakeChannelAdapter(check)),
                                dsp, std::make_shared<MessageMarshaller>(),
                                "test", std::make_shared<Channel::EventQueue>(),
                                std::chrono::seconds(5)),
      "test");
  std::this_thread::sleep_for(std::chrono::seconds(1));

  QCOMPARE(notUsedDialog.action(), DialogMessage::Action::ACCEPT_DIALOG);
  QCOMPARE(notUsedDialog.content(), std::string_view("FAKE"));
  QCOMPARE(notUsedDialog.dialogId(), std::string_view("FAKE_ID"));

  QCOMPARE(outMessage.action(), action);
  QCOMPARE(outMessage.content(), std::string_view(content));
  QCOMPARE(outMessage.dialogId(), std::string_view(dialog_id));
}

void TestMessageDispatchTest::testDispatchMessageForActionMessage_data() {
  QTest::addColumn<DialogMessage::Action>("action");
  QTest::addColumn<std::string>("dialog_id");
  QTest::addColumn<std::string>("content");
  QTest::addColumn<std::string>("check");

  QTest::newRow("0") << DialogMessage::Action::MESSAGE
                     << std::string("23434_12312")
                     << std::string(
                            "test body is a body\n\
with delitimer\n\
tets:1")
                     << std::string(
                            "SECURE DIALOG MESSAGE\n\
VERSION:1.0\n\
ACTION:MESSAGE\n\
SEQUENTAL_NUMBER:2\n\
DIALOG:23434_12312\n\
TIMESTAMP:" + std::to_string(get_timestamp()) +
                            "\n\
SIGNATURE: 1\n\
--------------------------------------------------------------\n\
test body is a body\n\
with delitimer\n\
tets:1");

  QTest::newRow("1") << DialogMessage::Action::MESSAGE
                     << std::string("dialog_id0")
                     << std::string("test body is a body")
                     << std::string(
                            "SECURE DIALOG MESSAGE\n\
VERSION:1.0\n\
ACTION:MESSAGE\n\
SEQUENTAL_NUMBER:2\n\
DIALOG:dialog_id0\n\
TIMESTAMP:" + std::to_string(get_timestamp()) +
                            "\n\
SIGNATURE: 1\n\
--------------------------------------------------------------\n\
test body is a body");
}

QTEST_APPLESS_MAIN(TestMessageDispatchTest)

#include "testmessagedispatchtest.moc"
