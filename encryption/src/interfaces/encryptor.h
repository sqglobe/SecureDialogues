#ifndef ENCRYPTOR_H
#define ENCRYPTOR_H
#include <string>

/**
 * @brief Интерфейс для шифратора
 * Предполагается передавать параметры, необходимые для шифрования, вроде
 * ключа через конструктор
 */
class Encryptor {
 public:
  virtual ~Encryptor() = default;

  /**
   * @brief Шифрует указанную строку
   * @param source строка для шифрования
   * @return результат шифрования
   */
  virtual std::string encrypt(const std::string& source) const = 0;
};
#endif  // ENCRYPTOR_H
