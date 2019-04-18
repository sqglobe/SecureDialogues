#include <QTest>
#include <memory>
#include "containers/messagecontainer.h"
#include "fakemessagecontainerhandler.h"

Q_DECLARE_METATYPE(std::string);

class FakeMessageContainerHandlerOnlyMesssageAdded;
class MessageContainer;

class TestMessageContainerRecieve : public QObject {
  Q_OBJECT

 public:
  TestMessageContainerRecieve();

 private Q_SLOTS:

  void init();
  void cleanup();

  void testMessageRecieve();
  void testMessageRecieve_data();

  void testMessageRecieveToActiveDialog();
  void testMessageRecieveToActiveDialog_data();

 private:
  std::shared_ptr<FakeMessageContainerHandlerOnlyMesssageAdded>
      mFakeMessagesAdded;
  std::shared_ptr<MessageContainer> mContainer;
  static const std::string FAKE_DIALOG_ID;
};

const std::string TestMessageContainerRecieve::FAKE_DIALOG_ID =
    "FAKE_DIALOG_ID";

TestMessageContainerRecieve::TestMessageContainerRecieve() {}

void TestMessageContainerRecieve::init() {
  mFakeMessagesAdded =
      std::make_shared<FakeMessageContainerHandlerOnlyMesssageAdded>();
  mContainer = std::make_shared<MessageContainer>();
  mContainer->setActiveDialog(FAKE_DIALOG_ID);
  mContainer->registerHandler(mFakeMessagesAdded);
}

void TestMessageContainerRecieve::cleanup() {}

void TestMessageContainerRecieve::testMessageRecieve() {
  QFETCH(std::string, dialog_id);
  QFETCH(std::string, message);
  QFETCH(bool, isIncomming);

  mContainer->addMessage(dialog_id, message, isIncomming);

  QCOMPARE(dialog_id, mFakeMessagesAdded->mDialogId);
  QCOMPARE(message, mFakeMessagesAdded->mMessage);
  QCOMPARE(isIncomming, mFakeMessagesAdded->mIsIncomming);
}

void TestMessageContainerRecieve::testMessageRecieve_data() {
  QTest::addColumn<std::string>("dialog_id");
  QTest::addColumn<std::string>("message");
  QTest::addColumn<bool>("isIncomming");

  QTest::newRow("0") << std::string("test1") << std::string("hi me") << false;
  QTest::newRow("1") << std::string("xc334") << std::string("21321asds asd")
                     << true;
  QTest::newRow("2") << std::string("6213213")
                     << std::string("hi me 132 easdas") << true;
  QTest::newRow("3") << std::string(",mkdjasd9") << std::string("21321asdasd")
                     << false;
  QTest::newRow("4") << std::string("2321312 213213")
                     << std::string("567987*9*/123") << true;
  QTest::newRow("5") << std::string("!@#@!#@!#sdsad")
                     << std::string("12312saa 03984") << false;
}

void TestMessageContainerRecieve::testMessageRecieveToActiveDialog() {
  QFETCH(std::string, message);
  QFETCH(bool, isIncomming);

  mContainer->addMessageToActivedialog(message, isIncomming);

  QCOMPARE(FAKE_DIALOG_ID, mFakeMessagesAdded->mDialogId);
  QCOMPARE(message, mFakeMessagesAdded->mMessage);
  QCOMPARE(isIncomming, mFakeMessagesAdded->mIsIncomming);
}

void TestMessageContainerRecieve::testMessageRecieveToActiveDialog_data() {
  QTest::addColumn<std::string>("message");
  QTest::addColumn<bool>("isIncomming");

  QTest::newRow("0") << std::string("hi me") << false;
  QTest::newRow("1") << std::string("21321asds asd") << true;
  QTest::newRow("2") << std::string("hi me 132 easdas") << true;
  QTest::newRow("3") << std::string("21321asdasd") << false;
  QTest::newRow("4") << std::string("567987*9*/123") << true;
  QTest::newRow("5") << std::string("12312saa 03984") << false;
}

QTEST_APPLESS_MAIN(TestMessageContainerRecieve)

#include "testmessagecontainerrecieve.moc"
