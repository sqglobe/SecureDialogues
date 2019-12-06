#include <QTest>
#include <memory>
#include "primitives/discoveredcontact.h"

#include "containers/storages.h"
#include "interfaces/changelistener.h"

#include <dbstl_common.h>
#include "utils/dbfactory.h"

Q_DECLARE_METATYPE(std::string)
Q_DECLARE_METATYPE(std::chrono::time_point<std::chrono::system_clock>::duration)

class FakeWatcher : public ChangeListener<DiscoveredContact> {
 public:
  void added(const element& obj) override {
    mMethod = "added";
    contact = obj;
  }
  void changed(const element& obj) override {
    mMethod = "changed";
    contact = obj;
  }
  void removed(const element& obj) override {
    mMethod = "removed";
    contact = obj;
  }

 public:
  std::string mMethod;
  DiscoveredContact contact;
};

class DiscoveredContactWatcherTest : public QObject {
  Q_OBJECT

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
  std::shared_ptr<DiscoveredContactStorage> mStorage;
  std::shared_ptr<FakeWatcher> mWatcher;
  Db* primary;
};

void DiscoveredContactWatcherTest::initTestCase() {
  dbstl::dbstl_startup();
  QDir().mkdir("DiscoveredContactWatcherTest_env");
  auto penv = make_db_env("DiscoveredContactWatcherTest_env", "test");
  primary = make_db("test_discovered_contacts.db", "primary", penv);

  mStorage = make_discovered_contact_storage(primary, penv);
}

void DiscoveredContactWatcherTest::cleanupTestCase() {
  dbstl::dbstl_exit();
  QDir curr;
  if (curr.cd("DiscoveredContactWatcherTest_env"))
    curr.removeRecursively();
}

void DiscoveredContactWatcherTest::testAddWatcher() {
  QFETCH(std::chrono::time_point<std::chrono::system_clock>::duration,
         duration);
  QFETCH(std::string, channel_moniker);
  QFETCH(std::string, contact_adress);
  QFETCH(std::string, public_key);
  QFETCH(std::string, name);
  QFETCH(std::string, dialog_id);

  mStorage->add(DiscoveredContact(
      std::chrono::time_point<std::chrono::system_clock>(duration),
      channel_moniker, contact_adress, public_key, name, dialog_id));

  QCOMPARE(mWatcher->mMethod, std::string("added"));
  QCOMPARE(mWatcher->contact.created().time_since_epoch(), duration);
  QCOMPARE(std::string(mWatcher->contact.channel_moniker()), channel_moniker);
  QCOMPARE(std::string(mWatcher->contact.contact_adress()), contact_adress);
  QCOMPARE(std::string(mWatcher->contact.public_key()), public_key);
  QCOMPARE(std::string(mWatcher->contact.name()), name);
  QCOMPARE(std::string(mWatcher->contact.dialog_id()), dialog_id);
}

void DiscoveredContactWatcherTest::testAddWatcher_data() {
  QTest::addColumn<
      std::chrono::time_point<std::chrono::system_clock>::duration>("duration");
  QTest::addColumn<std::string>("channel_moniker");
  QTest::addColumn<std::string>("contact_adress");
  QTest::addColumn<std::string>("public_key");
  QTest::addColumn<std::string>("name");
  QTest::addColumn<std::string>("dialog_id");

  QTest::newRow("0")
      << std::chrono::time_point<std::chrono::system_clock>::duration(1500)
      << std::string("fake channel moniker 1") << std::string("fake address 1")
      << std::string("fake public key 1") << std::string("fake name 1")
      << std::string("fake dialog_id 1");

  QTest::newRow("1")
      << std::chrono::time_point<std::chrono::system_clock>::duration(15012)
      << std::string("fake channel moniker 2") << std::string("fake address 2")
      << std::string("fake public key 2") << std::string("fake name 2")
      << std::string("fake dialog_id 2");

  QTest::newRow("2")
      << std::chrono::time_point<std::chrono::system_clock>::duration(1502)
      << std::string("fake channel moniker 3") << std::string("fake address 3")
      << std::string("fake public key 3") << std::string("fake name 3")
      << std::string("fake dialog_id 3");
}

