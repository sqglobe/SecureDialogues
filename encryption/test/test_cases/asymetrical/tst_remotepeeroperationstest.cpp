#include <QtTest>
// add necessary includes here

#include <string>
#include "asymetrical/remotepeeroperations.h"

#include <cryptopp/base64.h>
#include <cryptopp/osrng.h>
#include <cryptopp/rsa.h>

#include <iostream>

const static std::string PUBLIC =
    "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAvbvJBkKt90hXq52oAY7H"
    "HjqalFSAAMipUXSrw3MMS/UaFkn0UJYpcMPTDCHO66uR2Csoi56XJuNBi07d7AU6"
    "UlGnChlkN8JIBgXUbhjG8FztpIN3No7IHgr/O/6sDAo6TCcOm4s//VUoiMbHpI9G"
    "5QOrzWd5Ckkc6VjBSM7OsVn0HDTYPUCCD5RcI7A02HEf4Ue5F0E1lBySS8OeYj/m"
    "rj8ejFC3jw7hzdJ1AnQz+XyBEow6QwqoimDxklHF1yyDmcSYhvYTMg0XO3gEw0gk"
    "0wqzDWE5GNDKSWruLkppQN42pXycN4MPDFkJdT66nngTRqI28aYUbZBY6mtPk9jL"
    "xQIDAQAB";

CryptoPP::RSA::PublicKey load(const std::string& key) {
  CryptoPP::ByteQueue bt;
  CryptoPP::StringSource ss(key, true /*pumpAll*/,
                            new CryptoPP::Base64Decoder());
  ss.TransferTo(bt);
  bt.MessageEnd();
  CryptoPP::RSA::PublicKey pKey;
  pKey.Load(bt);
  CryptoPP::AutoSeededRandomPool rng;
  if (!pKey.Validate(rng, 3)) {
    throw std::runtime_error("RSA key not valid!");
  }
  return pKey;
}

class RemotePeerOperationsTest : public QObject {
  Q_OBJECT

 public:
 private slots:
  void test_signatureVerification();
  void test_encryption();
};

void RemotePeerOperationsTest::test_signatureVerification() {
  std::string signature =
      "miMJ4GUbkiEmj6VV9KMQxpfWAU6jzJZe+HprMF93VNL1F3BMmi8Y4Ecf97w7CyzKB4ed1hyg"
      "3RvpCFpP3Ifnr81OZgSw5zTb78SJYLIgjnERmBi/Z68GyO5M1MrFgm2Z0fysDCKZdVq7+sv7"
      "vLJ3O99S1Cx6+PgBycHm7p1Oj0WoxqY9JI+41s2/QCqCgsbrT1soR+Tmf6qwl+wka0L7yegT"
      "+6cn4KQIayHV94rD/OlAv7UDfMS5+oEbA0ritOFn3/qxKulfuo8mIx7Mp/E1/Wa6R4+KNAFc"
      "OoZvLzahPbPb23tvwBVs49jOEslJEBswruZrQ8CuQ5e2+7lHNzXX1w==";
  auto key = load(PUBLIC);
  RemotePeerOperations opers(key);

  QVERIFY(opers.isValid("Hello, World!", signature));
}

void RemotePeerOperationsTest::test_encryption() {
  std::string message = "Hellow world!";
  std::string encrypted =
      "kP4gxh7fWlppl1cnXCtFqOW1uC6dIkvExsPyAFelai/gUzXJt2CjLiQPZpT8VG+jO6jvLOKf"
      "Lp3O/a983aSKUKnnD84ZdPFEn0tmppVoSo16Q3056hTUKJTeLLgZ9Hd3QE33sIQD/XXgYN9a"
      "hRNVl3SMvVpbjaHjag3zzP3NwGm1cUG01dSoIj/mf31tR81uEt9g55OaLCTTEXNuTceZXAFO"
      "jbNzuQ9uYkCqB/5DxElRDOA0CRdVDmQKRPs109rZLjlI8KQXn1B9lygkCdjn5ZTZcZ0+Saji"
      "DqcWB+7GHf5DGSeGjdPxlUmdvg0o15JVqhcZeIOfsAo3cvBA/2vU6w==";

  auto key = load(PUBLIC);

  RemotePeerOperations opers(key);

  auto encr = opers.encrypt(message);

  QVERIFY(!encr.empty());
}

QTEST_APPLESS_MAIN(RemotePeerOperationsTest)

#include "tst_remotepeeroperationstest.moc"
