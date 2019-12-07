#include <QtTest>
#include "primitives/dialogmessage.h"
#include "utils/messagemarshaller.h"

Q_DECLARE_METATYPE(std::string);
Q_DECLARE_METATYPE(DialogMessage::Action);

class DialogMessageCreationFromSourceTest : public QObject {
  Q_OBJECT

 public:
 private Q_SLOTS:
  void testDialogCreatedOk_data();
  void testDialogCreatedOk();
};

void DialogMessageCreationFromSourceTest::testDialogCreatedOk_data() {
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

void DialogMessageCreationFromSourceTest::testDialogCreatedOk() {
  QFETCH(DialogMessage::Action, action);
  QFETCH(std::string, dialog_id);
  QFETCH(std::string, content);
  QFETCH(std::string, check);

  auto marshaller = MessageMarshaller();

  auto message = marshaller.unmarshall(check, "test");
  QVERIFY(message);
  QCOMPARE(message->action(), action);
  QCOMPARE(message->content(), std::string_view(content));
  QCOMPARE(message->dialogId(), std::string_view(dialog_id));
}

QTEST_APPLESS_MAIN(DialogMessageCreationFromSourceTest)

#include "dialogmessagecreationfromsourcetest.moc"
