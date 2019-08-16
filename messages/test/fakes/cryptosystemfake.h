#ifndef CRYPTOSYSTEMFAKE_H
#define CRYPTOSYSTEMFAKE_H

#include "interfaces/cryptosystem.h"

class CryptoSystemFake : public CryptoSystem {
 public:
  bool isSignatureOk(const DialogMessage& message) const
      noexcept(false) override;
  std::string createSignature(const DialogMessage& message) const
      noexcept(false) override;

  std::string encryptMessageBody(std::string_view dialogId,
                                 std::string_view message) const
      noexcept(false) override;
  std::string decryptMessageBody(std::string_view dialogId,
                                 std::string_view message) const
      noexcept(false) override;

 public:
  std::string generateAndExportKey(
      std::string_view dialogId,
      std::string_view addres) noexcept(false) override;
  std::string importKey(std::string_view dialogId,
                        std::string_view message) noexcept(false) override;
  bool checkVerificationString(
      std::string_view dialogId,
      std::string_view message) noexcept(false) override;
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
    std::string_view,
    std::string_view message) const {
  return std::string(message);
}

inline std::string CryptoSystemFake::decryptMessageBody(
    std::string_view,
    std::string_view message) const {
  return std::string(message);
}

inline std::string CryptoSystemFake::generateAndExportKey(std::string_view,
                                                          std::string_view) {
  return "1";
}

inline std::string CryptoSystemFake::importKey(std::string_view,
                                               std::string_view) {
  return "1";
}

inline bool CryptoSystemFake::checkVerificationString(std::string_view,
                                                      std::string_view) {
  return true;
}

#endif  // CRYPTOSYSTEMFAKE_H
