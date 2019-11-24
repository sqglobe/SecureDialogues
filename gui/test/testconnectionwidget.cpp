#include "cryptosystemfake.h"
#include "fakechanneladapter.h"
#include "fakechanneladaptersendmessageonly.h"
#include "fakeconnectioninfowidget.h"
#include "fakenotifier.h"
#include "faketemplatechangewatcher.h"
#include "fakeuserask.h"

#include <functional>
#include "communication/channel.h"
#include "communication/messagedespatcher.h"
#include "interfaces/abstractchanneladapter.h"
#include "primitives/connectionholder.h"

#include <string>
#include "dialogwidgetgasket.h"

#include <qmetatype.h>
#include <QComboBox>
#include <QLineEdit>
#include <QTest>

#include <memory>

#include <db_cxx.h>
#include "containers/storages.h"
#include "utils/dbfactory.h"

Q_DECLARE_METATYPE(std::string)

template <typename Container, typename Widget>
class DialogWidgetGasket;

class TestConnectionWidget : public QObject {
  Q_OBJECT
 public:
  explicit TestConnectionWidget(QObject* parent = nullptr);

 signals:

 private slots:
  void initTestCase();
  void cleanupTestCase();
  void init();
  void testViewAt();
  void testViewAt_data();

  void testUpdate();
  void testUpdate_data();

  void testAdd();
  void testAdd_data();

  void testEnable();
  void testEnableVsViewAtPos();

  void testCleare();
  void testDisable();

 private:
  std::shared_ptr<ConnectionStorage> mContainer;
  std::shared_ptr<FakeConnectionInfoWidget> mWidget;
  std::shared_ptr<FakeTemplateChangeListener<ConnectionHolder>> mChangeWatcher;
  std::shared_ptr<
      DialogWidgetGasket<ConnectionStorage, FakeConnectionInfoWidget>>
      mGasket;
  Db* primary;
  std::shared_ptr<ContactStorage> mContactStorage;
  std::shared_ptr<DialogStorage> mDialogs;
};

TestConnectionWidget::TestConnectionWidget(QObject* parent) : QObject(parent) {}

void TestConnectionWidget::initTestCase() {
  dbstl::dbstl_startup();
  QDir().mkdir("TestConnectionWidget_env");
  auto penv = make_db_env("TestConnectionWidget_env", "test");
  primary = make_db("test_connections.db", "primary", penv);

  mDialogs = make_dialog_storage(
      make_db("test_dialogs.db", "primary", penv),
      make_db("test_dialogs.db", "secondary", penv, DB_DUP), penv);

  mContactStorage = make_contact_storage(
      make_db("test_contacts.db", "primary", penv),
      make_db("test_contacts.db", "secondary", penv, DB_DUP), penv, mDialogs);

  mContainer = make_connection_storage(primary, penv, mContactStorage);
}

void TestConnectionWidget::cleanupTestCase() {
  dbstl::dbstl_exit();
}

void TestConnectionWidget::init() {
  primary->truncate(nullptr, nullptr, 0);
  auto messageDispatcher = std::make_shared<MessageDespatcher>(
      std::make_shared<CryptoSystemFake>(), std::make_shared<FakeNotifier>());
  mContainer->add(ConnectionHolder(
      GmailConnection{"test1@gmail.com", "09asdkvju"}, "conn 1"));
  mContainer->add(ConnectionHolder(
      GmailConnection{"test2@gmail.com", "09asdkvju"}, "conn 2"));
  mContainer->add(ConnectionHolder(
      GmailConnection{"test3@gmail.com", "09asdkvju"}, "conn 3"));
  mContainer->add(ConnectionHolder(
      GmailConnection{"test4@gmail.com", "09asdkvju"}, "conn 4"));
  mContainer->add(ConnectionHolder(
      GmailConnection{"test5@gmail.com", "09asdkvju"}, "conn 5"));

  mWidget = std::make_shared<FakeConnectionInfoWidget>();

  mChangeWatcher =
      std::make_shared<FakeTemplateChangeListener<ConnectionHolder>>();
  mContainer->appendPermanentListener(mChangeWatcher);
  // DialogWidgetGasket(const std::shared_ptr<Container> &container, Widget
  // *widget, const std::shared_ptr<AbstractUserAsk> &userAsk,
  // std::shared_ptr<AbstractUserNotifier> notifier);
  mGasket = std::make_shared<
      DialogWidgetGasket<ConnectionStorage, FakeConnectionInfoWidget>>(
      mContainer, mWidget.get(), std::make_shared<FakeUserAsk>(true),
      std::make_shared<FakeNotifier>());
}

void TestConnectionWidget::testViewAt() {
  QFETCH(std::string, connection);
  QFETCH(std::string, login);
  QFETCH(std::string, pass);
  QFETCH(std::string, viewPos);

  mGasket->viewAt(viewPos);
  auto conn_name_field = mWidget->findChild<QLineEdit*>("connectionName");
  auto login_field = mWidget->findChild<QLineEdit*>("login");
  auto pass_field = mWidget->findChild<QLineEdit*>("pass");

  QCOMPARE(conn_name_field->text().toStdString(), connection);
  QCOMPARE(login_field->text().toStdString(), login);
  QCOMPARE(pass_field->text().toStdString(), pass);
}

void TestConnectionWidget::testViewAt_data() {
  QTest::addColumn<std::string>("connection");
  QTest::addColumn<std::string>("login");
  QTest::addColumn<std::string>("pass");
  QTest::addColumn<std::string>("viewPos");

  QTest::newRow("0") << std::string("conn 1") << std::string("test1@gmail.com")
                     << std::string("") << std::string("conn 1");

  QTest::newRow("1") << std::string("conn 2") << std::string("test2@gmail.com")
                     << std::string("") << std::string("conn 2");

  QTest::newRow("2") << std::string("conn 3") << std::string("test3@gmail.com")
                     << std::string("") << std::string("conn 3");

  QTest::newRow("3") << std::string("conn 4") << std::string("test4@gmail.com")
                     << std::string("") << std::string("conn 4");

  QTest::newRow("4") << std::string("conn 5") << std::string("test5@gmail.com")
                     << std::string("") << std::string("conn 5");
}

