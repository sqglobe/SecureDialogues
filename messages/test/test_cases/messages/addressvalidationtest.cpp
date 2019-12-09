#include <QtTest>
#include "primitives/connectionholder.h"

Q_DECLARE_METATYPE(std::string);
Q_DECLARE_METATYPE(ConnectionType);

class AddressValidationTest : public QObject {
  Q_OBJECT

 public:
 private Q_SLOTS:
  void testAddressValidation_data();
  void testAddressValidation();
};

void AddressValidationTest::testAddressValidation_data() {
  QTest::addColumn<ConnectionType>("type");
  QTest::addColumn<QString>("address");
  QTest::addColumn<bool>("isValid");

  QTest::newRow("VK ok 1") << ConnectionType::VK << "id1234122" << true;
  QTest::newRow("VK ok 2") << ConnectionType::VK << "1234122" << true;
  QTest::newRow("VK ok 3") << ConnectionType::VK << "https://vk.com/id1234122"
                           << true;
  QTest::newRow("VK bad 1") << ConnectionType::VK << "test2341221a" << false;

  QTest::newRow("UDP ok") << ConnectionType::UDP << "test2341221a" << false;
  QTest::newRow("UDP bad") << ConnectionType::UDP << "192.168.1.2" << true;

  QTest::newRow("EMAIL bad")
      << ConnectionType::EMAIL << "test2341221a" << false;
  QTest::newRow("EMAIL ok 1")
      << ConnectionType::EMAIL << "test2341221a@mail.ru" << true;
  QTest::newRow("EMAIL ok 2")
      << ConnectionType::EMAIL << "tes-t1.Aa@mail.ru" << true;

  QTest::newRow("GMAIL bad")
      << ConnectionType::GMAIL << "test2341221a" << false;
  QTest::newRow("GMAIL ok 1")
      << ConnectionType::GMAIL << "test2341221a@mail.ru" << true;
  QTest::newRow("GMAIL ok 2")
      << ConnectionType::GMAIL << "tes-t1.Aa@mail.ru" << true;
}

void AddressValidationTest::testAddressValidation() {
  QFETCH(ConnectionType, type);
  QFETCH(QString, address);
  QFETCH(bool, isValid);

  auto res = is_address_valid(address.toUtf8().constData(), type);

  QCOMPARE(res, isValid);
}

QTEST_APPLESS_MAIN(AddressValidationTest)

#include "addressvalidationtest.moc"
