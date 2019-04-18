#ifndef ABSTRACTMESSAGEGEHANDLER_H
#define ABSTRACTMESSAGEGEHANDLER_H

#include "primitives/dialogmessage.h"

/**
 * @brief Интерфейс для обработчика типов сообщений
 * Класс используется в MessageDespatcher для выполнения обработки сообщений
 * Именно в реализациях этого класса реализуется конечная логика,
 * характерная для конкретного типа сообщения
 */
class AbstractMessageHandler {
 public:
  /**
   * @brief Обработка сообщения, поступившего по сети.
   * Вызывается MessageDespatcher дял поступивших по сети сообщений,
   * для типа которых метод isItYouAction вернул true
   * @param message сообщение для обработки
   * @param channel идентификатор канала, по которому пришло сообщение
   */
  virtual void handle(const DialogMessage& message,
                      const std::string& channel) noexcept = 0;

  /**
   * @brief Определяет обрабатывается ли переданный тип сообщения данным классом
   * @param action тип сообщения
   * @return true, если переданный тип обрабатывается, в противном случае -
   * false
   */
  virtual bool isItYouAction(DialogMessage::Action action) const noexcept = 0;

 public:
  virtual ~AbstractMessageHandler() = default;
};
#endif  // ABSTRACTMESSAGEGEHANDLER_H
