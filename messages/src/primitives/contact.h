#ifndef CONTACT_H
#define CONTACT_H

#include <string>

/**
 * @brief Контакт пользователя
 * Класс для хранения данных о конкретном адресате сообщений.
 * Нужен для хранения данных об адресах
 */

class Contact {
 public:
  /**
   * @brief Конструктор класса. В нем идентификатор пользователя задается
   * автоматически.
   * @param channelMoniker код канала передачи данных, который будет
   * использоваться, когда пользователь будет отсылать этому контакту сообщения
   * @param name - имя контакта
   * @param adress - адрес, на который будут отправлены сообщения
   * @param key - публичный ключ собеседника
   */
  Contact(const std::string& channelMoniker,
          const std::string& name,
          const std::string& adress,
          const std::string& key);

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
  Contact(const std::string& channelMoniker,
          const std::string& name,
          const std::string& adress,
          const std::string& key,
          const std::string& id);

 public:
  std::string name() const;
  std::string channelMoniker() const;
  std::string adress() const;
  std::string id() const;
  std::string publicKey() const;

 public:
  void name(const std::string& name);
  void channelMoniker(const std::string& moniker);
  void adress(const std::string& adress);
  void publicKey(const std::string& key);

 private:
  std::string mChannelMoniker;
  std::string mName;
  std::string mAdress;
  std::string mContactId;
  std::string mPublicKey;
};

#endif  // CONTACT_H
