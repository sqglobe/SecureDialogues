#ifndef REMOTEPEEROPERATIONS_H
#define REMOTEPEEROPERATIONS_H

#include "interfaces/encryptor.h"
#include "interfaces/signaturechecker.h"

#include <cryptopp/rsa.h>
#include <shared_mutex>

/**
 * @brief Реализует операции, которые может выполнить пользователь с пришедшим
 * сообщением, воспользовавшись открытым ключем собеседника
 */
class RemotePeerOperations : public Encryptor, public SignatureChecker {
 public:
  explicit RemotePeerOperations(const CryptoPP::RSA::PublicKey& key);

 public:
  std::string encrypt(const std::string& sourc) const override;
  bool isValid(const std::string& message,
               const std::string& signature) const override;

  void updateKey(const CryptoPP::RSA::PublicKey& key);

 private:
  CryptoPP::RSA::PublicKey mKey;
  mutable std::shared_mutex mMutex;
};

#endif  // REMOTEPEEROPERATIONS_H
