#include "fakechanneladapter.h"
#include "fakechanneladaptersendmessageonly.h"
#include "fakenotifier.h"
#include "faketemplatechangewatcher.h"
#include "fakeuserask.h"

#include <functional>
#include "communication/channel.h"
#include "communication/messagedespatcher.h"
#include "containers/contactcontainer.h"
#include "interfaces/abstractchanneladapter.h"
#include "primitives/connectionholder.h"
#include "primitives/contact.h"

#include "dialogwidgetgasket.h"
#include "models/channelslistmodel.h"
#include "widgets/contactwidget.h"

#include <QComboBox>
#include <QLineEdit>
#include <QTest>
#include <QTextEdit>

#include <memory>

#include <iostream>

Q_DECLARE_METATYPE(std::string)

const std::string PUBLIC_KEY =
    "MIIBoDANBgkqhkiG9w0BAQEFAAOCAY0AMIIBiAKCAYEAyzs2Zuo+Efa08SGVQVOHeANtLTrhu7iP90tZrmUD67DxHRz3rJfjxrkl\
        jJ9FsIitY1EMzyJKJ8dgoybUI65XNlPqYXClT+bQoX8ZkORzcwFGGrBVXMzjQSGGN8DbGZYKRiMiInKRRh+Xzt1xJoahVKNtrbhI\
        BimgOSl5uspcn6giAMQGCoh0AO4PDLzrcqMoysPUxQujrD+SLE///36t8LIJsOgh8iy66dAjS0xR9vhCnlW7V6Rhu8u/adex+j7r\
        vZFdRNfcDr1xqkN9hwdNLTJ1BogSaLK+5cTatu2FxwAbOz3K4Dmlo9Lj0vHiIOT/4uRkFBbMGM4WdRwB1KPjLJRse2C4T/7AyoMV\
        jkbUv19qS26gbEuqGjQ4CjFXXJW+JtKOogor3Zz/aFGSsMuMpyIm9TfyEJqrZjj8zuBhLDUq77NZb5eMJOrXHiSlJCQqb1Uhj4bL\
        Qe6FzwRJXlY8MRHVPId/Hd6TsnlaJ+YM84rMCIlxszBlOpR/oqQRr+RZAgER";

class TestContactWidget : public QObject {
  Q_OBJECT
 public:
  explicit TestContactWidget(QObject* parent = nullptr);

 private slots:
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
  std::shared_ptr<ContactContainer> mContainer;
  std::shared_ptr<ContactWidget> mWidget;
  std::shared_ptr<FakeTemplateChangeWatcher<std::shared_ptr<const Contact>>>
      mChangeWatcher;
  std::shared_ptr<DialogWidgetGasket<ContactContainer, ContactWidget>> mGasket;
};

TestContactWidget::TestContactWidget(QObject* parent) : QObject(parent) {
  // ChannelsListModel(const std::vector<ConnectionInfo> &elements);
}

void TestContactWidget::init() {
  std::vector<ConnectionHolder> infos = {
      ConnectionHolder(GmailConnection{"login1@gmail.com", "tesr23"}, "conn 1"),
      ConnectionHolder(UdpConnection{}, "conn 2"),
      ConnectionHolder(UdpConnection{}, "conn 3"),
      ConnectionHolder(UdpConnection{}, "conn 4"),
      ConnectionHolder(UdpConnection{}, "conn 5")};

  mContainer = std::make_shared<ContactContainer>();
  mWidget = std::make_shared<ContactWidget>(
      std::make_shared<ChannelsListModel>(infos));
  mChangeWatcher = std::make_shared<
      FakeTemplateChangeWatcher<std::shared_ptr<const Contact>>>();
  mContainer->registerWatcher(mChangeWatcher);
  mGasket =
      std::make_shared<DialogWidgetGasket<ContactContainer, ContactWidget>>(
          mContainer, mWidget.get(), std::make_shared<FakeUserAsk>(true),
          std::make_shared<FakeNotifier>());
  // Contact(const std::string &channelMoniker, const std::string &name, const
  // std::string &adress, const std::string &id);
  mContainer->add(std::make_shared<Contact>(
      "conn 1", "name conn 1 1", "addr1@gmail.com", PUBLIC_KEY, "id 1"));
  mContainer->add(std::make_shared<Contact>(
      "conn 1", "name conn 1 2", "addr2@gmail.com", PUBLIC_KEY, "id 2"));
  mContainer->add(std::make_shared<Contact>("conn 2", "name conn 2 3",
                                            "adress 3", PUBLIC_KEY, "id 3"));
  mContainer->add(std::make_shared<Contact>("conn 3", "name conn 3 4",
                                            "adress 4", PUBLIC_KEY, "id 4"));
  mContainer->add(std::make_shared<Contact>("conn 4", "name conn 4 5",
                                            "adress 5", PUBLIC_KEY, "id 5"));
  mContainer->add(std::make_shared<Contact>("conn 4", "name conn 4 6",
                                            "adress 6", PUBLIC_KEY, "id 6"));
  mContainer->add(std::make_shared<Contact>("conn 5", "name conn 5 7",
                                            "adress 7", PUBLIC_KEY, "id 7"));
  mContainer->add(std::make_shared<Contact>("conn 5", "name conn 5 8",
                                            "adress 8", PUBLIC_KEY, "id 8"));
}

