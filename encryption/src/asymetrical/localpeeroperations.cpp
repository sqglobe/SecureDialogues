#include "localpeeroperations.h"

#include <cryptopp/base64.h>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>
#include <cryptopp/pssr.h>

LocalPeerOperations::LocalPeerOperations(const CryptoPP::RSA::PrivateKey& key) :
    mKey(key) {}

std::string LocalPeerOperations::decrypt(const std::string& data) const {
  std::shared_lock<std::shared_mutex> guard(mMutex);
  using namespace CryptoPP;
  AutoSeededRandomPool rng;
  std::string recover;
  RSAES_OAEP_SHA_Decryptor d(mKey);

  StringSource(data, true,
               new Base64Decoder(new PK_DecryptorFilter(
                   rng, d, new StringSink(recover)))  // PK_EncryptorFilter
  );                                                  // StringSource
  return recover;
}

std::string LocalPeerOperations::createSign(const std::string& message) const {
  std::shared_lock<std::shared_mutex> guard(mMutex);
  CryptoPP::AutoSeededRandomPool rng;

  CryptoPP::RSASS<CryptoPP::PSS, CryptoPP::SHA256>::Signer signer(mKey);
  size_t length = signer.MaxSignatureLength();
  CryptoPP::SecByteBlock signature(length);

  // Sign message
  length = signer.SignMessage(
      rng, reinterpret_cast<const unsigned char*>(message.data()),
      message.length(), signature);

  // Resize now we know the true size of the signature
  signature.resize(length);

  std::string str, out;
  str.resize(length);  // Make room for elements
  std::memcpy(&str[0], &signature[0], str.size());
  CryptoPP::StringSource ss(
      str, true /*pumpAll*/,
      new CryptoPP::Base64Encoder(new CryptoPP::StringSink(out)));
  return out;
}

void LocalPeerOperations::updateKey(const CryptoPP::RSA::PrivateKey& key) {
  std::unique_lock<std::shared_mutex> guard(mMutex);
  mKey = key;
}
