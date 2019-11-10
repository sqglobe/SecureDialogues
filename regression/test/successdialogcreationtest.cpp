#include <QtTest>
#include "fakemessagecontainerhandler.h"
#include "fakenotifier.h"
#include "faketemplatechangewatcher.h"
#include "stand-helpers/queuedactionschanneladapter.h"
#include "stand/dialogcreationandmessagesexchangecorestand.h"

#include "primitives/dialog.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/spdlog.h"
#include "utils/messagemarshaller.h"
Q_DECLARE_METATYPE(std::string);

class SuccessDialogCreationTest : public QObject {
  Q_OBJECT

 public:
 private Q_SLOTS:
  void initTestCase();
  void cleanupTestCase();
  void testCreateDialogThrowMessageExchangeAndThenCloseThem();
  void testCreateDialogThrowUserActionAndThenCloseThem();

 private:
  std::unique_ptr<DialogCreationAndMessagesExchangeCoreStand> mStand;
  std::shared_ptr<FakeMessageContainerHandlerOnlyMesssageAdded> mMessageAdded;
  std::shared_ptr<FakeTemplateChangeListener<Dialog>> mDialogChange;
  QueuedActionsChannelAdapter* mAdapter;
};

void SuccessDialogCreationTest::initTestCase() {
  QDir curr;
  if (curr.cd("SuccessDialogCreationTest_env"))
    curr.removeRecursively();

  spdlog::set_level(spdlog::level::debug);  // Set global log level to info
  spdlog::rotating_logger_mt("root_logger", "SuccessDialogCreationTest.log",
                             1048576 * 5, 3);
  mStand = std::make_unique<DialogCreationAndMessagesExchangeCoreStand>(
      "SuccessDialogCreationTest_env");
  mMessageAdded =
      std::make_shared<FakeMessageContainerHandlerOnlyMesssageAdded>();
  mDialogChange = std::make_shared<FakeTemplateChangeListener<Dialog>>();
  mAdapter = new QueuedActionsChannelAdapter(std::make_shared<FakeNotifier>(),
                                             ConnectionHolder("Test 1"));
  mStand->addChannelAdapter(mAdapter, "Test 1");
  mStand->messageContainer()->registerHandler(mMessageAdded);
  mStand->dialogStorage()->appendPermanentListener(mDialogChange);
}

void SuccessDialogCreationTest::cleanupTestCase() {
  mStand.release();
}