void TestContactWidget::testViewAt() {
  QFETCH(std::string, connection);
  QFETCH(std::string, name);
  QFETCH(std::string, adress);
  QFETCH(int, viewPos);

  mGasket->viewAt(viewPos);

  auto userAdress = mWidget->findChild<QLineEdit*>("contactAdress");
  auto userName = mWidget->findChild<QLineEdit*>("contactName");
  auto connNames = mWidget->findChild<QComboBox*>("channelMonikers");
  QCOMPARE(connNames->currentText().toStdString(), connection);
  QCOMPARE(userName->text().toStdString(), name);
  QCOMPARE(userAdress->text().toStdString(), adress);
}

void TestContactWidget::testViewAt_data() {
  QTest::addColumn<std::string>("connection");
  QTest::addColumn<std::string>("name");
  QTest::addColumn<std::string>("adress");
  QTest::addColumn<int>("viewPos");

  QTest::newRow("0") << std::string("conn 1") << std::string("name conn 1 1")
                     << std::string("addr1@gmail.com") << 0;
  QTest::newRow("1") << std::string("conn 1") << std::string("name conn 1 2")
                     << std::string("addr2@gmail.com") << 1;
  QTest::newRow("2") << std::string("conn 2") << std::string("name conn 2 3")
                     << std::string("adress 3") << 2;
  QTest::newRow("3") << std::string("conn 3") << std::string("name conn 3 4")
                     << std::string("adress 4") << 3;
  QTest::newRow("4") << std::string("conn 4") << std::string("name conn 4 5")
                     << std::string("adress 5") << 4;
  QTest::newRow("5") << std::string("conn 4") << std::string("name conn 4 6")
                     << std::string("adress 6") << 5;
  QTest::newRow("6") << std::string("conn 5") << std::string("name conn 5 7")
                     << std::string("adress 7") << 6;
  QTest::newRow("7") << std::string("conn 5") << std::string("name conn 5 8")
                     << std::string("adress 8") << 7;
}

void TestContactWidget::testUpdate() {
  QFETCH(std::string, new_connection);
  QFETCH(std::string, new_name);
  QFETCH(std::string, new_adress);
  QFETCH(std::string, old_id);
  QFETCH(int, viewPos);

  mGasket->viewAt(viewPos);

  auto userAdress = mWidget->findChild<QLineEdit*>("contactAdress");
  auto userName = mWidget->findChild<QLineEdit*>("contactName");
  auto connNames = mWidget->findChild<QComboBox*>("channelMonikers");

  userAdress->setText(new_adress.c_str());
  userName->setText(new_name.c_str());
  connNames->setCurrentText(new_connection.c_str());

  mGasket->update();

  QCOMPARE(mChangeWatcher->mMethod, std::string("changed"));
  QCOMPARE(mChangeWatcher->mVal->get()->adress(), new_adress);
  QCOMPARE(mChangeWatcher->mVal->get()->channelMoniker(), new_connection);
  QCOMPARE(mChangeWatcher->mVal->get()->name(), new_name);
  QCOMPARE(mChangeWatcher->mVal->get()->id(), old_id);
}

void TestContactWidget::testUpdate_data() {
  QTest::addColumn<std::string>("new_connection");
  QTest::addColumn<std::string>("new_name");
  QTest::addColumn<std::string>("new_adress");
  QTest::addColumn<std::string>("old_id");
  QTest::addColumn<int>("viewPos");

  QTest::newRow("0") << std::string("conn 1")
                     << std::string("name conn fake new")
                     << std::string("addr-new@gmail.com") << std::string("id 1")
                     << 0;
  QTest::newRow("1") << std::string("conn 1") << std::string("name conn 1 2")
                     << std::string("addr2@gmail.com") << std::string("id 2")
                     << 1;
  QTest::newRow("2") << std::string("conn 5") << std::string("name conn 2 3")
                     << std::string("adress 3") << std::string("id 3") << 2;
  QTest::newRow("3") << std::string("conn 3") << std::string("name conn 3 4")
                     << std::string("adress new") << std::string("id 4") << 3;
  QTest::newRow("4") << std::string("conn 4") << std::string("name conn test")
                     << std::string("adress 5") << std::string("id 5") << 4;
  QTest::newRow("5") << std::string("conn 1") << std::string("name conn 4 6")
                     << std::string("addr5@gmail.com") << std::string("id 6")
                     << 5;
  QTest::newRow("6") << std::string("conn 3")
                     << std::string("name conn fale _1")
                     << std::string("adress 7") << std::string("id 7") << 6;
  QTest::newRow("7") << std::string("conn 5") << std::string("name conn 5 8")
                     << std::string("adress old") << std::string("id 8") << 7;
}

