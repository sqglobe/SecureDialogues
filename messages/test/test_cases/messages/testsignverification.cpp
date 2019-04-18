#include <QTest>

#include "asymetrical/localpeeroperations.h"
#include "asymetrical/remotepeeroperations.h"
#include "primitives/dialogmessage.h"
#include "serialization/serializations.h"
#include "utils/messagemarshaller.h"

#include <cryptopp/base64.h>
#include <cryptopp/osrng.h>
#include <cryptopp/rsa.h>
#include <iostream>

#include "asymetrical/asymetricalkeygenerator.h"
#include "asymetrical/asymetricalkeystore.h"

Q_DECLARE_METATYPE(std::string);
Q_DECLARE_METATYPE(DialogMessage::Action);

static const std::string PUBLIC =
    "MIIBoDANBgkqhkiG9w0BAQEFAAOCAY0AMIIBiAKCAYEAnZBloa3oCXe7/"
    "APNpXTgGHXCiMIn4Y0dNcaHrDyMtdEViZh83HjEw6qR"
    "4eiB3omJQw6mVI39d+gWgZz0HmcloD9hvbvOriZTmp7KkzXrZage15C8Y7hxJIfEpYe0rg/"
    "PWIQLTsKGEB6y3iDrqKG0i3aS5zLk"
    "8vgYk0AwyKk2jdaDUcTToT21fK1Dzn9xjO1yJxB4V10iL5sMvvvZ0IfziMRQHMpH5LGLR+gj/"
    "HrJPPw8xALlfuWGa/r7hP9sYdx3"
    "Rsqyhhn+v8UhW9nHZWRz0PzS6zL66glrWH9yWzHUayGEdb3DNbZTi72y883s3k5JPFD+"
    "rI88qgjcSTaYci9EDb1u0D3G5VAWzYG3"
    "ckWoF+"
    "QUKwd08qXovMTHKx0wjp9YeiyT0o7Fea3EY1J9UAZPpCfEYIYQhHXE0MfjAoMunnW73qrLODg3"
    "ln/EzVFA81eFgQxOsFbU"
    "ZkjGMJU2YFLhqp7iZFv4HReknA4zP2/r6691c8btFCGm2q1mOtLqHLkhAgER";