void SuccessDialogCreationTest::
    testCreateDialogThrowMessageExchangeAndThenCloseThem() {
  std::optional<QueuedActionsChannelAdapter::MessageData> messageFromApp;
  std::optional<DialogMessage> parsedMessageFromApp;
  MessageMarshaller marshaller;

  mAdapter->sendMessageToApplication(
      {"address 1",
       "SECURE DIALOG MESSAGE\n"
       "VERSION:1.0\n"
       "ACTION:CREATE_DIALOG\n"
       "SEQUENTAL_NUMBER:1\n"
       "DIALOG:23434_12312\n"
       "TIMESTAMP: " +
           std::to_string(get_timestamp()) +
           "\n"
           "SIGNATURE: 1\n"
           "--------------------------------------------------------------\n"
           "\n"});
  messageFromApp = mAdapter->recieveMessageFromApplication(2000);
  {
    QVERIFY(messageFromApp.has_value());
    QCOMPARE(messageFromApp->address, "address 1");

    parsedMessageFromApp =
        marshaller.unmarshall(messageFromApp->message, messageFromApp->address);

    QVERIFY(parsedMessageFromApp.has_value());
    QCOMPARE(parsedMessageFromApp->action(), DialogMessage::Action::ACK);
    QCOMPARE(parsedMessageFromApp->sequential(), 1);
    QCOMPARE(parsedMessageFromApp->dialogId(), "23434_12312");
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(200));
  {
    QVERIFY(mDialogChange->mVal.has_value());
    QCOMPARE(mDialogChange->mMethod, std::string("added"));
    QCOMPARE(mDialogChange->mVal->getStatus(), Dialog::Status::CREATE_REQUEST);
    QCOMPARE(mDialogChange->mVal->getDialogId(), std::string("23434_12312"));
    QCOMPARE(mDialogChange->mVal->getContactId(), std::string("contact 1"));
  }

  messageFromApp = mAdapter->recieveMessageFromApplication(2000);
  {
    QVERIFY(messageFromApp.has_value());
    QCOMPARE(messageFromApp->address, "address 1");

    parsedMessageFromApp =
        marshaller.unmarshall(messageFromApp->message, messageFromApp->address);

    QVERIFY(parsedMessageFromApp.has_value());
    QCOMPARE(parsedMessageFromApp->action(), DialogMessage::Action::VERIFY_KEY);
    QCOMPARE(parsedMessageFromApp->dialogId(), "23434_12312");
    QCOMPARE(parsedMessageFromApp->content(), std::string("1"));
  }

  mAdapter->sendMessageToApplication(
      {"address 1",
       "SECURE DIALOG MESSAGE\n"
       "VERSION:1.0\n"
       "ACTION:ACK\n"
       "SEQUENTAL_NUMBER:" +
           std::to_string(parsedMessageFromApp->sequential()) +
           "\n"
           "DIALOG:23434_12312\n"
           "TIMESTAMP: " +
           std::to_string(get_timestamp()) +
           "\n"
           "SIGNATURE: 1\n"
           "--------------------------------------------------------------\n"
           "\n"});

  std::this_thread::sleep_for(std::chrono::milliseconds(200));
  {
    QCOMPARE(mDialogChange->mMethod, std::string("changed"));
    QVERIFY(mDialogChange->mVal.has_value());
    QCOMPARE(mDialogChange->mVal->getStatus(),
             Dialog::Status::WAIT_KEY_VERIFICAION);
    QCOMPARE(mDialogChange->mVal->getDialogId(), std::string("23434_12312"));
    QCOMPARE(mDialogChange->mVal->getContactId(), std::string("contact 1"));
  }

  mAdapter->sendMessageToApplication(
      {"address 1",
       "SECURE DIALOG MESSAGE\n"
       "VERSION:1.0\n"
       "ACTION:KEY_VERIFICATION\n"
       "SEQUENTAL_NUMBER:2\n"
       "DIALOG:23434_12312\n"
       "TIMESTAMP: " +
           std::to_string(get_timestamp()) +
           "\n"
           "SIGNATURE: 1\n"
           "--------------------------------------------------------------\n"
           "1\n"});

  messageFromApp = mAdapter->recieveMessageFromApplication(200);
  {
    QVERIFY(messageFromApp.has_value());
    QCOMPARE(messageFromApp->address, "address 1");

    parsedMessageFromApp =
        marshaller.unmarshall(messageFromApp->message, messageFromApp->address);

    QVERIFY(parsedMessageFromApp.has_value());
    QCOMPARE(parsedMessageFromApp->action(), DialogMessage::Action::ACK);
    QCOMPARE(parsedMessageFromApp->sequential(), 2);
    QCOMPARE(parsedMessageFromApp->dialogId(), "23434_12312");
  }

  messageFromApp = mAdapter->recieveMessageFromApplication(2000);
  {
    QVERIFY(messageFromApp.has_value());
    QCOMPARE(messageFromApp->address, "address 1");

    parsedMessageFromApp =
        marshaller.unmarshall(messageFromApp->message, messageFromApp->address);

    QVERIFY(parsedMessageFromApp.has_value());
    QCOMPARE(parsedMessageFromApp->action(),
             DialogMessage::Action::ACCEPT_DIALOG);
    QCOMPARE(parsedMessageFromApp->dialogId(), "23434_12312");
  }

  mAdapter->sendMessageToApplication(
      {"address 1",
       "SECURE DIALOG MESSAGE\n"
       "VERSION:1.0\n"
       "ACTION:ACK\n"
       "SEQUENTAL_NUMBER:" +
           std::to_string(parsedMessageFromApp->sequential()) +
           "\n"
           "DIALOG:23434_12312\n"
           "TIMESTAMP: " +
           std::to_string(get_timestamp()) +
           "\n"
           "SIGNATURE: 1\n"
           "--------------------------------------------------------------\n"
           "\n"});

  std::this_thread::sleep_for(std::chrono::milliseconds(200));
  {
    QCOMPARE(mDialogChange->mMethod, std::string("changed"));
    QVERIFY(mDialogChange->mVal.has_value());
    QCOMPARE(mDialogChange->mVal->getStatus(), Dialog::Status::ACTIVE);
    QCOMPARE(mDialogChange->mVal->getDialogId(), std::string("23434_12312"));
    QCOMPARE(mDialogChange->mVal->getContactId(), std::string("contact 1"));
  }

  mAdapter->sendMessageToApplication(
      {"address 1",
       "SECURE DIALOG MESSAGE\n"
       "VERSION:1.0\n"
       "ACTION:MESSAGE\n"
       "SEQUENTAL_NUMBER:3\n"
       "DIALOG:23434_12312\n"
       "TIMESTAMP: " +
           std::to_string(get_timestamp()) +
           "\n"
           "SIGNATURE: 1\n"
           "--------------------------------------------------------------\n"
           "test message\n"});

  messageFromApp = mAdapter->recieveMessageFromApplication(200);
  {
    QVERIFY(messageFromApp.has_value());
    QCOMPARE(messageFromApp->address, "address 1");

    parsedMessageFromApp =
        marshaller.unmarshall(messageFromApp->message, messageFromApp->address);

    QVERIFY(parsedMessageFromApp.has_value());
    QCOMPARE(parsedMessageFromApp->action(), DialogMessage::Action::ACK);
    QCOMPARE(parsedMessageFromApp->sequential(), 3);
    QCOMPARE(parsedMessageFromApp->dialogId(), "23434_12312");
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(200));
  {
    QVERIFY(mMessageAdded->mIsIncomming);

    QCOMPARE(mMessageAdded->mDialogId, "23434_12312");
    QCOMPARE(mMessageAdded->mMessage, "test message");
  }

  mAdapter->sendMessageToApplication(
      {"address 1",
       "SECURE DIALOG MESSAGE\n"
       "VERSION:1.0\n"
       "ACTION:CLOSE_DIALOG\n"
       "SEQUENTAL_NUMBER:4\n"
       "DIALOG:23434_12312\n"
       "TIMESTAMP: " +
           std::to_string(get_timestamp()) +
           "\n"
           "SIGNATURE: 1\n"
           "--------------------------------------------------------------\n"
           "\n"});

  messageFromApp = mAdapter->recieveMessageFromApplication(200);
  {
    QVERIFY(messageFromApp.has_value());
    QCOMPARE(messageFromApp->address, "address 1");

    parsedMessageFromApp =
        marshaller.unmarshall(messageFromApp->message, messageFromApp->address);

    QVERIFY(parsedMessageFromApp.has_value());
    QCOMPARE(parsedMessageFromApp->action(), DialogMessage::Action::ACK);
    QCOMPARE(parsedMessageFromApp->sequential(), 4);
    QCOMPARE(parsedMessageFromApp->dialogId(), "23434_12312");
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(200));
  {
    QCOMPARE(mDialogChange->mMethod, std::string("changed"));
    QVERIFY(mDialogChange->mVal.has_value());
    QCOMPARE(mDialogChange->mVal->getStatus(), Dialog::Status::CLOSED);
    QCOMPARE(mDialogChange->mVal->getDialogId(), std::string("23434_12312"));
    QCOMPARE(mDialogChange->mVal->getContactId(), std::string("contact 1"));
  }
}

