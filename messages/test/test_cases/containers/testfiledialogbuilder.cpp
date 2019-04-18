
#include <QTest>

#include "builders/builders.h"
#include "containers/contactcontainer.h"
#include "containers/dialogmanager.h"
#include "fakefile.h"
#include "primitives/contact.h"
#include "primitives/dialog.h"

class TestFileDialogBuilder : public QObject {
  Q_OBJECT
 public:
  explicit TestFileDialogBuilder(QObject* parent = nullptr);

 signals:

 private slots:
  void testSerialize();
  void testUnserialize();
};

TestFileDialogBuilder::TestFileDialogBuilder(QObject* parent) :
    QObject(parent) {}

void TestFileDialogBuilder::testSerialize() {
  std::shared_ptr<ContactContainer> cc = std::make_shared<ContactContainer>();
  cc->add(std::make_shared<Contact>("fake channel", "fake name 1",
                                    "fake address1", "id_1"));
  cc->add(std::make_shared<Contact>("fake channel", "fake name 2",
                                    "fake address2", "id_2"));
  cc->add(std::make_shared<Contact>("fake channel", "fake name 3",
                                    "fake address3", "id_3"));

  DialogManager dm;

  dm.add(std::make_shared<Dialog>(cc->get("id_1"), "fake dialog 1"));
  dm.add(std::make_shared<Dialog>(cc->get("id_2"), "fake dialog 2",
                                  Dialog::Status::ACTIVE));
  dm.add(std::make_shared<Dialog>(cc->get("id_3"), "fake dialog 3",
                                  Dialog::Status::CLOSED));

  FakeFile fakeFile("");
  auto builder = make_builder(fakeFile, dm);
  builder.serialize();

  QCOMPARE(fakeFile.mwritelines[0], std::string("fake dialog 1;id_1;0"));
  QCOMPARE(fakeFile.mwritelines[1], std::string("fake dialog 2;id_2;5"));
  QCOMPARE(fakeFile.mwritelines[2], std::string("fake dialog 3;id_3;6"));
}

void TestFileDialogBuilder::testUnserialize() {
  std::shared_ptr<ContactContainer> cc = std::make_shared<ContactContainer>();
  cc->add(std::make_shared<Contact>("fake channel", "fake name 1",
                                    "fake address1", "id_1"));
  cc->add(std::make_shared<Contact>("fake channel", "fake name 2",
                                    "fake address2", "id_2"));
  cc->add(std::make_shared<Contact>("fake channel", "fake name 3",
                                    "fake address3", "id_3"));

  DialogManager dm;
  FakeFile fakeFile(std::string("fake active dialog;id_2;2"));
  auto builder = make_builder(fakeFile, dm);
  builder.setContactContainer(cc);
  builder.unserialize();

  auto dialog = dm.getAllElements()[0];
  QCOMPARE(dialog->getContactId(), std::string("id_2"));
  QCOMPARE(dialog->getDialogId(), std::string("fake active dialog"));
  QCOMPARE(dialog->getStatus(), Dialog::Status::WAIT_CONFIRM);
}

QTEST_APPLESS_MAIN(TestFileDialogBuilder)

#include "testfiledialogbuilder.moc"
