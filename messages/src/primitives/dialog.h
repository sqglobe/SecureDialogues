#ifndef DIALOG_H
#define DIALOG_H

#include <exception>
#include <memory>
#include "dialogmessage.h"

class Contact;

class invalid_dialog_action : public std::runtime_error {
 public:
  invalid_dialog_action(const std::string& str);
};

/**
 * @brief Класс диалога
 * Сущность диалога подобна соединению. Перед тем, как начать отправлять
 * сообщения пользователи должны создать между собой диалог. На этапе настройки
 * возможны согласования некоторых стартовых параметров передачи сообщений. Для
 * создания диалога один пользователь должен отправить запрос на создание
 * диалога удаленному контакту, который может либо подтвердить создание диалога,
 * либо отклонить.
 *
 * Класс обеспечивает создание сообщений. Причем производится проверка текущего
 * состояния диалога, и типа сообщения, создание которого запрошено. Если диалог
 * находится в неподходящем состоянии, то будет выброшено исключение.
 */

class Dialog {
 public:
  // Текущий статус диалога
  enum class Status {
    NEW = 0,
    CREATE_REQUEST,
    WAIT_CONFIRM,
    ACCEPTED_WAIT,
    CANCEL_WAIT,
    WAIT_KEY_VERIFICAION,
    ACTIVE,
    CLOSED,
    CANCELED,
    ABORTED
  };

 public:
  /**
   * @brief Конструктор диалога, вызывается при условии, что мы восстанавливаем
   * ранее сохраненный диалог
   * @param contact - контакт, с которым создан диалог
   * @param dialogId - id диалога, должно быть уникальным
   * @param status - текущий статус диалога
   */
  Dialog(std::shared_ptr<const Contact> contact,
         std::string dialogId,
         Status status = Status::NEW);

  /**
   * @brief Конструктор класса
   * @param contact контакт, для которого создается диалог
   * @param dialogId идентификатор диалога
   * @param sequental порядковый номер инициирующего диалог сообщения
   * @param status статус диалога
   */
  Dialog(std::shared_ptr<const Contact> contact,
         std::string dialogId,
         unsigned long sequental,
         Status status = Status::NEW);

  /**
   * @brief Конструктор, создает новый диалог в статусе Status::NEW, а так же
   * генерирует id диалога
   * @param contact - контакт, с которым создается диалог
   */
  Dialog(std::shared_ptr<const Contact> contact);

 public:
  std::string getDialogId() const;
  std::string getChannelMoniker() const;

  /**
   * @brief Создает сообщение с указанным содержимым и указанным типом
   * @param action - тип сообщения
   * @param content - содержимое сообщения
   * @return сообщение для передачи
   * @throws выбрасывает исключение, если action не соответствует состоянию
   * диалога
   */
  DialogMessage makeMessage(DialogMessage::Action action,
                            const std::string& content) const noexcept(false);

  /**
   * @brief Создает сообщение для аварийного завершения диалога
   * @return сообщение с типом abort
   */
  DialogMessage makeAbort();

  /**
   * @brief Возвращает текущий статус диалога
   * @return статус диалога
   */
  Status getStatus() const noexcept;

  /**
   * @brief Переводит диалог в новое состояние
   * @param status - новое состояние диалога
   */
  void setStatus(Status status) noexcept(false);
  std::string getContactId() const;
  std::string getAdress() const;
  std::string getName() const;

  /**
   * @brief Выполняет проверку и обновление последнего порядкового номера
   * сообщения. Если указанный в параметре порядковый номер меньше или равен
   * номеру, сохраненному в диалоге, то такое сообщение должно быть отброшено. В
   * противном случае сообщение проходит, а последний номер принятого сообщения
   * обновляется.
   * @param sequental порядковый номер сообщения
   * @return true, если сообщение с указанным порядковым номером должно быть
   * принято, в противном случае - false
   */
  bool isSequentalOk(unsigned long sequental);

  /**
   * @brief Выполняет проверку, является ли допустимым создать сообщение с
   * указанным типом для текущего диалога
   * @param action тип сообщения, которое предполагается создать
   * @return true, если текущее состояние диалога разрешает создавать сообщения
   * с указанным типом, в противном случае -  false.
   */
  bool isMessageActionAllowed(DialogMessage::Action action) const noexcept;

 private:
  unsigned long getNextSequental() const;

 private:
  std::shared_ptr<const Contact> mContact;
  std::string mDialogId;
  Status mStatus;
  unsigned long mLastSequental;
  mutable unsigned long mThisSequental;
};

#endif  // DIALOG_H
