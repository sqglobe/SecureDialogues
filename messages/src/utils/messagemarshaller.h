#ifndef MESSAGEMARSHALLER_H
#define MESSAGEMARSHALLER_H

#include "interfaces/abstractmessagemarshaller.h"

#include "primitives/dialogmessage.h"

struct MessageFields;

class MessageMarshaller : public AbstractMessageMarshaller {
 public:
  std::optional<DialogMessage> unmarshall(const std::string& message,
                                          const std::string& address) override;
  std::string marshall(const DialogMessage& message) override;
  std::string marshallExceptSignature(const DialogMessage& message) override;

 private:
  /**
   * @brief Выполняет конвертацию типа строки к типу сообщение.
   * @param action - строковое представление типа сообщения
   * @return тип сообщения
   */
  static DialogMessage::Action convertAction(const std::string& action);

  /**
   * @brief Конвертирует тип сообщения в строку
   * @param action тип сообщения
   * @return  строковое представление типа сообщения
   */
  static std::string convertAction(DialogMessage::Action action);

  static bool splitKeyVal(const std::string& source,
                          std::string& key,
                          std::string& val,
                          char ch = ':');
  bool prepareLine(const std::string& line,
                   int lineNumber,
                   MessageFields& fields);
};

#endif  // MESSAGEMARSHALLER_H
