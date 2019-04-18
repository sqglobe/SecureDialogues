#ifndef AESKEYSERIALIZATOR_H
#define AESKEYSERIALIZATOR_H

#include <memory>
#include <string>
#include "interfaces/encryptor.h"
#include "symetrical/aeskeystruct.h"
#include "utils/interfaces/objectserializer.h"

/**
 * @brief Класс создает строковое представление объекта
 * Предполагается, что для каждой операции необходимо создавать новый объект
 */
class AesKeySerializator : public ObjectSerializer<AesKeyStruct> {
 public:
  /**
   * @brief Конструктор класса
   * @param encryptor шифратор, с помощью которого будет зашифровано строковое
   * представление объекта
   * @param ss строка, в которую будет помещен результат шифрования
   */
  AesKeySerializator(const std::shared_ptr<const Encryptor>& encryptor,
                     std::string& ss);

 public:
  /**
   * @brief Создает строковое представление object, шифрует его и записывает в
   * ссылку на строку, переданную в конструкторе.
   * @param object объект, для которого необходимо создать строковое
   * представление.
   */
  virtual void serialize(const AesKeyStruct& object) noexcept(false) override;

 private:
  std::shared_ptr<const Encryptor> mKeyEncryptor;
  std::string& mOut;
};

#endif  // AESKEYSERIALIZATOR_H