static const std::string PRIVATE =
    "MIIG+"
    "wIBADANBgkqhkiG9w0BAQEFAASCBuUwggbhAgEAAoIBgQCdkGWhregJd7v8A82ldOAYdcKIwif"
    "hjR01xoesPIy10RWJmHzc"
    "eMTDqpHh6IHeiYlDDqZUjf136BaBnPQeZyWgP2G9u86uJlOansqTNetlqB7XkLxjuHEkh8Slh7"
    "SuD89YhAtOwoYQHrLeIOuoobSL"
    "dpLnMuTy+BiTQDDIqTaN1oNRxNOhPbV8rUPOf3GM7XInEHhXXSIvmwy++9nQh/"
    "OIxFAcykfksYtH6CP8esk8/DzEAuV+5YZr+vuE"
    "/2xh3HdGyrKGGf6/xSFb2cdlZHPQ/"
    "NLrMvrqCWtYf3JbMdRrIYR1vcM1tlOLvbLzzezeTkk8UP6sjzyqCNxJNphyL0QNvW7QPcbl"
    "UBbNgbdyRagX5BQrB3Typei8xMcrHTCOn1h6LJPSjsV5rcRjUn1QBk+kJ8RghhCEdcTQx+"
    "MCgy6edbveqss4ODeWf8TNUUDzV4WB"
    "DE6wVtRmSMYwlTZgUuGqnuJkW/gdF6ScDjM/"
    "b+vrr3Vzxu0UIabarWY60uocuSECARECggGAAt5qi4x9DcwKPCmpCGQCpWAVHE4c"
    "sUh8OrRgaJqPHGiOu99H1K+737VNO6/"
    "5wnA37BEeRxjQoE0Cj6BCHTyt3I2z+cYbyRY13poyTIpNyY9AYdz8+e18ro8FX4SfQY5F"
    "Y351zuEfH9hm7ogg+"
    "WPAPVciOf2zI0IvxnIcIzSPVcO59xdNtpDmQyqYLm0qlTXNX13hNID6hvZGJeXAM3upuP0e2/"
    "PoNgBdP1ZV"
    "RX5Kp9VDy6LU2cuHd5F6LoW55/d3Bz2Fri4P7xHCOGz5I1/"
    "DPBpBdblb71SlG+LyJ8GMuB2HVNyRH+oyxH8U+OuvXCnjfRCnqkXm"
    "miQq3xJgB9K5E+XY+GQjc9FA2nIatE/Gy/"
    "kCTLIsLX8KX1wZ5T8K0OoXi3w9nZLhrS3Lqrb8Lan7ghCXk+KH4HSIRTGjf5xq4lcl"
    "e7kW2L4Ff3foQnL3OJyKN78zCuof45a0p9KeLkVX4XO4Mz1moqaQt4Pae50k+jFfzxIKJ+"
    "a4va9bSTQQGBALAoHBAMyRoZJqvlhD"
    "uiPxb3dzbSZSpQSFlvIzj68AePwZRUSYmzS+Y98Zhqn/+AzqxG6jx5LuHA0jVT/"
    "qDbNd5Dxi2f7Rz/hQ0g6SCDaGV3HY/lLkl0NB"
    "MhePZyXxN+jjw7wZvIQADBHHYv3zsrkOGKgRCOBxY4oWPc1G3QjV+rvqRsheSGsG7hzFu9b0b+"
    "N9ZGb7hCGYXtdzbKmhl63XhtK1"
    "OjKLLa76FfDEmMFctNek9rrjhLaTZcw20XT+EGwoT+"
    "aYWwKBwQDFLXaBfpowxbwJJL0cAvxjL4UbEEgo5/k3urawL8Dhbi8zhcZ9"
    "wF65jbEwoWG9csb70CUDzzL1vtcI08slqo2ir2Dv+"
    "uOHinNqgGo9dnlkptc4UWYEmfkhX9mdKRrFON2TDrIZuloZIemAPV3vpLOh"
    "E2WSTDXKfVyg1XH5gCDQoC2tUKeHmjoX0afjUPnJi0ZV+"
    "IZGWcte74ALPyI90MzKyeEZmASCIJ0ZBTwxTy2q0bFj8kXMHB8pwXFV"
    "MGFbTTMCgcBUO/c8ShIkWB94Njz080sPx61cNwHrQmhXHk/vVbMcPtZ/"
    "ITgusChkHhrYJG8AYY5prVbYO7mw5+eGF5o2+4bwdILP"
    "qM728NY0kauJOzuLi01X7as24LIAkIBu9F+nzlyu0tfLFd13oJTiyZGunZo+"
    "TM6iRWS98ACpSQzj9w4WRPCkimIL2PL+KGpOnQs5"
    "dp/RmRf+XLRF2SBWpApWwxf2sceESNvbnD7mNTtn2oO2Xa8eAHU1+HRdWY5Kpy/1igcCgcB/"
    "leNEu1S2JZfJrlw/TTnl03Q+sC6x"
    "DpIzHnY1xIvdKS2Zzwf3BAEOpvouhoqJpJ7fLFQvpDAIbG0FtjgnblulgIoEsWYMaKUIrXHriO"
    "UjAotCjwXGvfuOENgaZeQlQum5"
    "gfrFWnaIu5cWvkvXTHQ7DI0ET24ZnGkcxljOnjNZ0Q5/"
    "NDAqkPhpw+Uanc7NtHjON2XxSSk9bctSkkNVLMC/c5GnJiEI5+0uTq56"
    "Qkq50vpPuuHebIyijDpGLl0c9bcCgcEAgFEsOtVltyGw3aW5J0psR9iBf1mtEX9xKFntGjfuTB"
    "12YTVf+dSUfKj5xp+KLKgXAkk3"
    "AK8gK6Ug9q9Fzf17jlmqez/"
    "z7HlrpAmUGEiOZ502kSWQzWYVt5b+sAuVCT1ID40YE5i0j+"
    "dovC89aHAvcMrcMKTMTO42VYBulRk3"
    "aLgBw4DRXgkDiKCnRWWkfkqCSrQMWAXbKQoyRPwbuaIYZAKMApRUYNFR+"
    "1dVNcFY9NvdZahwcj40rRH6zepd9qRG";

