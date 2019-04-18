#ifndef SIGNATURECHECKER_H
#define SIGNATURECHECKER_H

#include <string>
/**
 * @brief Интерфейс, который выполняет проверку цифровой подписи для указанного
 * сообщения
 */
class SignatureChecker {
 public:
  virtual ~SignatureChecker() = default;

  /**
   * @brief Выполняет проверку цифровой подписи. Возвращает успешность проверки
   * @param message сообщение, для которого выполняется проверка цифровой
   * подписи
   * @param signature цифровая подпись
   * @return true, если подпись соответствует сообщению, false во всех остальных
   * случаях
   */
  virtual bool isValid(const std::string& message,
                       const std::string& signature) const = 0;
};

#endif  // SIGNATURECHECKER_H
