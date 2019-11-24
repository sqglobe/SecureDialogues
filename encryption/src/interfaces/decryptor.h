#ifndef DECRYPTOR_H
#define DECRYPTOR_H
#include <string>

/**
 * @brief Интерфейс дешифратора
 * Предполагается передача ключа и дополнительных параметров через конструктор
 */
class Decryptor {
 public:
  virtual ~Decryptor() = default;

  /**
   * @brief Выполняет дешифрование указанной строки
   * @param data строка для выполнения дешифрования
   * @return результат дешифрования
   * @throws Может выбрасывать исключения, если дешифрование не удалось
   */
  virtual std::string decrypt(const std::string& data) const = 0;
  virtual std::string decrypt(std::string_view data) const = 0;
};

#endif  // DECRYPTOR_H
