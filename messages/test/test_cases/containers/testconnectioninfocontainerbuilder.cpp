#include <QTest>

#include "builders/builders.h"
#include "communication/channel.h"
#include "communication/messagedespatcher.h"
#include "containers/connectioninfocontainer.h"
#include "containers/contactcontainer.h"
#include "fakechanneladapter.h"
#include "fakefile.h"
#include "primitives/contact.h"

class TestConnectionInfocontainerBuilder : public QObject {
  Q_OBJECT
 public:
  explicit TestConnectionInfocontainerBuilder(QObject* parent = nullptr);

 signals:

 private slots:
  void testSerialize();
  void testUnserialize();
};

TestConnectionInfocontainerBuilder::TestConnectionInfocontainerBuilder(
    QObject* parent) :
    QObject(parent) {}

void TestConnectionInfocontainerBuilder::testSerialize() {
  auto connCont = ConnectionInfoContainer();
  connCont.add(
      ConnectionHolder(GmailConnection{"fake@gmail.com", "1234"}, "fake"));

  connCont.add(
      ConnectionHolder(GmailConnection{"fake2@gmail.com", "123234"}, "fake2"));
  connCont.add(ConnectionHolder(UdpConnection{}, "fake_udp"));

  connCont.add(ConnectionHolder(VkConnection{"134", "test123"}, "fake_vk"));

  connCont.add(ConnectionHolder(
      EmailConnection{"fake_user", "fake_pass", true, "fake@from.com",
                      "smtp.fake.com", 100, "imap.fake.com", 121},
      "fake_email"));

  FakeFile fakeFile("");
  auto builder = make_builder(fakeFile, connCont);
  builder.serialize();

  QCOMPARE(fakeFile.mwritelines[0], std::string("fake;2;fake@gmail.com;1234"));
  QCOMPARE(fakeFile.mwritelines[1],
           std::string("fake2;2;fake2@gmail.com;123234"));
  QCOMPARE(fakeFile.mwritelines[2], std::string("fake_udp;1"));
  QCOMPARE(fakeFile.mwritelines[3], std::string("fake_vk;3;134;test123"));
  QCOMPARE(fakeFile.mwritelines[4],
           std::string("fake_email;4;fake_user;fake_pass;1;fake@from.com;smtp."
                       "fake.com;100;imap.fake.com;121"));
}

void TestConnectionInfocontainerBuilder::testUnserialize() {
  auto connCont = ConnectionInfoContainer();
  FakeFile fakeFile(std::string("fake;2;fake@gmail.com;1234"));
  auto builder = make_builder(fakeFile, connCont);
  builder.unserialize();

  auto connHolder = connCont.get("fake");
  auto gmailConn = connHolder.getConnection<GmailConnection>();

  QCOMPARE(connCont.size(), 1);
  QVERIFY(connCont.has("fake"));
  QCOMPARE(connHolder.getConnectionName(), std::string("fake"));
  QCOMPARE(connHolder.getType(), ConnectionType::GMAIL);
  QCOMPARE(gmailConn.email, std::string("fake@gmail.com"));
  QCOMPARE(gmailConn.accessToken, std::string("1234"));
}

QTEST_APPLESS_MAIN(TestConnectionInfocontainerBuilder)

#include "testconnectioninfocontainerbuilder.moc"
