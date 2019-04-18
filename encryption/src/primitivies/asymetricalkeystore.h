#ifndef ASYMETRICALKEYSTORE_H
#define ASYMETRICALKEYSTORE_H
#include <cryptopp/rsa.h>
#include <exception>
#include <optional>

class KeyNotInitialised : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};

class AsymetricalKeyStore {
 public:
  CryptoPP::RSA::PublicKey getPublicKey() const noexcept(false);
  CryptoPP::RSA::PrivateKey getPrivateKey() const noexcept(false);

  void setKeys(const CryptoPP::RSA::PublicKey& pub,
               const CryptoPP::RSA::PrivateKey& priv);

 private:
  std::optional<CryptoPP::RSA::PublicKey> mPublicKey;
  std::optional<CryptoPP::RSA::PrivateKey> mPrivateKey;
};

#endif  // ASYMETRICALKEYSTORE_H
