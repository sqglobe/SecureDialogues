#include <QtTest>
// add necessary includes here

#include "asymetrical/asymetricalkeygenerator.h"
#include "asymetrical/asymetricalkeystore.h"
#include "asymetrical/localpeeroperations.h"
#include "asymetrical/remotepeeroperations.h"
#include "symetrical/aeskeygenerator.h"
#include "symetrical/aeskeystruct.h"
#include "symetrical/symetricalsystemfactories.h"

#include "interfaces/symetricalcipher.h"

#include <iostream>

class TotalAesSystemTest : public QObject {
  Q_OBJECT

 private slots:
  void test_encryption_decryption();
};

void TotalAesSystemTest::test_encryption_decryption() {
  std::string message = "Hellow world";

  auto rsaKeyStore = std::make_shared<AsymetricalKeyStore>();
  QVERIFY(AsymetricalKeyGenerator(rsaKeyStore).generate());

  try {
    auto local =
        std::make_shared<LocalPeerOperations>(rsaKeyStore->getPrivateKey());
    auto remote =
        std::make_shared<RemotePeerOperations>(rsaKeyStore->getPublicKey());

    std::string keyData;
    auto oneCipher = generateSymmetricalCipherTo(keyData, remote);

    auto secondCipher = createSymmetricalCipherFrom(keyData, local);

    auto encrypted = oneCipher->encrypt(message);

    auto decrypted = secondCipher->decrypt(encrypted);
    QCOMPARE(message, decrypted);

  } catch (std::exception& ex) {
    throw ex;
  }
}

QTEST_APPLESS_MAIN(TotalAesSystemTest)

#include "tst_totalaessystemtest.moc"
