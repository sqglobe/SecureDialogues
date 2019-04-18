#ifndef PUBLICKEYSERIALIZATOR_H
#define PUBLICKEYSERIALIZATOR_H

#include <utils/interfaces/objectserializer.h>

#include <cryptopp/rsa.h>
#include <string>

/**
 * @brief Класс сериализует пебличный ключ в строку
 */
class PublicKeySerializator : ObjectSerializer<CryptoPP::RSA::PublicKey> {
 public:
  /**
   * @brief Конструктор класса
   * @param out строка в которую будет помещено строковое представление объекта
   */
  PublicKeySerializator(std::string& out);

 public:
  virtual void serialize(const CryptoPP::RSA::PublicKey& object) noexcept(
      false) override;

 private:
  std::string& mOut;
};

#endif  // PUBLICKEYSERIALIZATOR_H
