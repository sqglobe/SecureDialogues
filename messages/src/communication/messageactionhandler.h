#ifndef MESSAGEACTIONHANDLER_H
#define MESSAGEACTIONHANDLER_H

#include <memory>
#include "interfaces/abstractmessagehandler.h"
namespace spdlog {
class logger;
}

class AbstractUserNotifier;
class DialogManager;
class MessageContainer;
class MessageDespatcher;
class CryptoSystem;

/**
 * @brief Класс, который занимается работой с текстовыми сообщениями
 * Класс обрабатывает поступающие тестовые сообщения, а так же позволяет
 * отправить сообщение
 *
 * В своей работе класс использует контейнер для хранения сообщений -
 * MessageContainer. В этот контейнер добавляются все сообщения, которые
 * получает система по сети, и которые пользователь отправляет.
 *
 * Для того, чтобы отправить сообщение, необходимо сперва указать текущий
 * активный диалог, вызвав метод setActiveDialog, а уж только потом выполнять
 * отправку сообщений.
 *
 * Это сделано потому, что пользователь сначала выбирает диалог, по которому
 * будет отправлять сообщения. После этого начинает отправлять сообщения.
 *
 */

class MessageActionHandler : public AbstractMessageHandler {
 public:
  /**
   * @brief Конструктор класса
   * @param manager контейнер диалогов, к в котором производится поиск активного
   * диалога для создания сообщения
   * @param container контейнер сообщений, в который будут помещены сообщения
   * @param despatcher диспетчер сообщений, используется для того, чтобы
   * отправить сообщение
   * @param notifier класс, который используется для того, чтобы
   * проинформировать пользователя о каком-либо событии
   */
  MessageActionHandler(std::shared_ptr<DialogManager> manager,
                       std::shared_ptr<MessageContainer> container,
                       std::shared_ptr<const MessageDespatcher> despatcher,
                       std::shared_ptr<AbstractUserNotifier> notifier,
                       std::shared_ptr<const CryptoSystem> system);

 public:
  void handle(const DialogMessage& message,
              const std::string& channel) noexcept override;
  bool isItYouAction(DialogMessage::Action action) const noexcept override;

 public:
  /**
   * @brief Отправляет сообщение, которое принадлежит ранее заданному активному
   * диалогу
   * @param message текст сообщения
   */
  void sendMessage(const std::string& message);

  /**
   * @brief Задает активный диалог, к которому будет относиться все отправляемые
   * сообщения
   * @param dialogId id диалога
   */
  void setActiveDialog(const std::string& dialogId);

 private:
  void abortOnException(const DialogMessage& message,
                        const std::string& channel) noexcept;

 private:
  std::shared_ptr<DialogManager> mDialogManager;
  std::shared_ptr<MessageContainer> mMessageContainer;
  std::weak_ptr<const MessageDespatcher> mDespatcher;
  std::shared_ptr<AbstractUserNotifier> mNotifier;
  std::shared_ptr<const CryptoSystem> mCryptoSystem;
};

#endif  // MESSAGEACTIONHANDLER_H
