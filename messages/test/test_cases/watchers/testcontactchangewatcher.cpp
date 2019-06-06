#include <QTest>
#include <memory>
#include "containers/contactcontainer.h"
#include "interfaces/changewatcher.h"
#include "primitives/contact.h"
#include "primitives/dialog.h"
Q_DECLARE_METATYPE(std::string)

class FakeContactWatcher
    : public ChangeWatcher<ContactContainer::const_element> {
 public:
  void added(const ContactContainer::const_element& obj) override {
    mMethod = "added";
    mId = obj->id();
    mChannel = obj->channelMoniker();
    mAdress = obj->adress();
    mName = obj->name();
  }
  void changed(const ContactContainer::const_element& obj) override {
    mMethod = "changed";
    mId = obj->id();
    mChannel = obj->channelMoniker();
    mAdress = obj->adress();
    mName = obj->name();
  }
  void removed(const ContactContainer::const_element& obj) override {
    mMethod = "removed";
    mId = obj->id();
    mChannel = obj->channelMoniker();
    mAdress = obj->adress();
    mName = obj->name();
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
  void testAddWatcher();
  void testAddWatcher_data();

  void testChangeWatcher();
  void testChangeWatcher_data();

  void testRemoveWatcher();
  void testRemoveWatcher_data();

  void init();

 private:
  std::shared_ptr<ContactContainer> mContactContainer;
  std::shared_ptr<FakeContactWatcher> mWatcher;
};

TestContactChangeWatcher::TestContactChangeWatcher(QObject* parent) :
    QObject(parent) {}

void TestContactChangeWatcher::testAddWatcher() {
  QFETCH(std::string, channel);
  QFETCH(std::string, name);
  QFETCH(std::string, adress);
  QFETCH(std::string, contact_id);

  mContactContainer->add(
      std::make_shared<Contact>(channel, name, adress, "no pass", contact_id));

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

  mContactContainer->update(
      std::make_shared<Contact>(channel, name, adress, "no pass", contact_id));

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

  mContactContainer->remove(contact_id);

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
  mContactContainer = std::make_shared<ContactContainer>();

  mContactContainer->add(std::make_shared<Contact>(
      "channel 1", "name 1", "adress 1", "no pass", "id 1"));
  mContactContainer->add(std::make_shared<Contact>(
      "channel 2", "name 2", "adress 2", "no pass", "id 2"));
  mContactContainer->add(std::make_shared<Contact>(
      "channel 3", "name 3", "adress 3", "no pass", "id 3"));
  mContactContainer->add(std::make_shared<Contact>(
      "channel 4", "name 4", "adress 4", "no pass", "id 4"));

  mWatcher = std::make_shared<FakeContactWatcher>();

  mContactContainer->registerWatcher(mWatcher);
}

QTEST_APPLESS_MAIN(TestContactChangeWatcher)

#include "testcontactchangewatcher.moc"