void DiscoveredContactWatcherTest::testChangeWatcher() {
  QFETCH(std::chrono::time_point<std::chrono::system_clock>::duration,
         duration);
  QFETCH(std::string, channel_moniker);
  QFETCH(std::string, contact_adress);
  QFETCH(std::string, public_key);
  QFETCH(std::string, name);
  QFETCH(std::string, dialog_id);

  mStorage->update(DiscoveredContact(
      std::chrono::time_point<std::chrono::system_clock>(duration),
      channel_moniker, contact_adress, public_key, name, dialog_id));

  QCOMPARE(mWatcher->mMethod, std::string("changed"));
  QCOMPARE(mWatcher->contact.created().time_since_epoch(), duration);
  QCOMPARE(std::string(mWatcher->contact.channel_moniker()), channel_moniker);
  QCOMPARE(std::string(mWatcher->contact.contact_adress()), contact_adress);
  QCOMPARE(std::string(mWatcher->contact.public_key()), public_key);
  QCOMPARE(std::string(mWatcher->contact.name()), name);
  QCOMPARE(std::string(mWatcher->contact.dialog_id()), dialog_id);
}

void DiscoveredContactWatcherTest::testChangeWatcher_data() {
  QTest::addColumn<
      std::chrono::time_point<std::chrono::system_clock>::duration>("duration");
  QTest::addColumn<std::string>("channel_moniker");
  QTest::addColumn<std::string>("contact_adress");
  QTest::addColumn<std::string>("public_key");
  QTest::addColumn<std::string>("name");
  QTest::addColumn<std::string>("dialog_id");

  QTest::newRow("0")
      << std::chrono::time_point<std::chrono::system_clock>::duration(9855)
      << std::string("fake channel moniker 21")
      << std::string("fake address 24") << std::string("fake public key 11")
      << std::string("fake name 11") << std::string("dialog_id 1");

  QTest::newRow("1")
      << std::chrono::time_point<std::chrono::system_clock>::duration(152)
      << std::string("fake channel moniker 214")
      << std::string("fake addr515ess 2") << std::string("fake pu51blic key 2")
      << std::string("fake name 2") << std::string("dialog_id 2");

  QTest::newRow("2")
      << std::chrono::time_point<std::chrono::system_clock>::duration(502)
      << std::string("fake channel moniker 03")
      << std::string("fake address 31") << std::string("fake public key 3")
      << std::string("fake name 3") << std::string("dialog_id 3");
}

void DiscoveredContactWatcherTest::testRemoveWatcher() {
  QFETCH(std::string, dialog_id);
  mStorage->remove(dialog_id);

  QCOMPARE(mWatcher->mMethod, std::string("removed"));
  QCOMPARE(std::string(mWatcher->contact.dialog_id()), dialog_id);
}

void DiscoveredContactWatcherTest::testRemoveWatcher_data() {
  QTest::addColumn<std::string>("dialog_id");

  QTest::newRow("0") << std::string("dialog_id 1");

  QTest::newRow("1") << std::string("dialog_id 2");

  QTest::newRow("2") << std::string("dialog_id 3");
}

void DiscoveredContactWatcherTest::init() {
  primary->truncate(nullptr, nullptr, 0);

  mStorage->add(DiscoveredContact(
      std::chrono::time_point<std::chrono::system_clock>(
          std::chrono::time_point<std::chrono::system_clock>::duration(1021)),
      "channel_moniker 1", "contact_adress 1", "public_key 1", "name 1",
      "dialog_id 1"));
  mStorage->add(DiscoveredContact(
      std::chrono::time_point<std::chrono::system_clock>(
          std::chrono::time_point<std::chrono::system_clock>::duration(1021)),
      "channel_moniker 2", "contact_adress 2", "public_key 2", "name 2",
      "dialog_id 2"));
  mStorage->add(DiscoveredContact(
      std::chrono::time_point<std::chrono::system_clock>(
          std::chrono::time_point<std::chrono::system_clock>::duration(1021)),
      "channel_moniker 3", "contact_adress 3", "public_key 3", "name 3",
      "dialog_id 3"));

  mWatcher = std::make_shared<FakeWatcher>();

  mStorage->appendPermanentListener(mWatcher);
}

QTEST_APPLESS_MAIN(DiscoveredContactWatcherTest)

#include "discoveredcontactwatchertest.moc"
