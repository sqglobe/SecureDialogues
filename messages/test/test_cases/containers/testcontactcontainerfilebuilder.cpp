#include <QTest>

#include "builders/builders.h"
#include "containers/contactcontainer.h"
#include "fakefile.h"
#include "primitives/contact.h"

class TestContactContainerFileBuilder : public QObject {
  Q_OBJECT
 public:
  explicit TestContactContainerFileBuilder(QObject* parent = nullptr);

 signals:

 private slots:
  void testSerialize();
  void testUnserialize();
};

TestContactContainerFileBuilder::TestContactContainerFileBuilder(
    QObject* parent) :
    QObject(parent) {}

void TestContactContainerFileBuilder::testSerialize() {
  ContactContainer cc;
  cc.add(std::make_shared<Contact>("fake channel", "fake name 1",
                                   "fake address1", "no pass", "id_1"));
  cc.add(std::make_shared<Contact>("fake channel", "fake name 2",
                                   "fake address2", "no pass", "id_2"));
  cc.add(std::make_shared<Contact>("fake channel", "fake name 3",
                                   "fake address3", "no pass", "id_3"));
  FakeFile fakeFile("");
  auto builder = make_builder(fakeFile, cc);
  builder.serialize();

  QCOMPARE(fakeFile.mwritelines[0],
           std::string("fake address1;fake channel;fake name 1;id_1;no pass"));
  QCOMPARE(fakeFile.mwritelines[1],
           std::string("fake address2;fake channel;fake name 2;id_2;no pass"));
  QCOMPARE(fakeFile.mwritelines[2],
           std::string("fake address3;fake channel;fake name 3;id_3;no pass"));
}

void TestContactContainerFileBuilder::testUnserialize() {
  ContactContainer cc;
  FakeFile fakeFile(
      std::string("fake address1;fake channel;fake name 1;id_1;no pass"));
  auto builder = make_builder(fakeFile, cc);
  builder.unserialize();

  auto contact = cc.getAllElements()[0];
  QCOMPARE(contact->adress(), std::string("fake address1"));
  QCOMPARE(contact->channelMoniker(), std::string("fake channel"));
  QCOMPARE(contact->name(), std::string("fake name 1"));
  QCOMPARE(contact->id(), std::string("id_1"));
  QCOMPARE(contact->publicKey(), std::string("no pass"));
}

QTEST_APPLESS_MAIN(TestContactContainerFileBuilder)

#include "testcontactcontainerfilebuilder.moc"
