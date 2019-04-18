#ifndef AESKEYDESERIALIZATOR_H
#define AESKEYDESERIALIZATOR_H

#include <exception>
#include <memory>
#include <string>
#include "interfaces/decryptor.h"
#include "symetrical/aeskeystruct.h"
#include "utils/interfaces/objectdeserializer.h"

class BadFormatException : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};

/**
 * @brief Конструирует объект AesKeyStruct из строки.
 * Предполагается, что строка, из которой производится воссоздание объекта
 * зашифрована. Предполагается, что для каждой операции десериализиации
 * необходимо создавать новый объект.
 */
class AesKeyDeserializator : public ObjectDeserializer<AesKeyStruct> {
 public:
  /**
   * @brief Конструктор класса
   * @param decryptor дешифратор, с помощью которого можно дешифровать строку
   * message
   * @param message зашифрованное строковое представление объекта AesKeyStruct
   */
  AesKeyDeserializator(const std::shared_ptr<const Decryptor>& decryptor,
                       const std::string& message);

 public:
  /**
   * @brief Заполняет переданный объект данными из строки.
   * Метод дешифрует переданную в конструкторе строку, затем на основе
   * результата запроняет объект object
   * @param object объект, поля которого заполняются на основе данных,
   * полученных из строки message
   */
  virtual void deserialize(AesKeyStruct& object) noexcept(false) override;

 private:
  std::shared_ptr<const Decryptor> mKeyDecryptor;
  std::string mMessage;
};

#endif  // AESKEYDESERIALIZATOR_H
