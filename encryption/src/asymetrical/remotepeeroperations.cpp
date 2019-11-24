#include "remotepeeroperations.h"

#include <cryptopp/base64.h>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>
#include <cryptopp/pssr.h>

RemotePeerOperations::RemotePeerOperations(
    const CryptoPP::RSA::PublicKey& key) :
    mKey(key) {}

std::string RemotePeerOperations::encrypt(const std::string& source) const {
  std::shared_lock<std::shared_mutex> guard(mMutex);
  CryptoPP::AutoSeededRandomPool rng;
  std::string cipher;
  CryptoPP::RSAES_OAEP_SHA_Encryptor e(mKey);
  CryptoPP::StringSource(
      source, true,
      new CryptoPP::PK_EncryptorFilter(
          rng, e,
          new CryptoPP::Base64Encoder(
              new CryptoPP::StringSink(cipher)))  // PK_EncryptorFilter
  );                                              // StringSource
  return cipher;
}

std::string RemotePeerOperations::encrypt(std::string_view sourc) const {
  std::shared_lock<std::shared_mutex> guard(mMutex);
  CryptoPP::AutoSeededRandomPool rng;
  std::string cipher;
  CryptoPP::RSAES_OAEP_SHA_Encryptor e(mKey);
  CryptoPP::StringSource(
      reinterpret_cast<const unsigned char*>(sourc.data()), sourc.size(), true,
      new CryptoPP::PK_EncryptorFilter(
          rng, e,
          new CryptoPP::Base64Encoder(
              new CryptoPP::StringSink(cipher)))  // PK_EncryptorFilter
  );                                              // StringSource
  return cipher;
}

bool RemotePeerOperations::isValid(const std::string& message,
                                   const std::string& signature) const {
  std::shared_lock<std::shared_mutex> guard(mMutex);
  std::string out;
  CryptoPP::StringSource ss(
      signature, true /*pumpAll*/,
      new CryptoPP::Base64Decoder(new CryptoPP::StringSink(out)));

  CryptoPP::SecByteBlock sign(
      reinterpret_cast<const unsigned char*>(out.data()), out.size());

  CryptoPP::RSASS<CryptoPP::PSS, CryptoPP::SHA256>::Verifier verifier(mKey);
  return verifier.VerifyMessage(
      reinterpret_cast<const unsigned char*>(message.data()), message.length(),
      sign, sign.size());
}

bool RemotePeerOperations::isValid(std::string_view message,
                                   std::string_view signature) const {
  std::shared_lock<std::shared_mutex> guard(mMutex);
  std::string out;
  CryptoPP::StringSource ss(
      reinterpret_cast<const unsigned char*>(signature.data()),
      signature.size(), true /*pumpAll*/,
      new CryptoPP::Base64Decoder(new CryptoPP::StringSink(out)));

  CryptoPP::SecByteBlock sign(
      reinterpret_cast<const unsigned char*>(out.data()), out.size());

  CryptoPP::RSASS<CryptoPP::PSS, CryptoPP::SHA256>::Verifier verifier(mKey);
  return verifier.VerifyMessage(
      reinterpret_cast<const unsigned char*>(message.data()), message.length(),
      sign, sign.size());
}

void RemotePeerOperations::updateKey(const CryptoPP::RSA::PublicKey& key) {
  std::unique_lock<std::shared_mutex> guard(mMutex);
  mKey = key;
}
