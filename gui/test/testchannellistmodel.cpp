#include "communication/messagedespatcher.h"
#include "fakechanneladaptersendmessageonly.h"

#include "cryptosystemfake.h"
#include "fakechanneladapter.h"
#include "fakenotifier.h"
#include "models/channelslistmodel.h"

#include <QTest>

#include <db_cxx.h>
#include <memory>
#include "containers/storages.h"
#include "utils/dbfactory.h"

class TestChannelListModel : public QObject {
  Q_OBJECT
 public:
  explicit TestChannelListModel(QObject* parent = nullptr);

 signals:

 private slots:
  void init();
  void initTestCase();
  void cleanupTestCase();
  void testAddElement();
  void testAddElement_data();

 private:
  std::shared_ptr<ConnectionStorage> mContainer;
  std::shared_ptr<ChannelsListModel> mModel;

  Db* primary;
  std::shared_ptr<ContactStorage> mContactStorage;
  std::shared_ptr<DialogStorage> mDialogs;
};

TestChannelListModel::TestChannelListModel(QObject* parent) : QObject(parent) {}

void TestChannelListModel::initTestCase() {
  QDir().mkdir("TestChannelListModel_env");
  dbstl::dbstl_startup();
  auto penv = make_db_env("TestChannelListModel_env", "test");
  primary = make_db("test_connections.db", "primary", penv);

  mDialogs = make_dialog_storage(
      make_db("test_dialogs.db", "primary", penv),
      make_db("test_dialogs.db", "secondary", penv, DB_DUP), penv);

  mContactStorage = make_contact_storage(
      make_db("test_contacts.db", "primary", penv),
      make_db("test_contacts.db", "secondary", penv, DB_DUP), penv, mDialogs);

  mContainer = make_connection_storage(primary, penv, mContactStorage);
}

void TestChannelListModel::cleanupTestCase() {
  dbstl::dbstl_exit();
  QDir().rmdir("TestChannelListModel_env");
}

void TestChannelListModel::init() {
  primary->truncate(nullptr, nullptr, 0);
  auto messageDispatcher = std::make_shared<MessageDespatcher>(
      std::make_shared<CryptoSystemFake>(), std::make_shared<FakeNotifier>());

  mContainer->add(ConnectionHolder(UdpConnection{}, "conn 1"));
  mContainer->add(ConnectionHolder(UdpConnection{}, "conn 2"));
  mContainer->add(ConnectionHolder(UdpConnection{}, "conn 3"));
  mContainer->add(ConnectionHolder(UdpConnection{}, "conn 4"));

  mModel = std::make_shared<ChannelsListModel>(mContainer->getAllElements());
  mContainer->appendPermanentListener(mModel);
}

void TestChannelListModel::testAddElement() {
  QFETCH(QString, added_name);
  QFETCH(int, added_pos);

  mContainer->add(ConnectionHolder(UdpConnection{}, added_name.toStdString()));
  auto data = mModel->data(mModel->index(added_pos, 0)).toString();

  QCOMPARE(data, added_name);
}

void TestChannelListModel::testAddElement_data() {
  QTest::addColumn<QString>("added_name");
  QTest::addColumn<int>("added_pos");

  QTest::newRow("0") << QString("added 1") << 4;
  QTest::newRow("1") << QString("added 2") << 4;
  QTest::newRow("2") << QString("added 3") << 4;
  QTest::newRow("3") << QString("added 4") << 4;
}

QTEST_MAIN(TestChannelListModel)
#include "testchannellistmodel.moc"
