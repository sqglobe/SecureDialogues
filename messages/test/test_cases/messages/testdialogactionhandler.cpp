#include <QTest>
#include <memory>
#include "communication/dialogactionhandler.h"
#include "containers/contactcontainer.h"
#include "containers/dialogmanager.h"
#include "fakemessagedespather.h"
#include "fakenotifier.h"
#include "primitives/contact.h"
#include "primitives/dialog.h"
#include "utils/messagemarshaller.h"

#include "cryptosystemfake.h"

Q_DECLARE_METATYPE(DialogMessage::Action)
Q_DECLARE_METATYPE(Dialog::Status)
Q_DECLARE_METATYPE(std::string)

class TestDialogActionHandler : public QObject {
  Q_OBJECT
 public:
  explicit TestDialogActionHandler(QObject* parent = nullptr);

 signals:

  /*
   *     void createDialog(const std::string &dialogId);
   *     void acceptDialog(const std::string &dialogId);
   *     void closeDialog(const std::string &dialogId);
   *     void cancelDialog(const std::string &dialogId);
   */
 private slots:

  void init();

  void testCreateDialog();
  void testCreateDialog_data();

  void testAcceptDialog();
  void testAcceptDialog_data();

  void testCloseDialog();
  void testCloseDialog_data();

  void testCancelDialog();
  void testCancelDialog_data();

  void testDialogActionRecieve();
  void testDialogActionRecieve_data();

 private:
  void makeAndFillDialogcontainer();

 private:
  std::shared_ptr<DialogManager> mDialogManager;
  std::shared_ptr<FakeMessageSendDespatcher> mMessageDispatcher;
  std::shared_ptr<FakeNotifier> mNotifier;
  std::shared_ptr<ContactContainer> mContacts;
};

template <typename F>
void exec_test(F f,
               const std::shared_ptr<DialogManager>& dialogManager,
               const std::shared_ptr<FakeMessageSendDespatcher>& mDespatcher,
               const std::shared_ptr<FakeNotifier>& notifier,
               const std::shared_ptr<ContactContainer>& contacts) {
  QFETCH(DialogMessage::Action, action);
  QFETCH(std::string, dialog_id);
  QFETCH(Dialog::Status, new_status);
  QFETCH(std::string, adress);
  QFETCH(std::string, channel);

  auto dialogActionHandler =
      DialogActionHandler(dialogManager, mDespatcher, notifier, contacts,
                          std::make_shared<CryptoSystemFake>());
  (dialogActionHandler.*f)(dialog_id);
  // QCOMPARE(mDialogEventHandler->mDialogId, dialog_id);
  // QCOMPARE(mDialogEventHandler->mFunction, "created");

  QCOMPARE(dialogManager->get(dialog_id)->getStatus(), new_status);
  QCOMPARE(mDespatcher->mChannel, channel);

  QCOMPARE(mDespatcher->mMessage.action(), action);
  QCOMPARE(mDespatcher->mMessage.dialogId(), dialog_id);
}

TestDialogActionHandler::TestDialogActionHandler(QObject* parent) :
    QObject(parent) {}

void TestDialogActionHandler::init() {
  makeAndFillDialogcontainer();
  mMessageDispatcher = std::make_shared<FakeMessageSendDespatcher>(
      FakeMessageSendDespatcher::Resolve::REMOVED);

  mNotifier = std::make_shared<FakeNotifier>();

  mContacts = std::make_shared<ContactContainer>();
}

void TestDialogActionHandler::testCreateDialog() {
  // exec_test(&DialogActionHandler::createDialog, mDialogManager,
  // mMessageDispatcher, mNotifier);
}

void TestDialogActionHandler::testCreateDialog_data() {
  QTest::addColumn<DialogMessage::Action>("action");
  QTest::addColumn<std::string>("dialog_id");
  QTest::addColumn<Dialog::Status>("new_status");
  QTest::addColumn<std::string>("adress");
  QTest::addColumn<std::string>("channel");

  /*QTest::newRow("0") << DialogMessage::Action::CREATE_DIALOG <<
  std::string("new dialog 1") << Dialog::Status::CREATED << std::string("adress
  new1") << std::string("channel 1"); QTest::newRow("1") <<
  DialogMessage::Action::CREATE_DIALOG << std::string("new dialog 2") <<
  Dialog::Status::CREATED << std::string("adress new2") << std::string("channel
  1"); QTest::newRow("2") << DialogMessage::Action::CREATE_DIALOG <<
  std::string("new dialog 3") << Dialog::Status::CREATED << std::string("adress
  new3") << std::string("channel 1");
*/
}

void TestDialogActionHandler::testAcceptDialog() {
  //  exec_test(&DialogActionHandler::acceptDialog, mDialogManager,
  //  mMessageDispatcher, mNotifier, mContacts);
}

