#include <QtTest>
#include "persistent_storage/discoveredcontactmarshaller.h"
#include "primitives/discoveredcontact.h"

#include <iostream>

class DiscoveredContactMarshallerTest : public QObject {
  Q_OBJECT

 public:
 private Q_SLOTS:
  void testMarshalling();
};

void DiscoveredContactMarshallerTest::testMarshalling() {
  DiscoveredContact standart("test_channel", "address 1", "no public key",
                             "test name", "testdialog id");
  std::string buff(DiscoveredContactMarshaller::size(standart), '\0');
  DiscoveredContactMarshaller::store(&buff[0], standart);

  DiscoveredContact restored;

  DiscoveredContactMarshaller::restore(restored, &buff[0]);

  QCOMPARE(standart.name(), restored.name());
  QCOMPARE(standart.created(), restored.created());
  QCOMPARE(standart.dialog_id(), restored.dialog_id());
  QCOMPARE(standart.public_key(), restored.public_key());
  QCOMPARE(standart.contact_adress(), restored.contact_adress());
  QCOMPARE(standart.channel_moniker(), restored.channel_moniker());
}

QTEST_APPLESS_MAIN(DiscoveredContactMarshallerTest)

#include "discoveredcontactmarshallertest.moc"
