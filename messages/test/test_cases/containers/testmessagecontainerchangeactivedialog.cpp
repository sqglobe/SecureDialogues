#include <QTest>
#include "containers/messagecontainer.h"
#include "fakemessagecontainerhandler.h"

#include <iostream>
#include <memory>

Q_DECLARE_METATYPE(std::string);

class MessageContainer;
class FakeMessageContainerHandlerUpdateActiveDialog;

class TestMessageContainerChangeActiveDialog : public QObject {
  Q_OBJECT

 private Q_SLOTS:
  void init();
  void testActiveDialogChange();
  void testActiveDialogChange_data();

 private:
  std::shared_ptr<FakeMessageContainerHandlerUpdateActiveDialog> mFake;
  std::shared_ptr<MessageContainer> mMessageContainer;
};

void TestMessageContainerChangeActiveDialog::init() {
  mFake = std::make_shared<FakeMessageContainerHandlerUpdateActiveDialog>();
  mMessageContainer = std::make_shared<MessageContainer>();
  mMessageContainer->setActiveDialog("test_test123");
  mMessageContainer->registerHandler(mFake);
}

void TestMessageContainerChangeActiveDialog::testActiveDialogChange() {
  QFETCH(std::string, dialog_id);
  QFETCH(std::string, message);
  QFETCH(bool, isIncomming);

  mMessageContainer->addMessage(dialog_id, message, isIncomming);
  mMessageContainer->setActiveDialog(dialog_id);

  QCOMPARE(mFake->mActiveDialogId, dialog_id);
  QCOMPARE(mFake->mIsIncomming, isIncomming);
  QCOMPARE(mFake->mMessage, message);
}

void TestMessageContainerChangeActiveDialog::testActiveDialogChange_data() {
  QTest::addColumn<std::string>("dialog_id");
  QTest::addColumn<std::string>("message");
  QTest::addColumn<bool>("isIncomming");

  QTest::newRow("0") << std::string("test1") << std::string("hi me") << false;
  QTest::newRow("1") << std::string("xc334") << std::string("21321asds asd")
                     << true;
  QTest::newRow("2") << std::string("test1") << std::string("hi me 132 easdas")
                     << true;
  QTest::newRow("3") << std::string("test1") << std::string("21321asdasd")
                     << false;
  QTest::newRow("4") << std::string("2321312 213213")
                     << std::string("567987*9*/123") << true;
  QTest::newRow("5") << std::string("xc334") << std::string("12312saa 03984")
                     << false;
}

QTEST_APPLESS_MAIN(TestMessageContainerChangeActiveDialog)

#include "testmessagecontainerchangeactivedialog.moc"