void TestDialogActionHandler::testAcceptDialog_data() {
  QTest::addColumn<DialogMessage::Action>("action");
  QTest::addColumn<std::string>("dialog_id");
  QTest::addColumn<Dialog::Status>("new_status");
  QTest::addColumn<std::string>("adress");
  QTest::addColumn<std::string>("channel");

  /*
  QTest::newRow("0") << DialogMessage::Action::ACCEPT_DIALOG <<
  std::string("created dialog 1") << Dialog::Status::WAIT_KEY_VERIFICAION <<
  std::string("adress created1") << std::string("channel 3"); QTest::newRow("1")
  << DialogMessage::Action::ACCEPT_DIALOG << std::string("created dialog 2") <<
  Dialog::Status::WAIT_KEY_VERIFICAION << std::string("adress created2") <<
  std::string("channel 3"); QTest::newRow("2") <<
  DialogMessage::Action::ACCEPT_DIALOG << std::string("created dialog 3") <<
  Dialog::Status::WAIT_KEY_VERIFICAION << std::string("adress created3") <<
  std::string("channel 3");
  */
}

void TestDialogActionHandler::testCloseDialog() {
  exec_test(&DialogActionHandler::closeDialog, mDialogManager,
            mMessageDispatcher, mNotifier, mContacts);
}

void TestDialogActionHandler::testCloseDialog_data() {
  QTest::addColumn<DialogMessage::Action>("action");
  QTest::addColumn<std::string>("dialog_id");
  QTest::addColumn<Dialog::Status>("new_status");
  QTest::addColumn<std::string>("adress");
  QTest::addColumn<std::string>("channel");

  QTest::newRow("0") << DialogMessage::Action::CLOSE_DIALOG
                     << std::string("active dialog 1") << Dialog::Status::CLOSED
                     << std::string("adress active1")
                     << std::string("channel 2");
  QTest::newRow("1") << DialogMessage::Action::CLOSE_DIALOG
                     << std::string("active dialog 2") << Dialog::Status::CLOSED
                     << std::string("adress active2")
                     << std::string("channel 2");
  QTest::newRow("2") << DialogMessage::Action::CLOSE_DIALOG
                     << std::string("active dialog 3") << Dialog::Status::CLOSED
                     << std::string("adress active3")
                     << std::string("channel 2");
}

void TestDialogActionHandler::testCancelDialog() {
  exec_test(&DialogActionHandler::cancelDialog, mDialogManager,
            mMessageDispatcher, mNotifier, mContacts);
}

void TestDialogActionHandler::testCancelDialog_data() {
  QTest::addColumn<DialogMessage::Action>("action");
  QTest::addColumn<std::string>("dialog_id");
  QTest::addColumn<Dialog::Status>("new_status");
  QTest::addColumn<std::string>("adress");
  QTest::addColumn<std::string>("channel");

  QTest::newRow("0") << DialogMessage::Action::CANCEL_DIALOG
                     << std::string("created dialog 1")
                     << Dialog::Status::CANCELED
                     << std::string("adress created1")
                     << std::string("channel 3");
  QTest::newRow("1") << DialogMessage::Action::CANCEL_DIALOG
                     << std::string("created dialog 2")
                     << Dialog::Status::CANCELED
                     << std::string("adress created2")
                     << std::string("channel 3");
  QTest::newRow("2") << DialogMessage::Action::CANCEL_DIALOG
                     << std::string("created dialog 3")
                     << Dialog::Status::CANCELED
                     << std::string("adress created3")
                     << std::string("channel 3");
}

void TestDialogActionHandler::testDialogActionRecieve() {
  QFETCH(std::string, dialog_id);
  QFETCH(Dialog::Status, new_status);
  QFETCH(std::string, message);
  QFETCH(std::string, funct);
  QFETCH(int, seqNum);

  auto dialogActionHandler =
      DialogActionHandler(mDialogManager, mMessageDispatcher, mNotifier,
                          mContacts, std::make_shared<CryptoSystemFake>());

  auto marshaller = MessageMarshaller();

  auto mes = marshaller.unmarshall(message, "");

  QVERIFY(mes);
  QCOMPARE(mes->sequential(), static_cast<unsigned long>(seqNum));

  dialogActionHandler.handle(mes.value(), "fake");

  QCOMPARE(mDialogManager->get(dialog_id)->getStatus(), new_status);
}

/*
 *
 * const std::string ACTION_MESSAGE = "MESSAGE";
const std::string ACTION_CREATE_DIALOG = "CREATE_DIALOG";
const std::string ACTION_ACCEPT_DIALOG = "ACCEPT_DIALOG";
const std::string ACTION_CLOSE_DIALOG = "CLOSE_DIALOG";
const std::string ACTION_CANCEL_DIALOG = "CANCEL_DIALOG";
s
 */
