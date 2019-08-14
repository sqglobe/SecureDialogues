#ifndef DIALOG_H
#define DIALOG_H

#include <exception>
#include <memory>
#include <string_view>
#include <type_traits>
#include "dialogmessage.h"
#include "utils.h"

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
    WAIT_KEY_VERIFICAION,
    ACTIVE,
    CLOSED,
    ABORTED
  };

 public:
  Dialog() = default;
  /**
   * @brief Конструктор диалога, вызывается при условии, что мы восстанавливаем
   * ранее сохраненный диалог
   * @param contact - контакт, с которым создан диалог
   * @param dialogId - id диалога, должно быть уникальным
   * @param status - текущий статус диалога
   */
  template <typename S,
            class = typename std::enable_if<
                std::is_constructible<std::string, S>::value>::type>
  explicit Dialog(S&& contactId);

  /**
   * @brief Конструктор класса
   * @param contact контакт, для которого создается диалог
   * @param dialogId идентификатор диалога
   * @param sequental порядковый номер инициирующего диалог сообщения
   * @param status статус диалога
   */
  template <typename S,
            class = typename std::enable_if<
                std::is_constructible<std::string, S>::value>::type>
  Dialog(S&& contactId,
         S&& dialogId,
         unsigned long peerSequental,
         unsigned long thisSequental,
         Status status);

  /**
   * @brief Конструктор, создает новый диалог в статусе Status::NEW, а так же
   * генерирует id диалога
   * @param contact - контакт, с которым создается диалог
   */
  template <typename S,
            class = typename std::enable_if<
                std::is_constructible<std::string, S>::value>::type>
  Dialog(S&& contactId, S&& dialogId, unsigned long sequental, Status status);

 public:
  std::string_view getDialogId() const noexcept;

  /**
   * @brief Возвращает текущий статус диалога
   * @return статус диалога
   */
  Status getStatus() const noexcept;
  std::string_view getContactId() const noexcept;

  unsigned long getPeerSequental() const noexcept;
  unsigned long getThisSequental() const noexcept;
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
   * @brief Переводит диалог в новое состояние
   * @param status - новое состояние диалога
   */
  void setStatus(Status status) noexcept(false);

  /**
   * @brief Выполняет проверку, является ли допустимым создать сообщение с
   * указанным типом для текущего диалога
   * @param action тип сообщения, которое предполагается создать
   * @return true, если текущее состояние диалога разрешает создавать сообщения
   * с указанным типом, в противном случае -  false.
   */
  bool isMessageActionAllowed(DialogMessage::Action action) const noexcept;
  unsigned long makeNextSequental();

 private:
  std::string mDialogId;
  std::string mContactId;
  Status mStatus;
  unsigned long mPeerSequental;
  unsigned long mThisSequental;
};

unsigned long init_sequental();

template <typename S, class>
Dialog::Dialog(S&& contactId,
               S&& dialogId,
               unsigned long sequental,
               Dialog::Status status) :
    Dialog(std::forward<std::string>(contactId),
           std::forward<std::string>(dialogId),
           sequental,
           init_sequental(),
           status) {}

template <typename S, class>
Dialog::Dialog(S&& contactId) :
    Dialog(std::forward<std::string>(contactId),
           make_uiid(),
           0,
           init_sequental(),
           Dialog::Status::NEW) {}

template <typename S, class>
Dialog::Dialog(S&& contactId,
               S&& dialogId,
               unsigned long peerSequental,
               unsigned long thisSequental,
               Dialog::Status status) :
    mDialogId(std::forward<std::string>(dialogId)),
    mContactId(std::forward<std::string>(contactId)), mStatus(status),
    mPeerSequental(peerSequental), mThisSequental(thisSequental) {
  static_assert(std::is_constructible<std::string, S>::value,
                "Type parameter for Dialog must be a string");
}

#endif  // DIALOG_H
