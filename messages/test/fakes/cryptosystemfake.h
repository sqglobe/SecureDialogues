#ifndef CRYPTOSYSTEMFAKE_H
#define CRYPTOSYSTEMFAKE_H

#include "interfaces/cryptosystem.h"

class CryptoSystemFake : public CryptoSystem {
 public:
  virtual bool isSignatureOk(const DialogMessage& message) const
      noexcept(false) override;
  virtual std::string createSignature(const DialogMessage& message) const
      noexcept(false) override;

  virtual std::string encryptMessageBody(const std::string& dialogId,
                                         const std::string& message) const
      noexcept(false) override;
  virtual std::string decryptMessageBody(const std::string& dialogId,
                                         const std::string& message) const
      noexcept(false) override;

 public:
  virtual std::string generateAndExportKey(
      const std::string& dialogId,
      const std::string& addres) noexcept(false) override;
  virtual std::string importKey(
      const std::string& dialogId,
      const std::string& message) noexcept(false) override;
  virtual bool checkVerificationString(
      const std::string& dialogId,
      const std::string& message) noexcept(false) override;
};

bool CryptoSystemFake::isSignatureOk([
    [maybe_unused]] const DialogMessage& message) const {
  return true;
}

std::string CryptoSystemFake::createSignature([
    [maybe_unused]] const DialogMessage& message) const {
  return "1";
}

std::string CryptoSystemFake::encryptMessageBody(
    [[maybe_unused]] const std::string& dialogId,
    [[maybe_unused]] const std::string& message) const {
  return message;
}

std::string CryptoSystemFake::decryptMessageBody(
    [[maybe_unused]] const std::string& dialogId,
    [[maybe_unused]] const std::string& message) const {
  return message;
}

std::string CryptoSystemFake::generateAndExportKey(
    [[maybe_unused]] const std::string& dialogId,
    [[maybe_unused]] const std::string& addres) {
  return "1";
}

std::string CryptoSystemFake::importKey(
    [[maybe_unused]] const std::string& dialogId,
    [[maybe_unused]] const std::string& message) {
  return "1";
}

bool CryptoSystemFake::checkVerificationString(
    [[maybe_unused]] const std::string& dialogId,
    [[maybe_unused]] const std::string& message) {
  return true;
}

#endif  // CRYPTOSYSTEMFAKE_H
