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
  std::shared_ptr<Contact> standard = std::make_shared<Contact>(
      "standart moniker", "standart name", "standart address", "standart key",
      "standart id");
  std::string buff(ContactMarshaller::size(standard), '\0');
  ContactMarshaller::store(&buff[0], standard);

  std::cout << "marshalled: " << buff << std::endl;

  std::shared_ptr<Contact> resored;
  ContactMarshaller::restore(resored, &buff[0]);

  std::cout << "name: " << resored->name() << ", adress: " << resored->adress()
            << ", publicKey: " << resored->publicKey()
            << ", moniker: " << resored->channelMoniker()
            << ", id: " << resored->id() << std::endl;

  QCOMPARE(resored->name(), standard->name());
  QCOMPARE(resored->adress(), standard->adress());
  QCOMPARE(resored->publicKey(), standard->publicKey());
  QCOMPARE(resored->channelMoniker(), standard->channelMoniker());
  QCOMPARE(resored->id(), standard->id());
}

QTEST_APPLESS_MAIN(ContactMarshallingTest)

#include "contactmarshallingtest.moc"
