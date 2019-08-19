#include <QTest>
#include <memory>
#include "primitives/contact.h"
#include "primitives/dialog.h"

#include "containers/storages.h"
#include "interfaces/changelistener.h"

#include <dbstl_common.h>
#include "utils/dbfactory.h"

Q_DECLARE_METATYPE(std::string)

Contact make_contact(std::string channelMoniker,
                     std::string name,
                     std::string adress,
                     std::string key,
                     std::string id) {
  return Contact(std::move(channelMoniker), std::move(name), std::move(adress),
                 std::move(key), std::move(id));
}

class FakeContactWatcher : public ChangeListener<Contact> {
 public:
  void added(const element& obj) override {
    mMethod = "added";
    mId = obj.id();
    mChannel = obj.channelMoniker();
    mAdress = obj.adress();
    mName = obj.name();
  }
  void changed(const element& obj) override {
    mMethod = "changed";
    mId = obj.id();
    mChannel = obj.channelMoniker();
    mAdress = obj.adress();
    mName = obj.name();
  }
  void removed(const element& obj) override {
    mMethod = "removed";
    mId = obj.id();
    mChannel = obj.channelMoniker();
    mAdress = obj.adress();
    mName = obj.name();
  }

 public:
  std::string mMethod;
  std::string mId;
  std::string mChannel;
  std::string mAdress;
  std::string mName;
};

class TestContactChangeWatcher : public QObject {
  Q_OBJECT
 public:
  explicit TestContactChangeWatcher(QObject* parent = nullptr);

 signals:

 private slots:
  void initTestCase();
  void cleanupTestCase();
  void testAddWatcher();
  void testAddWatcher_data();

  void testChangeWatcher();
  void testChangeWatcher_data();

  void testRemoveWatcher();
  void testRemoveWatcher_data();

  void init();

 private:
  std::shared_ptr<ContactStorage> mContactStorage;
  std::shared_ptr<DialogStorage> mDialogStorage;
  std::shared_ptr<FakeContactWatcher> mWatcher;
  Db* primary;
  Db* secondary;
};

TestContactChangeWatcher::TestContactChangeWatcher(QObject* parent) :
    QObject(parent) {}

void TestContactChangeWatcher::initTestCase() {
  dbstl::dbstl_startup();
  QDir().mkdir("TestContactChangeWatcher_env");
  auto penv = make_db_env("TestContactChangeWatcher_env", "test");
  primary = make_db("test_contacts.db", "primary", penv);
  secondary = make_db("test_contacts.db", "secondary", penv, DB_DUP);

  mDialogStorage = make_dialog_storage(
      make_db("test_dialogs.db", "primary", penv),
      make_db("test_dialogs.db", "secondary", penv, DB_DUP), penv);

  mContactStorage =
      make_contact_storage(primary, secondary, penv, mDialogStorage);
}

void TestContactChangeWatcher::cleanupTestCase() {
  dbstl::dbstl_exit();
  QDir().rmdir("TestContactChangeWatcher_env");
}

void TestContactChangeWatcher::testAddWatcher() {
  QFETCH(std::string, channel);
  QFETCH(std::string, name);
  QFETCH(std::string, adress);
  QFETCH(std::string, contact_id);

  mContactStorage->add(
      make_contact(channel, name, adress, "no pass", contact_id));

  QCOMPARE(mWatcher->mChannel, channel);
  QCOMPARE(mWatcher->mMethod, std::string("added"));
  QCOMPARE(mWatcher->mId, contact_id);
  QCOMPARE(mWatcher->mAdress, adress);
  QCOMPARE(mWatcher->mName, name);
}

void TestContactChangeWatcher::testAddWatcher_data() {
  QTest::addColumn<std::string>("channel");
  QTest::addColumn<std::string>("name");
  QTest::addColumn<std::string>("adress");
  QTest::addColumn<std::string>("contact_id");

  QTest::newRow("0") << std::string("added channel 1")
                     << std::string("fake name 1")
                     << std::string("fake address 1")
                     << std::string("fake added id 1");
  QTest::newRow("1") << std::string("added channel 2")
                     << std::string("fake name 2")
                     << std::string("fake address 2")
                     << std::string("fake added id 2");
  QTest::newRow("2") << std::string("added channel 3")
                     << std::string("fake name 3")
                     << std::string("fake address 3")
                     << std::string("fake added id 3");
  QTest::newRow("3") << std::string("added channel 4")
                     << std::string("fake name 4")
                     << std::string("fake address 4")
                     << std::string("fake added id 4");
}

