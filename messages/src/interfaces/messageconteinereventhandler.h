#ifndef MESSAGECONTEINEREVENTHANDLER_H
#define MESSAGECONTEINEREVENTHANDLER_H

#include <memory>
#include <string>
class UserMessage;
/**
 * @brief Интерфейс обработчика событий контейнера сообщений.
 * Обрабатывает такие события, как получение нового сообщения,
 * изменение активного диалога.
 */
class MessageContainerEventHandler {
 public:
  virtual ~MessageContainerEventHandler() = default;

 public:
  /**
   * @brief Вызывается, когда было получено или отправлено новое сообщение
   * @param dialogId id диалога, к которому было привязано данное сообщение
   * @param message - содержит информацию о сообщении, вроде текста, входящее
   * или нет и пр.
   */
  virtual void messageAdded(
      const std::string& dialogId,
      const std::shared_ptr<const UserMessage>& message) = 0;

  /**
   * @brief вызывается, когда произошла смена активного диалога
   * @param newActiveDialogId - id нового диалога
   */
  virtual void activeDialogChanged(const std::string& newActiveDialogId) = 0;

  /**
   * @brief Вызывается, для того, чтобы передать обработчику все сообщения по
   * одному для нового активного диалога
   * @param message сообщение
   */
  virtual void peekMessage(
      const std::shared_ptr<const UserMessage>& message) = 0;

  /**
   * @brief Вызывается для того, чтобы класс перечитал все сообщения для
   * указанного диалога
   * @param dialogId - диалог, для которого изменились сообщения
   */
  virtual void invalidateData(const std::string& dialogId) = 0;
};
#endif  // MESSAGECONTEINEREVENTHANDLER_H
