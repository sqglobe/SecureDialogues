#include "publickeydeserializer.h"
#include <cryptopp/base64.h>
#include <cryptopp/osrng.h>
#include <cryptopp/rsa.h>

PublicKeyDeserializer::PublicKeyDeserializer(const std::string& message) :
    mMessage(message) {}

void PublicKeyDeserializer::deserialize(
    CryptoPP::RSA::PublicKey& object) noexcept(false) {
  CryptoPP::ByteQueue bt;
  CryptoPP::StringSource ss(mMessage, true /*pumpAll*/,
                            new CryptoPP::Base64Decoder());
  ss.TransferTo(bt);
  bt.MessageEnd();
  object.Load(bt);
  CryptoPP::AutoSeededRandomPool rng;
  if (!object.Validate(rng, 3)) {
    throw std::runtime_error("RSA key not valid!");
  }
}