void TestContactWidget::testAdd() {
  QFETCH(std::string, new_connection);
  QFETCH(std::string, new_name);
  QFETCH(std::string, new_adress);

  auto userAdress = mWidget->findChild<QLineEdit*>("contactAdress");
  auto userName = mWidget->findChild<QLineEdit*>("contactName");
  auto connNames = mWidget->findChild<QComboBox*>("channelMonikers");
  auto pub = mWidget->findChild<QTextEdit*>("publicKey");

  connNames->setCurrentText(new_connection.c_str());
  userAdress->setText(new_adress.c_str());
  userName->setText(new_name.c_str());
  pub->setPlainText(PUBLIC_KEY.c_str());

  mGasket->add();
  QCOMPARE(mChangeWatcher->mMethod, std::string("added"));
  QCOMPARE(mChangeWatcher->mVal->get()->adress(), new_adress);
  QCOMPARE(mChangeWatcher->mVal->get()->channelMoniker(), new_connection);
  QCOMPARE(mChangeWatcher->mVal->get()->name(), new_name);
}

void TestContactWidget::testAdd_data() {
  QTest::addColumn<std::string>("new_connection");
  QTest::addColumn<std::string>("new_name");
  QTest::addColumn<std::string>("new_adress");

  QTest::newRow("0") << std::string("conn 1") << std::string("added name 1")
                     << std::string("addr-added-1@gmail.com");
  QTest::newRow("1") << std::string("conn 1") << std::string("added name 2")
                     << std::string("addr-added-2@gmail.com");
  QTest::newRow("2") << std::string("conn 5") << std::string("added name 3")
                     << std::string("added new adress 3");
  QTest::newRow("3") << std::string("conn 3") << std::string("added name 4")
                     << std::string("added new adress 4");
  QTest::newRow("4") << std::string("conn 4") << std::string("added name 5")
                     << std::string("added new adress 5");
  QTest::newRow("5") << std::string("conn 1") << std::string("added name 6")
                     << std::string("addr-added-6@gmail.com");
}

void TestContactWidget::testEnable() {
  mWidget->actionDisable();

  auto userAdress = mWidget->findChild<QLineEdit*>("contactAdress");
  auto userName = mWidget->findChild<QLineEdit*>("contactName");
  auto connNames = mWidget->findChild<QComboBox*>("channelMonikers");

  mWidget->actionEnable();

  QVERIFY(userAdress->isEnabled());
  QVERIFY(userName->isEnabled());
  QVERIFY(connNames->isEnabled());
}

void TestContactWidget::testEnableVsViewAtPos() {
  mGasket->viewAt(0);

  auto userAdress = mWidget->findChild<QLineEdit*>("contactAdress");
  auto userName = mWidget->findChild<QLineEdit*>("contactName");
  auto connNames = mWidget->findChild<QComboBox*>("channelMonikers");

  mWidget->actionEnable();

  QVERIFY(userAdress->isEnabled());
  QVERIFY(userName->isEnabled());
  QVERIFY(connNames->isEnabled());
}

void TestContactWidget::testCleare() {
  mGasket->viewAt(0);

  auto userAdress = mWidget->findChild<QLineEdit*>("contactAdress");
  auto userName = mWidget->findChild<QLineEdit*>("contactName");

  mWidget->actionCleare();

  QVERIFY(userName->text().isEmpty());
  QVERIFY(userAdress->text().isEmpty());
}

void TestContactWidget::testDisable() {
  mWidget->actionEnable();

  auto userAdress = mWidget->findChild<QLineEdit*>("contactAdress");
  auto userName = mWidget->findChild<QLineEdit*>("contactName");
  auto connNames = mWidget->findChild<QComboBox*>("channelMonikers");

  mWidget->actionDisable();

  QVERIFY(!userAdress->isEnabled());
  QVERIFY(!userName->isEnabled());
  QVERIFY(!connNames->isEnabled());
}

QTEST_MAIN(TestContactWidget)

#include "testcontactwidget.moc"