static const std::string MESSAGE =
    "SECURE DIALOG MESSAGE\n"
    "VERSION:1.0\n"
    "ACTION:MESSAGE\n"
    "SEQUENTAL_NUMBER:1\n"
    "DIALOG:dialog 1\n"
    "TIMESTAMP:1546596522\n"
    "SIGNATURE:SLVT6l1bAWLr4wc2mHbZoNwd7w0iQ0TtuxRm6U9YHQX7/"
    "FKEpQ87+QDyJPCyC6xZleGQV5JE\n"
    "gmwaDnamZAlc+z6+6r9MSnOtA0N4beEA5QtGmjcJ+IQlgf0gex1HiMICR8BY9avS6ieN77z1\n"
    "6qZ9ImW0lwGCIPImK4MEWEoBvO8RI0qdv9UOYXvDYvWqHpkgme4zOcwTeA0eH+KH7d11slRG\n"
    "06iWnG/k7VTAzUWK+fOf9XNVzc+TO38NP8vl34CF0Fhx44bo8Nx4LmPOvmR4luzZyIPBJkBw\n"
    "iHY0zwbWprKV8Ym7OfyNa7NG7jWsr0p5V4dGKXlRl/oN9vlZECtBQZ/sCvTVAEOd9+SKFLrk\n"
    "2bg83KvzaHkUKcAPZ8+wWHdUXgQ+631UGqLuJKv1J3PpwWknXoXqIAg25Eb3oFPxzeAate8M\n"
    "pOF/0gV2FYHEOML58Zfhh2R6KBAKcMXhyA2cjmUbjkKQLW2PwoshTarzV4z1/dq/4Yki55L4\n"
    "S+pqvXhI\n"
    "\n"
    "--------------------------------------------------------------\n"
    "test";

CryptoPP::RSA::PrivateKey loadPrivate(const std::string& key) {
  CryptoPP::ByteQueue bt;
  CryptoPP::StringSource ss(key, true /*pumpAll*/,
                            new CryptoPP::Base64Decoder());
  ss.TransferTo(bt);
  bt.MessageEnd();
  CryptoPP::RSA::PrivateKey pKey;
  // pKey.BERDecodePrivateKey(bt, false, 0);
  pKey.Load(bt);
  CryptoPP::AutoSeededRandomPool rng;
  if (!pKey.Validate(rng, 3)) {
    throw std::runtime_error("RSA key not valid!");
  }
  return pKey;
}

CryptoPP::RSA::PublicKey loadPublic(const std::string& key) {
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

class TestSignVerification : public QObject {
  Q_OBJECT

 private Q_SLOTS:
  void testSignVerificcation();
  void testKeyGeneration();
  void testSignCreation();
};

std::string serialize(const CryptoPP::RSA::PrivateKey& object) {
  std::string res;
  CryptoPP::Base64Encoder encoder(new CryptoPP::StringSink(res), true, 100);
  CryptoPP::ByteQueue queue;
  object.Save(queue);
  queue.CopyTo(encoder);
  encoder.MessageEnd();
  return res;
}

void TestSignVerification::testSignVerificcation() {
  CryptoPP::RSA::PublicKey pub;
  deserialize(PUBLIC, pub);

  RemotePeerOperations oper(pub);

  MessageMarshaller mm;

  auto unmMessage = mm.unmarshall(MESSAGE, "test");

  QVERIFY(unmMessage);

  auto message = unmMessage.value();

  DialogMessage messWithoutSign(message.action(), message.content(),
                                message.dialogId(), message.adress(),
                                message.sequential(), "", message.timestamp());
  auto strRepr = mm.marshall(messWithoutSign);

  QVERIFY(oper.isValid(strRepr, message.sign()));
}

void TestSignVerification::testKeyGeneration() {
  /* auto keyStore = std::make_shared<AsymetricalKeyStore>();
   AsymetricalKeyGenerator gen(keyStore);

   gen.generate();

   std::cout << serialize(keyStore->getPrivateKey()) << std::endl << std::endl;
   std::cout << serialize(keyStore->getPublicKey()) << std::endl << std::endl;
   */
}

void TestSignVerification::testSignCreation() {
  /*  DialogMessage mess(DialogMessage::Action::MESSAGE, "test", "dialog 1",
    "tom", 1); MessageMarshaller mm; auto messText = mm.marshall(mess);
    LocalPeerOperations oper(loadPrivate(PRIVATE));
    mess.setSignature(oper.createSign(messText));

    std::cout << mm.marshall(mess) << std::endl;*/
}

QTEST_APPLESS_MAIN(TestSignVerification)

#include "testsignverification.moc"
