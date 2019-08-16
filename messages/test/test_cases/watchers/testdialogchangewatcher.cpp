#include <QTest>
#include "primitives/contact.h"
#include "primitives/dialog.h"

#include "containers/storages.h"
#include "interfaces/changelistener.h"
#include "utils/dbfactory.h"

#include <dbstl_common.h>
#include <memory>

Q_DECLARE_METATYPE(std::string)
Q_DECLARE_METATYPE(Dialog::Status)

class FakeDialogWather : public ChangeListener<Dialog> {
 public:
  void added(const element& obj) override {
    mMethod = "added";
    mDialogId = obj.getDialogId();
    mContactId = obj.getContactId();
    mStatus = obj.getStatus();
  }
  void changed(const element& obj) override {
    mMethod = "changed";
    mDialogId = obj.getDialogId();
    mContactId = obj.getContactId();
    mStatus = obj.getStatus();
  }
  void removed(const element& obj) override {
    mMethod = "removed";
    mDialogId = obj.getDialogId();
    mContactId = obj.getContactId();
    mStatus = obj.getStatus();
  }

 public:
  std::string mMethod;
  std::string mDialogId;
  std::string mContactId;
  Dialog::Status mStatus;
};

class TestDialogChangeWatcher : public QObject {
  Q_OBJECT
 public:
  explicit TestDialogChangeWatcher(QObject* parent = nullptr);

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
  std::shared_ptr<DialogStorage> mDialogStorage;
  std::shared_ptr<FakeDialogWather> mWatcher;
  Db* primary;
  Db* secondary;
};

TestDialogChangeWatcher::TestDialogChangeWatcher(QObject* parent) :
    QObject(parent) {}

void TestDialogChangeWatcher::initTestCase() {
  dbstl::dbstl_startup();
  auto penv = make_db_env("TestDialogChangeWatcher_env", "test");
  primary = make_db("test_dialogs.db", "primary", penv);
  secondary = make_db("test_dialogs.db", "secondary", penv, DB_DUP);

  mDialogStorage = make_dialog_storage(primary, secondary, penv);
}

void TestDialogChangeWatcher::cleanupTestCase() {
  dbstl::dbstl_exit();
}

void TestDialogChangeWatcher::testAddWatcher() {
  QFETCH(std::string, contact_id);
  QFETCH(std::string, dialog_id);
  QFETCH(Dialog::Status, status);

  mDialogStorage->add(Dialog(contact_id, dialog_id, 0, 0, status));

  QCOMPARE(mWatcher->mContactId, contact_id);
  QCOMPARE(mWatcher->mDialogId, dialog_id);
  QCOMPARE(mWatcher->mMethod, std::string("added"));
  QCOMPARE(mWatcher->mStatus, status);
}

void TestDialogChangeWatcher::testAddWatcher_data() {
  QTest::addColumn<std::string>("contact_id");
  QTest::addColumn<std::string>("dialog_id");
  QTest::addColumn<Dialog::Status>("status");

  QTest::newRow("0") << std::string("fake name 1")
                     << std::string("fake added id 1") << Dialog::Status::NEW;

  QTest::newRow("1") << std::string("fake name 2")
                     << std::string("fake active dialog id")
                     << Dialog::Status::ACTIVE;

  QTest::newRow("2") << std::string("fake name 3")
                     << std::string("fake closed dialog")
                     << Dialog::Status::CLOSED;

  QTest::newRow("3") << std::string("fake name 4")
                     << std::string("fake wait confirm id")
                     << Dialog::Status::WAIT_CONFIRM;
}

void TestDialogChangeWatcher::testChangeWatcher() {
  QFETCH(std::string, contact_id);
  QFETCH(std::string, dialog_id);
  QFETCH(Dialog::Status, status);

  mDialogStorage->update(Dialog(contact_id, dialog_id, 0, 0, status));

  QCOMPARE(mWatcher->mDialogId, dialog_id);
  QCOMPARE(mWatcher->mContactId, contact_id);
  QCOMPARE(mWatcher->mMethod, std::string("changed"));
  QCOMPARE(mWatcher->mStatus, status);
}

