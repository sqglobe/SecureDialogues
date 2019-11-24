#ifndef CONTACT_H
#define CONTACT_H

#include <string>
#include <string_view>
#include "utils.h"
/**
 * @brief Контакт пользователя
 * Класс для хранения данных о конкретном адресате сообщений.
 * Нужен для хранения данных об адресах
 */

class Contact {
 public:
  Contact() = default;

  /**
   * @brief Конструктор класса. В нем идентификатор пользователя задается
   * автоматически.
   * @param channelMoniker код канала передачи данных, который будет
   * использоваться, когда пользователь будет отсылать этому контакту сообщения
   * @param name - имя контакта
   * @param adress - адрес, на который будут отправлены сообщения
   * @param key - публичный ключ собеседника
   */
  template <typename S,
            class = typename std::enable_if<
                std::is_constructible<std::string, S>::value>::type>
  Contact(S&& channelMoniker, S&& name, S&& adress, S&& key);

  /**
   * @brief Конструктор класса
   * @param channelMoniker код канала передачи данных, который будет
   * использоваться, когда пользователь будет отсылать этому контакту сообщения
   * @param name - имя контакта
   * @param adress - адрес, на который будут отправлены сообщения
   * @param key - публичный ключ собеседника
   * @param id - уникальный идентификатор пользователя, служит для связи с
   * другими классами
   */
  template <typename S,
            class = typename std::enable_if<
                std::is_constructible<std::string, S>::value>::type>
  Contact(S&& channelMoniker, S&& name, S&& adress, S&& key, S&& id);

 public:
  std::string_view name() const noexcept;
  std::string_view channelMoniker() const noexcept;
  std::string_view adress() const noexcept;
  std::string_view id() const noexcept;
  std::string_view publicKey() const noexcept;

 public:
  void name(std::string name);
  void channelMoniker(std::string moniker);
  void adress(std::string adress);
  void publicKey(std::string key);

 private:
  std::string mChannelMoniker;
  std::string mName;
  std::string mAdress;
  std::string mContactId;
  std::string mPublicKey;
};

template <typename S, class>
Contact::Contact(S&& channelMoniker, S&& name, S&& adress, S&& key) :
    Contact(std::forward<std::string>(channelMoniker),
            std::forward<std::string>(name),
            std::forward<std::string>(adress),
            std::forward<std::string>(key),
            make_uiid()) {}

template <typename S, class>
Contact::Contact(S&& channelMoniker, S&& name, S&& adress, S&& key, S&& id) :
    mChannelMoniker(std::forward<std::string>(channelMoniker)),
    mName(std::forward<std::string>(name)),
    mAdress(std::forward<std::string>(adress)),
    mContactId(std::forward<std::string>(id)),
    mPublicKey(std::forward<std::string>(key)) {
  static_assert(std::is_constructible<std::string, S>::value,
                "Type parameter for Contact must be a string");
}

#endif  // CONTACT_H