void TestDialogActionHandler::testDialogActionRecieve_data() {
  QTest::addColumn<std::string>("dialog_id");
  QTest::addColumn<Dialog::Status>("new_status");
  QTest::addColumn<std::string>("message");
  QTest::addColumn<std::string>("funct");
  QTest::addColumn<int>("seqNum");

  QTest::newRow("2") << std::string("created dialog 1")
                     << Dialog::Status::ACTIVE
                     << std::string(
                            "SECURE DIALOG MESSAGE\n\
VERSION:1.0\n\
ACTION:ACCEPT_DIALOG\n\
SEQUENTAL_NUMBER:1\n\
DIALOG:created dialog 1\n\
TIMESTAMP:1232323\n\
SIGNATURE: 1\n\
--------------------------------------------------------------\n\
") << std::string("accepted")
                     << 1;

  QTest::newRow("3") << std::string("created dialog 2")
                     << Dialog::Status::ACTIVE
                     << std::string(
                            "SECURE DIALOG MESSAGE\n\
VERSION:1.0\n\
ACTION:ACCEPT_DIALOG\n\
SEQUENTAL_NUMBER:1\n\
DIALOG:created dialog 2\n\
TIMESTAMP:1232323\n\
SIGNATURE: 1\n\
--------------------------------------------------------------\n\
") << std::string("accepted")
                     << 1;

  QTest::newRow("4") << std::string("new dialog 3") << Dialog::Status::CANCELED
                     << std::string(
                            "SECURE DIALOG MESSAGE\n\
VERSION:1.0\n\
ACTION:CANCEL_DIALOG\n\
SEQUENTAL_NUMBER:3\n\
DIALOG:new dialog 3\n\
TIMESTAMP:1232323\n\
SIGNATURE: 1\n\
--------------------------------------------------------------\n\
") << std::string("canceled")
                     << 3;

  QTest::newRow("5") << std::string("new dialog 4") << Dialog::Status::CANCELED
                     << std::string(
                            "SECURE DIALOG MESSAGE\n\
VERSION:1.0\n\
ACTION:CANCEL_DIALOG\n\
SEQUENTAL_NUMBER:2\n\
DIALOG:new dialog 4\n\
TIMESTAMP:1232323\n\
SIGNATURE: 1\n\
--------------------------------------------------------------\n\
") << std::string("canceled")
                     << 2;

  QTest::newRow("6") << std::string("active dialog 1") << Dialog::Status::CLOSED
                     << std::string(
                            "SECURE DIALOG MESSAGE\n\
VERSION:1.0\n\
ACTION:CLOSE_DIALOG\n\
SEQUENTAL_NUMBER:2\n\
DIALOG:active dialog 1\n\
TIMESTAMP:1232323\n\
SIGNATURE: 1\n\
--------------------------------------------------------------\n\
") << std::string("closed")
                     << 2;

  QTest::newRow("7") << std::string("active dialog 2") << Dialog::Status::CLOSED
                     << std::string(
                            "SECURE DIALOG MESSAGE\n\
VERSION:1.0\n\
ACTION:CLOSE_DIALOG\n\
SEQUENTAL_NUMBER:3\n\
DIALOG:active dialog 2\n\
TIMESTAMP:1232323\n\
SIGNATURE: 1\n\
--------------------------------------------------------------\n\
") << std::string("closed")
                     << 3;
}

void TestDialogActionHandler::makeAndFillDialogcontainer() {
  auto dialog = [](const std::string& channel, const std::string& name,
                   const std::string& adress, const std::string& dialog_id,
                   Dialog::Status st) {
    return std::make_shared<Dialog>(
        std::make_shared<Contact>(channel, name, adress, ""), dialog_id, st);
  };

  mDialogManager = std::make_shared<DialogManager>();
  mDialogManager->add(dialog("channel 1", "name new1", "adress new1",
                             "new dialog 1", Dialog::Status::NEW));
  mDialogManager->add(dialog("channel 1", "name new2", "adress new2",
                             "new dialog 2", Dialog::Status::NEW));
  mDialogManager->add(dialog("channel 1", "name new3", "adress new3",
                             "new dialog 3", Dialog::Status::NEW));
  mDialogManager->add(dialog("channel 1", "name new4", "adress new4",
                             "new dialog 4", Dialog::Status::NEW));

  mDialogManager->add(dialog("channel 2", "name active1", "adress active1",
                             "active dialog 1", Dialog::Status::ACTIVE));
  mDialogManager->add(dialog("channel 2", "name active2", "adress active2",
                             "active dialog 2", Dialog::Status::ACTIVE));
  mDialogManager->add(dialog("channel 2", "name active3", "adress active3",
                             "active dialog 3", Dialog::Status::ACTIVE));

  mDialogManager->add(dialog("channel 3", "name created1", "adress created1",
                             "created dialog 1",
                             Dialog::Status::CREATE_REQUEST));
  mDialogManager->add(dialog("channel 3", "name created2", "adress created2",
                             "created dialog 2",
                             Dialog::Status::CREATE_REQUEST));
  mDialogManager->add(dialog("channel 3", "name created3", "adress created3",
                             "created dialog 3",
                             Dialog::Status::CREATE_REQUEST));
}

QTEST_APPLESS_MAIN(TestDialogActionHandler)

#include "testdialogactionhandler.moc"
