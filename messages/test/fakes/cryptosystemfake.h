#ifndef CRYPTOSYSTEMFAKE_H
#define CRYPTOSYSTEMFAKE_H

#include "interfaces/cryptosystem.h"

class CryptoSystemFake : public CryptoSystem {
 public:
  bool isSignatureOk(const DialogMessage& message) const
      noexcept(false) override;
  std::string createSignature(const DialogMessage& message) const
      noexcept(false) override;

  std::string encryptMessageBody(const std::string& dialogId,
                                 const std::string& message) const
      noexcept(false) override;
  std::string decryptMessageBody(const std::string& dialogId,
                                 const std::string& message) const
      noexcept(false) override;

 public:
  std::string generateAndExportKey(
      const std::string& dialogId,
      const std::string& addres) noexcept(false) override;
  std::string importKey(const std::string& dialogId,
                        const std::string& message) noexcept(false) override;
  bool checkVerificationString(
      const std::string& dialogId,
      const std::string& message) noexcept(false) override;
};

inline bool CryptoSystemFake::isSignatureOk([
    [maybe_unused]] const DialogMessage& message) const {
  return true;
}

inline std::string CryptoSystemFake::createSignature([
    [maybe_unused]] const DialogMessage& message) const {
  return "1";
}

inline std::string CryptoSystemFake::encryptMessageBody(
    [[maybe_unused]] const std::string& dialogId,
    [[maybe_unused]] const std::string& message) const {
  return message;
}

inline std::string CryptoSystemFake::decryptMessageBody(
    [[maybe_unused]] const std::string& dialogId,
    [[maybe_unused]] const std::string& message) const {
  return message;
}

inline std::string CryptoSystemFake::generateAndExportKey(
    [[maybe_unused]] const std::string& dialogId,
    [[maybe_unused]] const std::string& addres) {
  return "1";
}

inline std::string CryptoSystemFake::importKey(
    [[maybe_unused]] const std::string& dialogId,
    [[maybe_unused]] const std::string& message) {
  return "1";
}

inline bool CryptoSystemFake::checkVerificationString(
    [[maybe_unused]] const std::string& dialogId,
    [[maybe_unused]] const std::string& message) {
  return true;
}

#endif  // CRYPTOSYSTEMFAKE_H
