#ifndef DIALOGMESSAGE_H
#define DIALOGMESSAGE_H

#include <stdexcept>
#include <string>

/**
 * @brief Класс представляет из себя внутреннее для системы представление
 * сообщения, которое подлежит передачи, либо уже принято Сообщение используется
 * не только для передачи текстовых сообщения, но и для переноса управляющих
 * команд, таких как создание нового диалога. отклонение диалога и пр.
 *
 * Сообщение может создаваться двумя различными путями:
 *  1. Непосредственным заданием всех полей через параметры конструктора.
 * Используется для отправляемых сообщений
 *  2. Разбором тестового представления сообщения в поля класса. Используется
 * при получении сообщения по сети. При разборе сообщения могут возникать
 * ошибки, поэтому необходимо проверять статус сообщения через автоматическое
 * приведение к bool или через isOk
 */

class DialogMessage {
 public:
  /**
   * @brief Тип сообщения
   * Показывает что содержится в письме:
   * MESSAGE - текстовое сообщение
   * CREATE_DIALOG - запрос на создание диалога
   * ACCEPT_DIALOG - сообщение, информирующее о том, что удаленная сторона
   * приняла созданный диалог и можно начать передачу тестовых сообщений (ответ
   * на CREATE_DIALOG) CLOSE_DIALOG - отправляется, когда пользователь закрывает
   * ранее созданный диалог. CANCEL_DIALOG - отмена создания нового диалога
   * (ответ на CREATE_DIALOG) ACK - подтверждает ранее полученное сообщение
   * Каждое сообщение отправляется в рамках диалога.
   */
  enum class Action {
    MESSAGE,
    CREATE_DIALOG,
    ACCEPT_DIALOG,
    CLOSE_DIALOG,
    CANCEL_DIALOG,
    ACK,
    ABORT,
    VERIFY_KEY,
    KEY_VERIFICATION
  };
  // constractors
 public:
  /**
   * @brief Простой конструктор класса
   * @param action тип сообщения
   * @param content текст сообщения
   * @param dialogId id диалога, с которым связано сообщение
   * @param adress - адрес, на который необходимо отправить сообщение
   * @param number порядковый номер сообщения в системе исчисления отправителя
   */
  DialogMessage(const Action& action,
                std::string content,
                std::string dialogId,
                std::string adress = "",
                unsigned long number = 0,
                std::string sign = "");

  /**
   * @brief Простой конструктор класса
   * @param action тип сообщения
   * @param content текст сообщения
   * @param dialogId id диалога, с которым связано сообщение
   * @param adress - адрес, на который необходимо отправить сообщение
   * @param number порядковый номер сообщения в системе исчисления отправителя
   */
  DialogMessage(const Action& action,
                std::string content,
                std::string dialogId,
                std::string adress,
                unsigned long number,
                std::string sign,
                long timestamp);

 public:
  Action action() const noexcept;
  std::string content() const noexcept;
  std::string dialogId() const noexcept;
  std::string adress() const noexcept;
  unsigned long sequential() const noexcept;
  std::string sign() const;
  long timestamp() const;

 public:
  void setSequentialNumber(unsigned long number) noexcept;
  void setSignature(const std::string& signature);
  void setContent(const std::string& content);

 private:
  Action mAction;
  std::string mContent;
  std::string mDialogId;
  std::string mAdress;  // adress recive from, or send to
  std::string mSign;
  unsigned long mSequentialNumber;
  long mTimestamp;

 public:
  static constexpr const char* const VERSION = "1.0";
};

/**
 * @brief Создает сообщение с типом ABORT
 * @param dialogId - диалог, для которого создается сообщение
 * @param contact - кому отправить сообщение
 * @return новое сообщение с типом abort для указанного диалога.
 */
DialogMessage make_abort(const std::string& dialogId,
                         const std::string& contact);

/**
 * @brief Создает подтверждение о доставке для указанного сообщения
 * @param mess - сообщение, для которого необходимо создать подтверждение о
 * доставке
 * @return подтверждение о доставке
 */
DialogMessage make_ack(const DialogMessage& mess);

#endif  // DIALOGMESSAGE_H