void TestContactChangeWatcher::testChangeWatcher() {
  QFETCH(std::string, channel);
  QFETCH(std::string, name);
  QFETCH(std::string, adress);
  QFETCH(std::string, contact_id);

  mContactStorage->update(
      make_contact(channel, name, adress, "no pass", contact_id));

  QCOMPARE(mWatcher->mChannel, channel);
  QCOMPARE(mWatcher->mMethod, std::string("changed"));
  QCOMPARE(mWatcher->mId, contact_id);
  QCOMPARE(mWatcher->mAdress, adress);
  QCOMPARE(mWatcher->mName, name);
}

void TestContactChangeWatcher::testChangeWatcher_data() {
  QTest::addColumn<std::string>("channel");
  QTest::addColumn<std::string>("name");
  QTest::addColumn<std::string>("adress");
  QTest::addColumn<std::string>("contact_id");

  QTest::newRow("0") << std::string("added channel 1")
                     << std::string("fake name 1")
                     << std::string("fake address 1") << std::string("id 1");
  QTest::newRow("1") << std::string("added channel 2")
                     << std::string("fake name 2")
                     << std::string("fake address 2") << std::string("id 2");
  QTest::newRow("2") << std::string("added channel 3")
                     << std::string("fake name 3")
                     << std::string("fake address 3") << std::string("id 3");

  QTest::newRow("3") << std::string("added channel 4")
                     << std::string("fake name 4")
                     << std::string("fake address 4") << std::string("id 4");
}

void TestContactChangeWatcher::testRemoveWatcher() {
  QFETCH(std::string, channel);
  QFETCH(std::string, name);
  QFETCH(std::string, adress);
  QFETCH(std::string, contact_id);

  std::cout << mDialogStorage->size() << std::endl;

  mContactStorage->remove(contact_id);

  QCOMPARE(mWatcher->mChannel, channel);
  QCOMPARE(mWatcher->mMethod, std::string("removed"));
  QCOMPARE(mWatcher->mId, contact_id);
  QCOMPARE(mWatcher->mAdress, adress);
  QCOMPARE(mWatcher->mName, name);
}

void TestContactChangeWatcher::testRemoveWatcher_data() {
  QTest::addColumn<std::string>("channel");
  QTest::addColumn<std::string>("name");
  QTest::addColumn<std::string>("adress");
  QTest::addColumn<std::string>("contact_id");

  QTest::newRow("0") << std::string("channel 1") << std::string("name 1")
                     << std::string("adress 1") << std::string("id 1");
  QTest::newRow("1") << std::string("channel 2") << std::string("name 2")
                     << std::string("adress 2") << std::string("id 2");
  QTest::newRow("2") << std::string("channel 3") << std::string("name 3")
                     << std::string("adress 3") << std::string("id 3");
  QTest::newRow("3") << std::string("channel 4") << std::string("name 4")
                     << std::string("adress 4") << std::string("id 4");
}

void TestContactChangeWatcher::init() {
  primary->truncate(nullptr, nullptr, 0);

  mContactStorage->add(
      make_contact("channel 1", "name 1", "adress 1", "no pass", "id 1"));
  mContactStorage->add(
      make_contact("channel 2", "name 2", "adress 2", "no pass", "id 2"));
  mContactStorage->add(
      make_contact("channel 3", "name 3", "adress 3", "no pass", "id 3"));
  mContactStorage->add(
      make_contact("channel 4", "name 4", "adress 4", "no pass", "id 4"));

  mWatcher = std::make_shared<FakeContactWatcher>();

  mContactStorage->appendPermanentListener(mWatcher);
}

QTEST_APPLESS_MAIN(TestContactChangeWatcher)

#include "testcontactchangewatcher.moc"
