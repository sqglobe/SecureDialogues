#ifndef DIALOGACTIONHANDLER_H
#define DIALOGACTIONHANDLER_H

#include "interfaces/abstractmessagehandler.h"

#include <memory>
#include "primitives/dialog.h"
#include "primitives/dialogmessage.h"

class DialogManager;
class AbstractMessageDespatcher;
class AbstractDialogEventHandler;
class AbstractUserNotifier;
class Contact;
class ContactContainer;
class DeliveryHandler;
class CryptoSystem;

namespace spdlog {
class logger;
}

/**
 * @brief Обработчик событий диалогов
 * Обрабатывает служебные сообщения, которые связаны с управлением диалогами,
 * вроде создания, удаления, изменения. Так же позволяет выполнить действия с
 * диалогами, запрошенные локальным пользователем
 */
class DialogActionHandler : public AbstractMessageHandler {
 public:
  /**
   * @brief Конструктор класса
   * @param dialogManager - контейнер с диалогами, нужен для получения
   * конкретного диалога по id
   * @param dispatcher - класс для отправки управляющих диалогами сообщений
   * удаленной стороне
   * @param eventHandler - класс обработчик сообщений об изменении состояния
   * диалогов
   * @param contacts - контейнер контактов
   */
  DialogActionHandler(std::shared_ptr<DialogManager> dialogManager,
                      std::shared_ptr<AbstractMessageDespatcher> dispatcher,
                      std::shared_ptr<AbstractUserNotifier> notifier,
                      std::shared_ptr<ContactContainer> contacts,
                      std::shared_ptr<CryptoSystem> cryptoSystem);

 public:
  /**
   * @brief Обработка поступивших управляющих состоянием диалогов сообщений.
   * В этом методе изменяется статус диалога, id которого указан в сообщении,
   * а так же уведомляется AbstractDialogEventHandler
   * @param message - управляющее сообщение
   * @param channel - мнемокод канала, по которому пришло сообщение
   */
  void handle(const DialogMessage& message,
              const std::string& channel) noexcept override;

  /**
   * @brief Определяет обрабатывается ли указанный тип сообщение данным классом
   * @param action - тип сообщения
   * @return возвращает true, если указанный тип обрабатывается, иначе - false
   */
  bool isItYouAction(DialogMessage::Action action) const noexcept override;

 public:
  /**
   * @brief Удаляет диалог, имеющий указанный моникер
   * Если диалог находится в активном состоянии, то перед удалением отсылается
   * сообщение на закрытие диалога. В противном случае отсылается сообщение -
   * ABORT.
   * @param dialogId - моникер диалога, который необходимо удалить
   */
  void removeDialog(const std::string& dialogId);

  /**
   * @brief Создает диалог с указанным контактом
   * Так же отправляется сообщение CREATE_DIALOG указанному контакту.
   * @param contact - контакт, с которым необходимо создать диалог.
   */
  void createDialog(const std::shared_ptr<const Contact>& contact);

  /**
   * @brief Уведомляет удаленную сторону о закрытии диалога сообщением ABORT.
   * Диалог, с указанным кодом переводится в соответствующее состояние. Этот
   * метод вызывается, когда произошла ошибка в обработке диалога.
   * @param dialogId - моникер диалога для которого производится информирование.
   */
  void abortDialog(const std::string& dialogId);

  /**
   * @brief Уведомляет удаленную сторону о закрытии диалога
   * @param dialogId - id диалога на закрытие
   */
  void closeDialog(const std::string& dialogId);

 private:
  void sendRequest(const std::string& dialogId,
                   DialogMessage::Action action,
                   const std::shared_ptr<DeliveryHandler>& handler,
                   const std::string& content = "");
  void prepareNotFoundDialog(const DialogMessage& message,
                             const std::string& channel);
  void prepareForFoundDialog(const DialogMessage& message,
                             const std::string& channel);

  Dialog::Status map(DialogMessage::Action action) const noexcept;
  bool isRemoveDialogAction(DialogMessage::Action action) const noexcept;
  void prepareHandleException(const DialogMessage& message,
                              const std::string& channel,
                              const std::exception& ex) noexcept;

 private:
  std::shared_ptr<DialogManager> mDialogManager;
  std::weak_ptr<AbstractMessageDespatcher> mMessageDispatcher;
  std::shared_ptr<AbstractUserNotifier> mNotifier;
  std::shared_ptr<ContactContainer> mContactContainer;
  std::shared_ptr<CryptoSystem> mCryptoSystem;
};

#endif  // DIALOGACTIONHANDLER_H
