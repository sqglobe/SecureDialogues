#ifndef AESCIPHER_H
#define AESCIPHER_H

#include "interfaces/symetricalcipher.h"

#include <cryptopp/secblock.h>

/**
 * @brief Шифратор для алгоритма AES
 */
class AesCipher : public SymetricalCipher {
 public:
  AesCipher(const CryptoPP::SecByteBlock& key,
            const CryptoPP::SecByteBlock& iv);

 public:
  std::string encrypt(const std::string& source) const override;
  std::string encrypt(std::string_view source) const override;

  std::string decrypt(const std::string& data) const override;
  std::string decrypt(std::string_view data) const override;

 private:
  CryptoPP::SecByteBlock mKey;
  CryptoPP::SecByteBlock mIv;
};

#endif  // AESCIPHER_H
