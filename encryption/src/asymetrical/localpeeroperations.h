#ifndef LOCALPEEROPERATIONS_H
#define LOCALPEEROPERATIONS_H

#include "interfaces/decryptor.h"
#include "interfaces/signer.h"

#include <cryptopp/rsa.h>
#include <shared_mutex>

/**
 * @brief Реализует операции, которые выполняются закрытым ключем, т.е. те, что
 * может выполнить локальный пользователь с отправляемым сообщением
 */
class LocalPeerOperations : public Decryptor, public Signer {
 public:
  explicit LocalPeerOperations(const CryptoPP::RSA::PrivateKey& key);

 public:
  virtual std::string decrypt(const std::string& data) const override;
  virtual std::string createSign(const std::string& message) const override;

  void updateKey(const CryptoPP::RSA::PrivateKey& key);

 private:
  CryptoPP::RSA::PrivateKey mKey;
  mutable std::shared_mutex mMutex;
};

#endif  // LOCALPEEROPERATIONS_H
