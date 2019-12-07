#include <QTest>

#include "communication/discovercontactagent.h"
#include "primitives/dialogmessage.h"

#include "cryptosystemfake.h"
#include "fakemessagedespather.h"

class DiscoverContactAgentTest : public QObject {
  Q_OBJECT

 private Q_SLOTS:
  void testGetNameAndPublickKey();
  void testDiscoverAction();
};

void DiscoverContactAgentTest::testGetNameAndPublickKey() {
  DialogMessage message{
      DialogMessage::Action::CONTACT_DISCOVER,
      std::string("test name\n------------------------------\nvery big public "
                  "key 1\n2"),
      std::string("uuid 1"), std::string("test adress 1"), 0};
  auto res = DiscoverContactAgent::getNameAndPublickKey(message);
  QCOMPARE(res.first, std::string("test name"));
  QCOMPARE(res.second, std::string("very big public key 1\n2"));
}

void DiscoverContactAgentTest::testDiscoverAction() {
  std::shared_ptr<CryptoSystem> crypto{new CryptoSystemFake};
  std::shared_ptr<FakeMessageSendDespatcher> despatcher{
      new FakeMessageSendDespatcher};

  DiscoverContactAgent agent{crypto, despatcher};
  agent.discover("test name", "test address", "test channel");

  QCOMPARE(despatcher->mChannel, std::string("test channel"));
  QCOMPARE(despatcher->mSendFunction, std::string("sendAndForget"));
  QCOMPARE(despatcher->mMessage.action(),
           DialogMessage::Action::CONTACT_DISCOVER);
  QCOMPARE(std::string(despatcher->mMessage.adress()),
           std::string("test address"));
  QCOMPARE(
      std::string(despatcher->mMessage.content()),
      std::string("test name\n------------------------------\npublic key 1"));
}

QTEST_APPLESS_MAIN(DiscoverContactAgentTest)

#include "discovercontactagenttest.moc"
