#include "publickeyserializator.h"

#include <cryptopp/base64.h>
#include <cryptopp/files.h>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>
#include <cryptopp/rsa.h>

PublicKeySerializator::PublicKeySerializator(std::string& out) : mOut(out) {}

void PublicKeySerializator::serialize(
    const CryptoPP::RSA::PublicKey& object) noexcept(false) {
  CryptoPP::Base64Encoder encoder(new CryptoPP::StringSink(mOut), true, 100);
  CryptoPP::ByteQueue queue;
  object.Save(queue);
  queue.CopyTo(encoder);
  encoder.MessageEnd();
}
