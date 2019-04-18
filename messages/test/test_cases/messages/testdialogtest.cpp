#include <QtTest>
#include "primitives/contact.h"
#include "primitives/dialog.h"
#include "primitives/dialogmessage.h"

Q_DECLARE_METATYPE(std::string);
Q_DECLARE_METATYPE(DialogMessage::Action);
Q_DECLARE_METATYPE(Dialog::Status);

class TestDialogTest : public QObject {
  Q_OBJECT

 public:
  TestDialogTest();

 private Q_SLOTS:
  void testDialogCreation_data();
  void testDialogCreation();
};

TestDialogTest::TestDialogTest() {}

void TestDialogTest::testDialogCreation_data() {
  QTest::addColumn<std::string>("adress");
  QTest::addColumn<std::string>("channelMoniker");
  QTest::addColumn<std::string>("dialogId");
  QTest::addColumn<DialogMessage::Action>("action");
  QTest::addColumn<Dialog::Status>("status");

  QTest::newRow("0") << std::string("address0") << std::string("fake_channel")
                     << std::string("fake_dialog0")
                     << DialogMessage::Action::MESSAGE
                     << Dialog::Status::ACTIVE;
  QTest::newRow("1") << std::string("address1") << std::string("fake_channel")
                     << std::string("fake_dialog1")
                     << DialogMessage::Action::CLOSE_DIALOG
                     << Dialog::Status::ACTIVE;
  QTest::newRow("2") << std::string("address1") << std::string("fake_channel")
                     << std::string("fake_dialog1")
                     << DialogMessage::Action::CREATE_DIALOG
                     << Dialog::Status::NEW;
}

void TestDialogTest::testDialogCreation() {
  QFETCH(std::string, adress);
  QFETCH(std::string, channelMoniker);
  QFETCH(std::string, dialogId);
  QFETCH(DialogMessage::Action, action);
  QFETCH(Dialog::Status, status);

  std::string content("FAKE CONTENT");
  auto contact = std::make_shared<Contact>(channelMoniker, "Fake", adress, "");
  Dialog dialog(contact, dialogId, status);
  auto message = dialog.makeMessage(action, content);

  QCOMPARE(message.action(), action);
  QCOMPARE(message.content(), content);
  QCOMPARE(message.dialogId(), dialogId);
}

QTEST_APPLESS_MAIN(TestDialogTest)

#include "testdialogtest.moc"