void SuccessDialogCreationTest::
    testCreateDialogThrowUserActionAndThenCloseThem() {
  std::optional<QueuedActionsChannelAdapter::MessageData> messageFromApp;
  std::optional<DialogMessage> parsedMessageFromApp;
  MessageMarshaller marshaller;

  auto contact =
      make_contact("Test 1", "name 2", "address 2", "no key", "contact 2");

  mStand->dialogActionHandler()->createDialog(contact);

  messageFromApp = mAdapter->recieveMessageFromApplication(2000);
  {
    QVERIFY(mDialogChange->mVal.has_value());
    QCOMPARE(mDialogChange->mMethod, std::string("changed"));
    QCOMPARE(mDialogChange->mVal->getStatus(), Dialog::Status::NEW);
    QCOMPARE(mDialogChange->mVal->getContactId(), std::string("contact 2"));
    QVERIFY(messageFromApp.has_value());
    QCOMPARE(messageFromApp->address, "address 2");

    parsedMessageFromApp =
        marshaller.unmarshall(messageFromApp->message, messageFromApp->address);

    QVERIFY(parsedMessageFromApp.has_value());
    QCOMPARE(parsedMessageFromApp->action(),
             DialogMessage::Action::CREATE_DIALOG);
  }

  const std::string dialogId(parsedMessageFromApp->dialogId());

  mAdapter->sendMessageToApplication(
      {"address 2",
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
  {
    QCOMPARE(mDialogChange->mMethod, std::string("changed"));
    QVERIFY(mDialogChange->mVal.has_value());
    QCOMPARE(mDialogChange->mVal->getStatus(), Dialog::Status::WAIT_CONFIRM);
    QCOMPARE(mDialogChange->mVal->getContactId(), std::string("contact 2"));
    QCOMPARE(mDialogChange->mVal->getDialogId(), dialogId);
  }

  mAdapter->sendMessageToApplication(
      {"address 2",
       "SECURE DIALOG MESSAGE\n"
       "VERSION:1.0\n"
       "ACTION:VERIFY_KEY\n"
       "SEQUENTAL_NUMBER:1\n"
       "DIALOG:" +
           dialogId +
           "\n"
           "TIMESTAMP: " +
           std::to_string(get_timestamp()) +
           "\n"
           "SIGNATURE: 1\n"
           "--------------------------------------------------------------\n"
           "1\n"});

  messageFromApp = mAdapter->recieveMessageFromApplication(2000);
  {
    QVERIFY(messageFromApp.has_value());
    QCOMPARE(messageFromApp->address, "address 2");

    parsedMessageFromApp =
        marshaller.unmarshall(messageFromApp->message, messageFromApp->address);

    QVERIFY(parsedMessageFromApp.has_value());
    QCOMPARE(parsedMessageFromApp->action(), DialogMessage::Action::ACK);
    QCOMPARE(parsedMessageFromApp->sequential(), 1);
    QCOMPARE(parsedMessageFromApp->dialogId(), dialogId);
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(200));
  {
    QCOMPARE(mDialogChange->mMethod, std::string("changed"));
    QVERIFY(mDialogChange->mVal.has_value());
    QCOMPARE(mDialogChange->mVal->getStatus(), Dialog::Status::WAIT_CONFIRM);
    QCOMPARE(mDialogChange->mVal->getContactId(), std::string("contact 2"));
    QCOMPARE(mDialogChange->mVal->getDialogId(), dialogId);
  }

  messageFromApp = mAdapter->recieveMessageFromApplication(2000);
  {
    QVERIFY(messageFromApp.has_value());
    QCOMPARE(messageFromApp->address, "address 2");

    parsedMessageFromApp =
        marshaller.unmarshall(messageFromApp->message, messageFromApp->address);

    QVERIFY(parsedMessageFromApp.has_value());
    QCOMPARE(parsedMessageFromApp->action(),
             DialogMessage::Action::KEY_VERIFICATION);
    QCOMPARE(parsedMessageFromApp->dialogId(), dialogId);
  }

  mAdapter->sendMessageToApplication(
      {"address 2",
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
  {
    QCOMPARE(mDialogChange->mMethod, std::string("changed"));
    QVERIFY(mDialogChange->mVal.has_value());
    QCOMPARE(mDialogChange->mVal->getStatus(), Dialog::Status::WAIT_CONFIRM);
    QCOMPARE(mDialogChange->mVal->getContactId(), std::string("contact 2"));
    QCOMPARE(mDialogChange->mVal->getDialogId(), dialogId);
  }

  mAdapter->sendMessageToApplication(
      {"address 2",
       "SECURE DIALOG MESSAGE\n"
       "VERSION:1.0\n"
       "ACTION:ACCEPT_DIALOG\n"
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

  messageFromApp = mAdapter->recieveMessageFromApplication(2000);
  {
    QVERIFY(messageFromApp.has_value());
    QCOMPARE(messageFromApp->address, "address 2");

    parsedMessageFromApp =
        marshaller.unmarshall(messageFromApp->message, messageFromApp->address);

    QVERIFY(parsedMessageFromApp.has_value());
    QCOMPARE(parsedMessageFromApp->action(), DialogMessage::Action::ACK);
    QCOMPARE(parsedMessageFromApp->sequential(), 2);
    QCOMPARE(parsedMessageFromApp->dialogId(), dialogId);
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(200));
  {
    QCOMPARE(mDialogChange->mMethod, std::string("changed"));
    QVERIFY(mDialogChange->mVal.has_value());
    QCOMPARE(mDialogChange->mVal->getStatus(), Dialog::Status::ACTIVE);
    QCOMPARE(mDialogChange->mVal->getContactId(), std::string("contact 2"));
    QCOMPARE(mDialogChange->mVal->getDialogId(), dialogId);
  }

  mStand->messageActionHandler()->setActiveDialog(dialogId);
  mStand->messageActionHandler()->sendMessage("test message");
  {
    QVERIFY(!mMessageAdded->mIsIncomming);
    QCOMPARE(mMessageAdded->mDialogId, dialogId);
    QCOMPARE(mMessageAdded->mMessage, "test message");
  }

  messageFromApp = mAdapter->recieveMessageFromApplication(2000);
  {
    QVERIFY(messageFromApp.has_value());
    QCOMPARE(messageFromApp->address, "address 2");

    parsedMessageFromApp =
        marshaller.unmarshall(messageFromApp->message, messageFromApp->address);

    QVERIFY(parsedMessageFromApp.has_value());
    QCOMPARE(parsedMessageFromApp->action(), DialogMessage::Action::MESSAGE);
    QCOMPARE(parsedMessageFromApp->dialogId(), dialogId);
    QCOMPARE(parsedMessageFromApp->content(), "test message");
  }

  mAdapter->sendMessageToApplication(
      {"address 2",
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

  mStand->dialogActionHandler()->closeDialog(dialogId);

  messageFromApp = mAdapter->recieveMessageFromApplication(2000);
  {
    QVERIFY(messageFromApp.has_value());
    QCOMPARE(messageFromApp->address, "address 2");

    parsedMessageFromApp =
        marshaller.unmarshall(messageFromApp->message, messageFromApp->address);

    QVERIFY(parsedMessageFromApp.has_value());
    QCOMPARE(parsedMessageFromApp->action(),
             DialogMessage::Action::CLOSE_DIALOG);
    QCOMPARE(parsedMessageFromApp->dialogId(), dialogId);
  }

  mAdapter->sendMessageToApplication(
      {"address 2",
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
  {
    QCOMPARE(mDialogChange->mMethod, std::string("changed"));
    QVERIFY(mDialogChange->mVal.has_value());
    QCOMPARE(mDialogChange->mVal->getStatus(), Dialog::Status::CLOSED);
    QCOMPARE(mDialogChange->mVal->getContactId(), std::string("contact 2"));
    QCOMPARE(mDialogChange->mVal->getDialogId(), dialogId);
  }
}

QTEST_APPLESS_MAIN(SuccessDialogCreationTest)

#include "successdialogcreationtest.moc"
