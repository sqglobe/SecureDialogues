#ifndef USERMESSAGE_H
#define USERMESSAGE_H

#include <string>

/**
 * @brief Сообщение пользователя или для пользователя
 */
class UserMessage {
 public:
  /**
   * @brief Статус сообщения:
   * WAIT_DELIVERY - ожидает доставки
   * DELIVERIED - доставлено
   * NOT_DELIVERIED - не доставлено
   * ERROR - сообщение об ошибке
   */
  enum class Status { WAIT_DELIVERY, DELIVERIED, NOT_DELIVERIED, ERROR };

  /**
   * @brief Тип сообщения, описывает источник поступления
   * IN - входящее
   * OUT - исходящее
   * SYSTEM - сообщение от системы
   */
  enum class Type { IN, OUT, SYSTEM };

 public:
  /**
   * @brief Конструктор класса
   * @param status статус сообщения
   * @param type тип сообщения
   * @param content текст сообщения
   */
  UserMessage(Status status, Type type, std::string content);

  /**
   * @brief Конструктор. Статус имеет значение - WAIT_DELIVERY. Используется
   * чаще всего для исходящих сообщений
   * @param type - тип сообщения
   * @param content - текст сообщения
   */
  UserMessage(Type type, std::string content);

 public:
  /**
   * @brief Задает новый статус сообщения
   * @param status новый статус
   */
  void updateStatus(Status status) noexcept;

 public:
  /**
   * @brief Возвращает текущий статус сообщения
   * @return текущий статус
   */
  Status status() const noexcept;

  /**
   * @brief Возвращает тип сообщения
   * @return тип сообщения
   */
  Type type() const noexcept;

  /**
   * @brief Возвращает текст сообщения
   * @return текст сообщения
   */
  std::string content() const;

 private:
  Status mStatus;
  Type mType;
  std::string mContent;
};

#endif  // USERMESSAGE_H
