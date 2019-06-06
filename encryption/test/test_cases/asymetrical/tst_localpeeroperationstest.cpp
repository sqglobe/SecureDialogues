#include <QtTest>
// add necessary includes here

#include <string>
#include "asymetrical/localpeeroperations.h"

#include <cryptopp/base64.h>
#include <cryptopp/osrng.h>
#include <cryptopp/rsa.h>

#include <iostream>

const static std::string PRIVATE =
    "MIIEpAIBAAKCAQEAvbvJBkKt90hXq52oAY7HHjqalFSAAMipUXSrw3MMS/UaFkn0"
    "UJYpcMPTDCHO66uR2Csoi56XJuNBi07d7AU6UlGnChlkN8JIBgXUbhjG8FztpIN3"
    "No7IHgr/O/6sDAo6TCcOm4s//VUoiMbHpI9G5QOrzWd5Ckkc6VjBSM7OsVn0HDTY"
    "PUCCD5RcI7A02HEf4Ue5F0E1lBySS8OeYj/mrj8ejFC3jw7hzdJ1AnQz+XyBEow6"
    "QwqoimDxklHF1yyDmcSYhvYTMg0XO3gEw0gk0wqzDWE5GNDKSWruLkppQN42pXyc"
    "N4MPDFkJdT66nngTRqI28aYUbZBY6mtPk9jLxQIDAQABAoIBACleFSFhTm7Y/gxz"
    "7lqQl8d2fZeTqOkC1y/kYULXBbpV8V8ktrkPVG+6snw1rtjhaoYYOtneNRSMfhDy"
    "nB5GsYWbOfFQwfIKUQvHH571lgJ4MHQrGcXycZ1jRmz9ROPHIxjRdzTgso34kELq"
    "HEM73fg2ybGaOuDMOhqCeHn4ZUZzdu0wtzzNjabbZKFcCuxrRuU9l5fOL31FZzW6"
    "/A2AdZOdlmAdDu22Piyi7P6kuAuaTE6br8tIrI7cgyKS/B7TWzJNocjK9butxQab"
    "4TieE76vr0LoOQolb6gUJeqatHx4vc+QdmdHEYKG5Xct8FP5kEFEUtrAN3WtTyHl"
    "wAKkp20CgYEA8X5SDwz6yHJHGD3chK/cKdqFqjJ1wlYN0qFF09lJvy2e3Vo2udJB"
    "5qAJUCcsl6Edi5M5u+4EHsYKld1s94kbAlKeT0fT6Oz6g8XyqRWpcjpSIeVpd+Do"
    "YWpxQLogkcfc2UXYOL8zdLzRRJDC8vEusMgZIf76tHl4X05TQ8UcZLcCgYEAySGA"
    "mWXKdKYdmapetdRfp9mjGj7KR8Uy8rS3xVSn6gy1ErWYp9pXRp/xo/WjnUVkUUOg"
    "a5xN744tJtps5n6qZhHe7PRNrGGjrphHiJEFVMsTJpBfc+w0aY98bX+eVEDsr0wl"
    "D5xCeWgMxtgPgVc4gP16HrFdYhr1FAzE3UXh72MCgYEAqMuSr9JyuAioiNk0ecoy"
    "KU8EfqImjV946fCvzhPi6C4LDM0x01CTn4/c8plvIc0LybNud102pAvo+jd7IspC"
    "hq0Y2nFqPTTnJZQGuQjbF2yMXATuY0xhITY1UvjGKx93zqZzK7b5UG9qFnLY0Naj"
    "vA0hBBIlSmBdMsCckpID9qECgYBpzXjjuNs+b6AElM3LotNK7b7R8JPiG0dIy24c"
    "nQLTqSmblPGf4sike0DW+kG+WqGB6iw6T3fj+4ue86nzulAS73WwenddpeM/1hVT"
    "Ls56I3WIhMvhpDCSEwvmYgq80YZwVILuaknBqc2G0GOfq20q1axm9vBjykB/8pOg"
    "vsEydwKBgQCZiJq/X24NFx6YM525udFHtXKopdPlhu+O1Mz8jDGg5sZ1/xJ0VyeD"
    "6hJkmPN0tynz8lbRlYAOiA70lyPNUrdaX3fY+NexEfMd4TAmyAxd3JWmkmHErUcE"
    "1yjrw4EcjMbDEoeOW3Eiv4Ap91wogljVhNnpTASeUkR95d6G3hoqvw==";

CryptoPP::RSA::PrivateKey load(const std::string& key) {
  CryptoPP::ByteQueue bt;
  CryptoPP::StringSource ss(key, true /*pumpAll*/,
                            new CryptoPP::Base64Decoder());
  ss.TransferTo(bt);
  bt.MessageEnd();
  CryptoPP::RSA::PrivateKey pKey;
  pKey.BERDecodePrivateKey(bt, false, 0);
  CryptoPP::AutoSeededRandomPool rng;
  if (!pKey.Validate(rng, 3)) {
    throw std::runtime_error("RSA key not valid!");
  }
  return pKey;
}

class LocalPeerOperationsTest : public QObject {
  Q_OBJECT

 public:
 private slots:
  void test_signatureCreation();
  void test_decryption();
};

void LocalPeerOperationsTest::test_signatureCreation() {
  std::string message = "Hello, World!";
  auto key = load(PRIVATE);
  LocalPeerOperations opers(key);
  auto sign = opers.createSign(message);

  std::cout << sign << std::endl;

  QVERIFY(!sign.empty());
}

void LocalPeerOperationsTest::test_decryption() {
  std::string TITUL = "Hellow world!";
  std::string message =
      "kP4gxh7fWlppl1cnXCtFqOW1uC6dIkvExsPyAFelai/gUzXJt2CjLiQPZpT8VG+jO6jvLOKf"
      "Lp3O/a983aSKUKnnD84ZdPFEn0tmppVoSo16Q3056hTUKJTeLLgZ9Hd3QE33sIQD/XXgYN9a"
      "hRNVl3SMvVpbjaHjag3zzP3NwGm1cUG01dSoIj/mf31tR81uEt9g55OaLCTTEXNuTceZXAFO"
      "jbNzuQ9uYkCqB/5DxElRDOA0CRdVDmQKRPs109rZLjlI8KQXn1B9lygkCdjn5ZTZcZ0+Saji"
      "DqcWB+7GHf5DGSeGjdPxlUmdvg0o15JVqhcZeIOfsAo3cvBA/2vU6w==";
  auto key = load(PRIVATE);
  LocalPeerOperations opers(key);

  auto res = opers.decrypt(message);
  std::cout << res << std::endl;

  QCOMPARE(TITUL, res);
}

QTEST_APPLESS_MAIN(LocalPeerOperationsTest)

#include "tst_localpeeroperationstest.moc"
