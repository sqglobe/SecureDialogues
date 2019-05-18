#ifndef ABSTRACTCHANNELADAPTER_H
#define ABSTRACTCHANNELADAPTER_H

#include <memory>
#include <string>
#include <utility>
#include "abstractusernotifier.h"

class SendMessageException : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};

class ConnectionHolder;
class AbstractChannelAdapter;

/**
 * @brief Класс-драйвер для конкретного подключения по сети
 * Обеспечивает общую функциональность для создания подключения,
 * отправки и получения через него сообщений, а так же
 * тестирования соединения
 */
class AbstractChannelAdapter {
 public:
  /**
   * @brief Конструктор класса
   * @param notifier класс, который может использоваться для уведомления
   * пользователя о происшедших проблемах
   */
  explicit AbstractChannelAdapter(
      const std::shared_ptr<AbstractUserNotifier>& notifier);
  virtual ~AbstractChannelAdapter() = default;

 public:
  /**
   * @brief Отправляет сообщение через сеть по указанному адресу
   * @param message текст сообщения, которое необходимо отправить
   * @param adress адрес, которому предназначено сообщение
   */
  virtual void send(const std::string& message,
                    const std::string& adress) noexcept(false) = 0;

  /**
   * @brief Метод используется для получения сообщения.
   * Предполагается, что в конкретной реализации метод будет блокировать
   * поток выполнения пока не получит сообщение, либо на время некоторого
   * таймаута, по истечении которого будет возвращено полученное сообщение, либо
   * пустое сообщение. Так же предполагается, что, если адаптер получает сразу
   * несколько сообщений, то он сохраняет их все в памяти, а потом по одному
   * извлекает их передает их по одному в результате этого метода
   * @return возвращает пару <адрес, текст сообщений>. Где адрес - от кого было
   * получено сообщение, "текст сообщения" - текст полученного сообщения
   */
  virtual std::pair<std::string, std::string> receive() noexcept(false) = 0;

  /**
   * @brief Выполняет подключение канала связи и возвращает успешность этого.
   * Этот метод должен вызываться перед выполнение отправки/получения сообщений
   * @param conn параметры подключения
   * @return true - если подключение прошло успешно, false в противном случае
   */
  virtual bool connect(const ConnectionHolder& conn) = 0;
  virtual bool isConnected() const noexcept = 0;

 protected:
  /**
   * @brief Выполняет информирование пользователя о произшедших проблемах
   * @param sev тип сообщения
   * @param message текст сообщения
   */
  void notify(AbstractUserNotifier::Severity sev, const std::string& message);

 private:
  std::shared_ptr<AbstractUserNotifier> mNotifier;
};

inline bool AbstractChannelAdapter::isConnected() const noexcept {
  return true;
}

#endif  // ABSTRACTCHANNELADAPTER_H