void TestConnectionWidget::testUpdate() {
  QFETCH(std::string, new_login);
  QFETCH(std::string, new_pass);
  QFETCH(std::string, viewPos);

  mGasket->viewAt(viewPos);
  auto login_field = mWidget->findChild<QLineEdit*>("login");
  auto pass_field = mWidget->findChild<QLineEdit*>("pass");

  login_field->setText(new_login.c_str());
  pass_field->setText(new_pass.c_str());

  /*
  mGasket->update();
  QCOMPARE(mChangeWatcher->mMethod, std::string("changed"));
  QCOMPARE(mChangeWatcher->mVal->getType(), ConnectionType::GMAIL);
  auto connParam = mChangeWatcher->mVal->getConnection<GmailConnaection>();
  QCOMPARE(connParam.email, new_login);
  QCOMPARE(connParam.accessToken, new_pass);
  */
}

void TestConnectionWidget::testUpdate_data() {
  QTest::addColumn<std::string>("new_login");
  QTest::addColumn<std::string>("new_pass");
  QTest::addColumn<std::string>("viewPos");

  QTest::newRow("0") << std::string("login 212") << std::string("pass 1")
                     << std::string("conn 1");
  QTest::newRow("1") << std::string("login 2") << std::string("pass 2312")
                     << std::string("conn 2");
  QTest::newRow("2") << std::string("login 3") << std::string("pass 3231")
                     << std::string("conn 3");
  QTest::newRow("3") << std::string("login 4") << std::string("pass 41")
                     << std::string("conn 4");
  QTest::newRow("4") << std::string("login 5312") << std::string("pass 5")
                     << std::string("conn 5");
}

void TestConnectionWidget::testAdd() {
  QFETCH(std::string, connection);

  auto conn_name_field = mWidget->findChild<QLineEdit*>("connectionName");
  auto connType_field = mWidget->findChild<QComboBox*>("connectionType");

  connType_field->setCurrentIndex(static_cast<int>(ConnectionType::UDP));
  conn_name_field->setText(connection.c_str());

  mGasket->add();

  QCOMPARE(mChangeWatcher->mMethod, std::string("added"));
  QCOMPARE(mChangeWatcher->mVal->getType(), ConnectionType::UDP);
  QCOMPARE(mChangeWatcher->mVal->getConnectionName(), connection);
}

void TestConnectionWidget::testAdd_data() {
  QTest::addColumn<std::string>("connection");
  QTest::addColumn<std::string>("login");
  QTest::addColumn<std::string>("pass");

  QTest::newRow("0") << std::string("new conn 1");
  QTest::newRow("1") << std::string("new conn 2");
  QTest::newRow("2") << std::string("new conn 3");
  QTest::newRow("3") << std::string("new conn 4");
  QTest::newRow("4") << std::string("new conn 5");
}

void TestConnectionWidget::testEnable() {
  mWidget->actionDisable();
  auto conn_name_field = mWidget->findChild<QLineEdit*>("connectionName");
  auto login_field = mWidget->findChild<QLineEdit*>("login");
  auto pass_field = mWidget->findChild<QLineEdit*>("pass");
  auto connType = mWidget->findChild<QComboBox*>("connectionType");

  mWidget->actionEnable();

  QVERIFY(conn_name_field->isEnabled());
  QVERIFY(login_field->isEnabled());
  QVERIFY(pass_field->isEnabled());
  QVERIFY(connType->isEnabled());
}

void TestConnectionWidget::testEnableVsViewAtPos() {
  mGasket->viewAt("conn 1");
  auto conn_name_field = mWidget->findChild<QLineEdit*>("connectionName");
  auto login_field = mWidget->findChild<QLineEdit*>("login");
  auto pass_field = mWidget->findChild<QLineEdit*>("pass");
  auto connType = mWidget->findChild<QComboBox*>("connectionType");

  mWidget->actionEnable();

  QVERIFY(!conn_name_field->isEnabled());
  QVERIFY(login_field->isEnabled());
  QVERIFY(pass_field->isEnabled());
  QVERIFY(connType->isEnabled());
}

void TestConnectionWidget::testCleare() {
  mGasket->viewAt("conn 1");
  auto conn_name_field = mWidget->findChild<QLineEdit*>("connectionName");
  auto login_field = mWidget->findChild<QLineEdit*>("login");
  auto pass_field = mWidget->findChild<QLineEdit*>("pass");

  mWidget->actionCleare();

  QVERIFY(conn_name_field->text().isEmpty());
  QVERIFY(login_field->text().isEmpty());
  QVERIFY(pass_field->text().isEmpty());
}

void TestConnectionWidget::testDisable() {
  mWidget->actionEnable();
  auto conn_name_field = mWidget->findChild<QLineEdit*>("connectionName");
  auto login_field = mWidget->findChild<QLineEdit*>("login");
  auto pass_field = mWidget->findChild<QLineEdit*>("pass");
  auto connType = mWidget->findChild<QComboBox*>("connectionType");

  mWidget->actionDisable();

  QVERIFY(!conn_name_field->isEnabled());
  QVERIFY(!login_field->isEnabled());
  QVERIFY(!pass_field->isEnabled());
  QVERIFY(!connType->isEnabled());
}

QTEST_MAIN(TestConnectionWidget)

#include "testconnectionwidget.moc"
