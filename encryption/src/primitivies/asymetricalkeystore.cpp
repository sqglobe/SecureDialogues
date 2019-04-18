#include "asymetricalkeystore.h"

CryptoPP::RSA::PublicKey AsymetricalKeyStore::getPublicKey() const
    noexcept(false) {
  if (mPublicKey) {
    return mPublicKey.value();
  } else {
    throw KeyNotInitialised("Public key is uninitialized");
  }
}

CryptoPP::RSA::PrivateKey AsymetricalKeyStore::getPrivateKey() const
    noexcept(false) {
  if (mPrivateKey) {
    return mPrivateKey.value();
  } else {
    throw KeyNotInitialised("Private key is uninitialized");
  }
}

void AsymetricalKeyStore::setKeys(const CryptoPP::RSA::PublicKey& pub,
                                  const CryptoPP::RSA::PrivateKey& priv) {
  mPublicKey = pub;
  mPrivateKey = priv;
}
