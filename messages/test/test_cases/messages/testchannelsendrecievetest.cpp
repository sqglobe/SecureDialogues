#include <QTest>
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
#include "communication/channel.h"
#include "fakechanneladapter.h"
#include "fakemessagedespather.h"
#include "fakemessagehandler.h"
#include "utils/messagemarshaller.h"

Q_DECLARE_METATYPE(std::string);
Q_DECLARE_METATYPE(DialogMessage::Action);

class TestChannelSendRecieveTest : public QObject {
  Q_OBJECT

 public:
  TestChannelSendRecieveTest();

 private Q_SLOTS:
  void testReceieveMessage();
  void testReceieveMessage_data();
};

TestChannelSendRecieveTest::TestChannelSendRecieveTest() {}

void TestChannelSendRecieveTest::testReceieveMessage() {
  QFETCH(DialogMessage::Action, action);
  QFETCH(std::string, dialog_id);
  QFETCH(std::string, content);
  QFETCH(std::string, check);
  std::shared_ptr<FakeMessageDespather> dsp(new FakeMessageDespather);

  auto channel = std::make_shared<Channel>(
      std::unique_ptr<AbstractChannelAdapter>(new FakeChannelAdapter(check)),
      dsp, std::make_shared<MessageMarshaller>(), "test");
  channel->startCycle();
  std::this_thread::sleep_for(std::chrono::seconds(1));

  DialogMessage m = dsp->mMessage;

  QCOMPARE(m.action(), action);
  QCOMPARE(m.content(), content);
  QCOMPARE(m.dialogId(), dialog_id);
}

void TestChannelSendRecieveTest::testReceieveMessage_data() {
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
SEQUENTAL_NUMBER:0\n\
DIALOG:23434_12312\n\
TIMESTAMP:1232323\n\
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
SEQUENTAL_NUMBER:0\n\
DIALOG:dialog_id0\n\
TIMESTAMP:1232323\n\
SIGNATURE: 1\n\
--------------------------------------------------------------\n\
test body is a body");
}

QTEST_APPLESS_MAIN(TestChannelSendRecieveTest)

#include "testchannelsendrecievetest.moc"