void TestDialogChangeWatcher::testChangeWatcher_data() {
  QTest::addColumn<std::string>("contact_id");
  QTest::addColumn<std::string>("dialog_id");
  QTest::addColumn<Dialog::Status>("status");

  QTest::newRow("0") << std::string("new name 21")
                     << std::string("new dialog 1")
                     << Dialog::Status::WAIT_CONFIRM;
  QTest::newRow("1") << std::string("new name 21")
                     << std::string("new dialog 2") << Dialog::Status::ABORTED;

  QTest::newRow("2") << std::string("name active1")
                     << std::string("active dialog 1")
                     << Dialog::Status::ABORTED;

  QTest::newRow("2") << std::string("name active2")
                     << std::string("active dialog 2")
                     << Dialog::Status::CLOSED;
}

void TestDialogChangeWatcher::testRemoveWatcher() {
  QFETCH(std::string, contact_id);
  QFETCH(std::string, dialog_id);

  mDialogStorage->remove(dialog_id);

  QCOMPARE(mWatcher->mContactId, contact_id);
  QCOMPARE(mWatcher->mDialogId, dialog_id);
  QCOMPARE(mWatcher->mMethod, std::string("removed"));
  QCOMPARE(mWatcher->mStatus, Dialog::Status::WAIT_CONFIRM);
}

void TestDialogChangeWatcher::testRemoveWatcher_data() {
  QTest::addColumn<std::string>("contact_id");
  QTest::addColumn<std::string>("dialog_id");

  QTest::newRow("0") << std::string("channel 3") << std::string("name created1")
                     << std::string("adress created1")
                     << std::string("created dialog 1");
  QTest::newRow("1") << std::string("channel 3") << std::string("name created2")
                     << std::string("adress created2")
                     << std::string("created dialog 2");
  QTest::newRow("2") << std::string("channel 3") << std::string("name created3")
                     << std::string("adress created3")
                     << std::string("created dialog 3");
}

void TestDialogChangeWatcher::init() {
  primary->truncate(nullptr, nullptr, 0);
  secondary->truncate(nullptr, nullptr, 0);
  mDialogStorage->add(Dialog(std::string("test contact"),
                             std::string("new dialog 1"), 0, 0,
                             Dialog::Status::NEW));
  mDialogStorage->add(Dialog(std::string("test contact"),
                             std::string("new dialog 2"), 0, 0,
                             Dialog::Status::NEW));
  mDialogStorage->add(Dialog(std::string("test contact"),
                             std::string("new dialog 4"), 0, 0,
                             Dialog::Status::NEW));

  mDialogStorage->add(Dialog(std::string("name active1"),
                             std::string("active dialog 1"), 0, 0,
                             Dialog::Status::ACTIVE));
  mDialogStorage->add(Dialog(std::string("name active2"),
                             std::string("active dialog 2"), 0, 0,
                             Dialog::Status::ACTIVE));
  mDialogStorage->add(Dialog(std::string("name active2"),
                             std::string("active dialog 2"), 0, 0,
                             Dialog::Status::ACTIVE));

  mDialogStorage->add(Dialog(std::string("name created1"),
                             std::string("created dialog 1"), 0, 0,
                             Dialog::Status::WAIT_CONFIRM));
  mDialogStorage->add(Dialog(std::string("name created2"),
                             std::string("created dialog 2"), 0, 0,
                             Dialog::Status::WAIT_CONFIRM));
  mDialogStorage->add(Dialog(std::string("name created3"),
                             std::string("created dialog 3"), 0, 0,
                             Dialog::Status::WAIT_CONFIRM));

  mWatcher = std::make_shared<FakeDialogWather>();
  mDialogStorage->appendPermanentListener(mWatcher);
}
QTEST_APPLESS_MAIN(TestDialogChangeWatcher)
#include "testdialogchangewatcher.moc"
