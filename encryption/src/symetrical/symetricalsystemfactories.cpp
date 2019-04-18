#include "symetricalsystemfactories.h"
#include "aescipher.h"
#include "interfaces/decryptor.h"
#include "interfaces/encryptor.h"
#include "serialization/serializations.h"

#include "aeskeygenerator.h"

#include <cryptopp/aes.h>
#include <cryptopp/hkdf.h>
#include <cryptopp/sha.h>

std::unique_ptr<SymetricalCipher> createSymmetricalCipherFrom(
    const std::string& message,
    const std::shared_ptr<const Decryptor>& decryptor) {
  AesKeyStruct key;
  deserialize(decryptor, message, key);
  return std::make_unique<AesCipher>(key.key, key.iv);
}

std::unique_ptr<SymetricalCipher> generateSymmetricalCipherTo(
    std::string& message,
    const std::shared_ptr<const Encryptor>& encryptor) {
  auto key = AesKeyGenerator().generate();
  message = serialize(encryptor, key);
  return std::make_unique<AesCipher>(key.key, key.iv);
}

std::unique_ptr<SymetricalCipher> makeForStringPass(const std::string& pass) {
  CryptoPP::SecByteBlock key(
      CryptoPP::AES::MAX_KEYLENGTH);  // AES::MAX_KEYLENGTH+AES::BLOCKSIZE
  CryptoPP::HKDF<CryptoPP::SHA256> hkdf;
  hkdf.DeriveKey(key, key.size(),
                 reinterpret_cast<const unsigned char*>(pass.data()),
                 pass.size(), nullptr, 0, nullptr, 0);
  return std::make_unique<AesCipher>(key, CryptoPP::SecByteBlock());
}
