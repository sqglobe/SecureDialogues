#include <QTest>
#include "containers/dialogmanager.h"
#include "interfaces/changewatcher.h"
#include "primitives/contact.h"
#include "primitives/dialog.h"

Q_DECLARE_METATYPE(std::string)

#include <memory>

std::shared_ptr<Dialog> mk_dialog(const std::string& channel,
                                  const std::string& name,
                                  const std::string& adress,
                                  const std::string& dialog_id,
                                  Dialog::Status st) {
  return std::make_shared<Dialog>(
      std::make_shared<Contact>(channel, name, adress, ""), dialog_id, st);
}

class FakeDialogWather : public ChangeWatcher<DialogManager::const_element> {
 public:
  virtual void added(const DialogManager::const_element& obj) override {
    mMethod = "added";
    mDialogId = obj->getDialogId();
    mChannel = obj->getChannelMoniker();
    mStatus = obj->getStatus();
  }
  virtual void changed(const DialogManager::const_element& obj) override {
    mMethod = "changed";
    mDialogId = obj->getDialogId();
    mChannel = obj->getChannelMoniker();
    mStatus = obj->getStatus();
  }
  virtual void removed(const DialogManager::const_element& obj) override {
    mMethod = "removed";
    mDialogId = obj->getDialogId();
    mChannel = obj->getChannelMoniker();
    mStatus = obj->getStatus();
  }

 public:
  std::string mMethod;
  std::string mDialogId;
  std::string mChannel;
  Dialog::Status mStatus;
};

class TestDialogChangeWatcher : public QObject {
  Q_OBJECT
 public:
  explicit TestDialogChangeWatcher(QObject* parent = nullptr);

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
  std::shared_ptr<DialogManager> mDialogManager;
  std::shared_ptr<FakeDialogWather> mWatcher;
};

TestDialogChangeWatcher::TestDialogChangeWatcher(QObject* parent) :
    QObject(parent) {}

void TestDialogChangeWatcher::testAddWatcher() {
  QFETCH(std::string, channel);
  QFETCH(std::string, name);
  QFETCH(std::string, adress);
  QFETCH(std::string, dialog_id);

  mDialogManager->add(mk_dialog(channel, name, adress, dialog_id,
                                Dialog::Status::WAIT_CONFIRM));

  QCOMPARE(mWatcher->mChannel, channel);
  QCOMPARE(mWatcher->mDialogId, dialog_id);
  QCOMPARE(mWatcher->mMethod, std::string("added"));
  QCOMPARE(mWatcher->mStatus, Dialog::Status::WAIT_CONFIRM);
}

void TestDialogChangeWatcher::testAddWatcher_data() {
  QTest::addColumn<std::string>("channel");
  QTest::addColumn<std::string>("name");
  QTest::addColumn<std::string>("adress");
  QTest::addColumn<std::string>("dialog_id");

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

void TestDialogChangeWatcher::testChangeWatcher() {
  QFETCH(std::string, channel);
  QFETCH(std::string, name);
  QFETCH(std::string, adress);
  QFETCH(std::string, dialog_id);

  mDialogManager->update(
      mk_dialog(channel, name, adress, dialog_id, Dialog::Status::ACTIVE));

  QCOMPARE(mWatcher->mChannel, channel);
  QCOMPARE(mWatcher->mDialogId, dialog_id);
  QCOMPARE(mWatcher->mMethod, std::string("changed"));
  QCOMPARE(mWatcher->mStatus, Dialog::Status::ACTIVE);
}

void TestDialogChangeWatcher::testChangeWatcher_data() {
  QTest::addColumn<std::string>("channel");
  QTest::addColumn<std::string>("name");
  QTest::addColumn<std::string>("adress");
  QTest::addColumn<std::string>("dialog_id");

  QTest::newRow("0") << std::string("channel 1") << std::string("name new1")
                     << std::string("adress new1")
                     << std::string("new dialog 1");
  QTest::newRow("1") << std::string("channel 1") << std::string("name new2")
                     << std::string("adress new2")
                     << std::string("new dialog 2");
  QTest::newRow("2") << std::string("channel 1") << std::string("name new3")
                     << std::string("adress new3")
                     << std::string("new dialog 3");
  QTest::newRow("3") << std::string("channel 1") << std::string("name new4")
                     << std::string("adress new4")
                     << std::string("new dialog 4");
}

void TestDialogChangeWatcher::testRemoveWatcher() {
  QFETCH(std::string, channel);
  QFETCH(std::string, name);
  QFETCH(std::string, adress);
  QFETCH(std::string, dialog_id);

  mDialogManager->remove(dialog_id);

  QCOMPARE(mWatcher->mChannel, channel);
  QCOMPARE(mWatcher->mDialogId, dialog_id);
  QCOMPARE(mWatcher->mMethod, std::string("removed"));
  QCOMPARE(mWatcher->mStatus, Dialog::Status::WAIT_CONFIRM);
}

void TestDialogChangeWatcher::testRemoveWatcher_data() {
  QTest::addColumn<std::string>("channel");
  QTest::addColumn<std::string>("name");
  QTest::addColumn<std::string>("adress");
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
  mDialogManager = std::make_shared<DialogManager>();
  mDialogManager->add(mk_dialog("channel 1", "name new1", "adress new1",
                                "new dialog 1", Dialog::Status::NEW));
  mDialogManager->add(mk_dialog("channel 1", "name new2", "adress new2",
                                "new dialog 2", Dialog::Status::NEW));
  mDialogManager->add(mk_dialog("channel 1", "name new3", "adress new3",
                                "new dialog 3", Dialog::Status::NEW));
  mDialogManager->add(mk_dialog("channel 1", "name new4", "adress new4",
                                "new dialog 4", Dialog::Status::NEW));

  mDialogManager->add(mk_dialog("channel 2", "name active1", "adress active1",
                                "active dialog 1", Dialog::Status::ACTIVE));
  mDialogManager->add(mk_dialog("channel 2", "name active2", "adress active2",
                                "active dialog 2", Dialog::Status::ACTIVE));
  mDialogManager->add(mk_dialog("channel 2", "name active3", "adress active3",
                                "active dialog 3", Dialog::Status::ACTIVE));

  mDialogManager->add(mk_dialog("channel 3", "name created1", "adress created1",
                                "created dialog 1",
                                Dialog::Status::WAIT_CONFIRM));
  mDialogManager->add(mk_dialog("channel 3", "name created2", "adress created2",
                                "created dialog 2",
                                Dialog::Status::WAIT_CONFIRM));
  mDialogManager->add(mk_dialog("channel 3", "name created3", "adress created3",
                                "created dialog 3",
                                Dialog::Status::WAIT_CONFIRM));

  mWatcher = std::make_shared<FakeDialogWather>();
  mDialogManager->registerWatcher(mWatcher);
}
QTEST_APPLESS_MAIN(TestDialogChangeWatcher)
#include "testdialogchangewatcher.moc"
