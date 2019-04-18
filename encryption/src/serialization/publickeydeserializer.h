#ifndef PUBLICKEYDESERIALIZER_H
#define PUBLICKEYDESERIALIZER_H

#include <utils/interfaces/objectdeserializer.h>

#include <cryptopp/rsa.h>
#include <string>

/**
 * @brief Класс для востановления публичного коча из строки
 */
class PublicKeyDeserializer
    : public ObjectDeserializer<CryptoPP::RSA::PublicKey> {
 public:
  /**
   * @brief Конструктор класса
   * @param message строка, которая содержит публичныый ключ
   */
  PublicKeyDeserializer(const std::string& message);
  virtual void deserialize(CryptoPP::RSA::PublicKey& object) noexcept(
      false) override;

 private:
  std::string mMessage;
};

#endif  // PUBLICKEYDESERIALIZER_H
