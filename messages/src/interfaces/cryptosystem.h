#ifndef CRYPTOSYSTEM_H
#define CRYPTOSYSTEM_H

#include <string>

class DialogMessage;

/**
 * @brief Абстрактный класс, который инкапсулирует в себе криптографический
 * функционал, который необходим приложению
 */
class CryptoSystem {
 public:
  virtual ~CryptoSystem() = default;

 public:
  /**
   * @brief Выполняет проверку подписи для указанного сообщения.
   * @param message сообщение, для которого необходимо выполнить проверку
   * подписи.
   * @return true, если проверка прошла успешно.
   */
  virtual bool isSignatureOk(const DialogMessage& message) const
      noexcept(false) = 0;

  /**
   * @brief Создает цифровую подпись для сообщения
   * @param message сообщение, для которого необходимо создать цифровую подпись
   * @return цифровая подпись для указанного сообщения
   */
  virtual std::string createSignature(const DialogMessage& message) const
      noexcept(false) = 0;

  /**
   * @brief Шифрует сообщение сеансовым ключем для указанного сообщения
   * @param dialogId идентификатор диалога, к которому принадлежит сообщение
   * @param message - тест сообщения, которое подлежит шифрованию
   * @return зашифрованый тест сообщения
   */
  virtual std::string encryptMessageBody(std::string_view dialogId,
                                         std::string_view message) const
      noexcept(false) = 0;

  /**
   * @brief Дешифрует тест сообщения сенсовым ключ указанного диалога
   * @param dialogId диалог, к которому принадлежит сообщение
   * @param message текст для дешифровки
   * @return текст сообщения после дишифрования
   */
  virtual std::string decryptMessageBody(std::string_view dialogId,
                                         std::string_view message) const
      noexcept(false) = 0;

 public:
  /**
   * @brief Генерирует и экспортирует сенсовый ключ для указанного диалога и
   * адреса Предполагается, что в реализации ключ будет зашифрован. Так же
   * результирующая строка содержит строку для проверки ключа.
   * @param dialogId идентификатор диалога, для которога выполняется генерация
   * @param addres адрес, для которого выполняется генерация
   * @return строковое представление сенсового ключа, готовое для передачи по
   * сети
   */
  virtual std::string generateAndExportKey(
      std::string_view dialogId,
      std::string_view addres) noexcept(false) = 0;

  /**
   * @brief Импортирует сенсовый ключ.
   * Дешифрует сенсовый ключ, выполняется его импорт. Возвращается проверочная
   * строка, зашифрованная сенсовым ключем
   * @param dialogId диалог, для которого выполняется импорт ключа
   * @param message текст сообщения с зашифрованным сеансовым ключем
   * @return зашифрованная проверочная строка сеансовым ключем
   */
  virtual std::string importKey(std::string_view dialogId,
                                std::string_view message) noexcept(false) = 0;

  /**
   * @brief Выполняется контроль проверочной строки.
   * @param dialogId диалог, для которого необходимо выполнить проверку
   * @param message проверочная строка
   * @return true - удалось расшифровать проверочную строку и она соответствует
   * отправленной, false - в любом другом случае.
   */
  virtual bool checkVerificationString(
      std::string_view dialogId,
      std::string_view message) noexcept(false) = 0;

  /**
   * @brief Экспортирует текущее значение публичного ключа пользователя
   * @return строковое представление публичного ключа пользователя
   */
  virtual std::string exportPublicKey() const noexcept(false) = 0;
};

#endif  // CRYPTOSYSTEM_H
