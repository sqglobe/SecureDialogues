#ifndef SERIALIZATIONS_H
#define SERIALIZATIONS_H

#include <cryptopp/rsa.h>
#include <memory>
#include <stdexcept>

class Decryptor;
class Encryptor;
class AesKeyStruct;

class BadFormatException : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};

void deserialize(const std::shared_ptr<const Decryptor>& decryptor,
                 const std::string& message,
                 AesKeyStruct& object) noexcept(false);
std::string serialize(const std::shared_ptr<const Encryptor>& encryptor,
                      const AesKeyStruct& object) noexcept(false);

void deserialize(const std::string& message,
                 CryptoPP::RSA::PublicKey& object) noexcept(false);
std::string serialize(const CryptoPP::RSA::PublicKey& object) noexcept(false);

#endif  // SERIALIZATIONS_H
