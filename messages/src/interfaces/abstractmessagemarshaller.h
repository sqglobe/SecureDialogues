#ifndef ABSTRACTMESSAGEMARSHALLER_H
#define ABSTRACTMESSAGEMARSHALLER_H

#include <optional>
#include <string>

class DialogMessage;

/**
 * Класс выполняет маршалинг/демаршалинг сообщение из объекта в строку и
 * обратною
 */
class AbstractMessageMarshaller {
 public:
  virtual ~AbstractMessageMarshaller() = default;

 public:
  /**
   * @brief Выполняет разбор текстового сообщения в строку.
   * @param message текстовое сообщение, которое необходимо разобрать
   * @param address адрес, от которого получено сообщение
   * @return объект optional, который содержит объект сообщения, если разбор
   * прошел успешно. В противном случае он не содержит ничего.
   */
  virtual std::optional<DialogMessage> unmarshall(
      const std::string& message,
      const std::string& address) = 0;

  /**
   * @brief Преобразует объект сообщения в текстовую строку
   * @param message объект, который необходимо преобразовать
   * @return строковое представление сообщения
   */
  virtual std::string marshall(const DialogMessage& message) = 0;
};

#endif  // ABSTRACTMESSAGEMARSHALLER_H
