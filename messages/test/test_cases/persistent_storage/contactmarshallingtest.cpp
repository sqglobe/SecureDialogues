#include <QtTest>
#include "persistent_storage/contactmarshaller.h"
#include "primitives/contact.h"

#include <iostream>

class ContactMarshallingTest : public QObject {
  Q_OBJECT

 public:
 private Q_SLOTS:
  void testMarshalling();
};

void ContactMarshallingTest::testMarshalling() {
  Contact standard = Contact(static_cast<const char*>("standart moniker"),
                             static_cast<const char*>("standart name"),
                             static_cast<const char*>("standart address"),
                             static_cast<const char*>("standart key"),
                             static_cast<const char*>("standart id"));
  std::string buff(ContactMarshaller::size(standard), '\0');
  ContactMarshaller::store(&buff[0], standard);

  Contact resored{"", "", "", ""};
  ContactMarshaller::restore(resored, &buff[0]);

  QCOMPARE(resored.name(), standard.name());
  QCOMPARE(resored.adress(), standard.adress());
  QCOMPARE(resored.publicKey(), standard.publicKey());
  QCOMPARE(resored.channelMoniker(), standard.channelMoniker());
  QCOMPARE(resored.id(), standard.id());
}

QTEST_APPLESS_MAIN(ContactMarshallingTest)

#include "contactmarshallingtest.moc"
