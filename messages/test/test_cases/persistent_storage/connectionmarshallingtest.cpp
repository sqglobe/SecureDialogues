#include <QtTest>
#include "persistent_storage/connectionmarshaller.h"
#include "primitives/connectionholder.h"

#include <iostream>

bool operator==(const ConnectionHolder& lh, const ConnectionHolder& rh) {
  if (lh.getType() != rh.getType() ||
      lh.getConnectionName() != rh.getConnectionName()) {
    return false;
  }

  if (lh.getType() == ConnectionType::VK) {
    auto const& lhConn = lh.getConnection<VkConnection>();
    auto const& rhConn = rh.getConnection<VkConnection>();

    return lhConn.userId == rhConn.userId &&
           lhConn.accessToken == lhConn.accessToken;
  } else if (lh.getType() == ConnectionType::EMAIL) {
    auto const& lhConn = lh.getConnection<EmailConnection>();
    auto const& rhConn = rh.getConnection<EmailConnection>();

    return lhConn.from == rhConn.from && lhConn.tlsUsed == rhConn.tlsUsed &&
           lhConn.imapAddr == rhConn.imapAddr &&
           lhConn.imapPort == rhConn.imapPort &&

           lhConn.password == rhConn.password &&
           lhConn.smtpAddr == rhConn.smtpAddr &&
           lhConn.smtpPort == rhConn.smtpPort &&
           lhConn.userName == rhConn.userName;
  } else if (lh.getType() == ConnectionType::GMAIL) {
    auto const& lhConn = lh.getConnection<GmailConnection>();
    auto const& rhConn = rh.getConnection<GmailConnection>();

    return lhConn.email == rhConn.email &&
           lhConn.accessToken == rhConn.accessToken;
  }
  return false;
}

class ConnectionMarshallingTest : public QObject {
  Q_OBJECT

 public:
 private Q_SLOTS:
  void testUdpMarshalling();
  void testVkMarshalling();
  void testGmailMarshalling();
  void testEmailMarshalling();
};

void ConnectionMarshallingTest::testUdpMarshalling() {
  UdpConnection udp;
  ConnectionHolder holder(udp, "test udp");

  std::string buff(ConnectionMarshaller::size(holder), '\0');
  ConnectionMarshaller::store(&buff[0], holder);

  ConnectionHolder resored("");
  ConnectionMarshaller::restore(resored, &buff[0]);

  QVERIFY(resored == holder);
}

void ConnectionMarshallingTest::testVkMarshalling() {
  VkConnection vkConn{"test user id", "test acc-tocken"};
  ConnectionHolder holder(vkConn, "test vk");

  std::string buff(ConnectionMarshaller::size(holder), '\0');
  ConnectionMarshaller::store(&buff[0], holder);

  ConnectionHolder resored("");
  ConnectionMarshaller::restore(resored, &buff[0]);

  QVERIFY(resored == holder);
}

void ConnectionMarshallingTest::testGmailMarshalling() {
  GmailConnection gmailConn{"test email", "test acc-tocken"};
  ConnectionHolder holder(gmailConn, "test gmail");

  std::string buff(ConnectionMarshaller::size(holder), '\0');
  ConnectionMarshaller::store(&buff[0], holder);

  ConnectionHolder resored("");
  ConnectionMarshaller::restore(resored, &buff[0]);

  QVERIFY(resored == holder);
}

void ConnectionMarshallingTest::testEmailMarshalling() {
  EmailConnection emailConn{
      "test user name", "test pass", false, "test from", "test smtp addr", 100,
      "test imap addr", 200};
  ConnectionHolder holder(emailConn, "test email");

  std::string buff(ConnectionMarshaller::size(holder), '\0');
  ConnectionMarshaller::store(&buff[0], holder);

  ConnectionHolder resored("");
  ConnectionMarshaller::restore(resored, &buff[0]);

  QVERIFY(resored == holder);
}

QTEST_APPLESS_MAIN(ConnectionMarshallingTest)

#include "connectionmarshallingtest.moc"
