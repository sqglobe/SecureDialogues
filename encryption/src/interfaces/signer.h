#ifndef SIGNER_H
#define SIGNER_H

#include <string>

/**
 * @brief Интерфейс, который создает цифровую подпись
 */
class Signer {
 public:
  virtual ~Signer() = default;

  /**
   * @brief Вычисляет подпись для указанного сообщения
   * @param message сообщение, для которого необходимо вычислить подпись.
   * @return строковое представление цифровой подписи
   */
  virtual std::string createSign(const std::string& message) const = 0;
};

#endif  // SIGNER_H
