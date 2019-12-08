#ifndef MESSAGEDESPATCHER_H
#define MESSAGEDESPATCHER_H
#include <atomic>
#include <map>
#include <shared_mutex>
#include <string>
#include <vector>
#include "interfaces/abstractmessagedespatcher.h"

class AbstractMessageHandler;
class Channel;
class DialogMessage;
class AbstractChannelAdapter;
class CryptoSystem;
class AbstractUserNotifier;
template <typename T, typename K>
class TimeoutedRrepository;
class AbstractDespatchErrorsSink;

namespace spdlog {
class logger;
}

/**
 * @brief Менеджер каналов для передачи сообщений
 *
 * Обеспечивает добавление, удаление каналов передачи сообщений.
 * А так же отправление сообщений по каналу, адресуемому по его идентификатору.
 * И маршрутизацию полученных по сети сообщений к нужному обработчику.
 */

class MessageDespatcher : public AbstractMessageDespatcher {
 public:
  MessageDespatcher(std::shared_ptr<const CryptoSystem> cryptoSystem,
                    std::shared_ptr<AbstractUserNotifier> notifier,
                    std::shared_ptr<AbstractDespatchErrorsSink> errorSink);

 public:
  /**
   * @brief Обработка сообщений, полученных по сети.
   * Выбирает обработчики сообщений AbstractMessageHandler, которые подходят под
   * тип, полученного сообщения и поручает им дополнительную обработку
   * @param message класс полученного сообщения
   * @param mChannelName идентификатор канала, для которого получено сообщение
   */
  void dispatch(DialogMessage&& message,
                const std::string& channelName) noexcept override;

  /**
   * @brief Отправляет сообщение по сети, через канал передачи, указанный в
   * параметрах
   * @param message  класс сообщения, которое необходимо отправить
   * @param channelName имя канала, по которому необходимо отправить сообщение
   */
  void sendMessage(
      DialogMessage&& message,
      std::string_view channelName,
      std::shared_ptr<DeliveryHandler>&& deliveryHandler) const override;

  void sendAndForget(DialogMessage&& message,
                     std::string_view channelName) const override;

 public:
  /**
   * @brief Добавляет новый обработчик входящих сообщений
   * @param handler - обработчик сообщений
   */
  void add(const std::shared_ptr<AbstractMessageHandler>& handler);

  /**
   * @brief Добавляет новый обработчик входящих сообщений
   * @param handler - обработчик сообщений
   */
  void add(AbstractMessageHandler* handler);

  /**
   * @brief Добавлет новый канал для передачи сообщений
   * @param channel указатель на канал
   * @param channelName имя канала, используется для его адресации
   */
  void add(std::unique_ptr<Channel>&& channel, const std::string& channelName);
  /**
   * @brief Добавлет новый канал для передачи сообщений
   * @param channel указатель на канал
   * @param channelName имя канала, используется для его адресации
   */
  void add(const std::shared_ptr<Channel>& channel,
           const std::string& channelName);

  /**
   * @brief Добавлет новый канал для передачи сообщений
   * @param channel указатель на канал
   * @param channelName имя канала, используется для его адресации
   */
  void add(Channel* channel, const std::string& channelName);

  /**
   * @brief Удаляет канал передачи сообщений, адресуемый идентификатором channel
   * @param channel идентификатор удаляемого канала
   */
  void removeChannel(const std::string& channel);

 public:
  /**
   * @brief Возвращает список всех каналов передачи сообщений, которые
   * зарегистрированы в системе
   * @return список идентификаторов каналов передачи сообщений
   */
  std::vector<std::string> getChannelsNames() const;

 private:
  void sendAck(const DialogMessage& message, const std::string& channel);
  bool isSignatureValid(const DialogMessage& message,
                        const std::string& channelName) const noexcept;

 private:
  std::vector<std::shared_ptr<AbstractMessageHandler>> mHandlers;
  std::map<std::string, std::shared_ptr<Channel>, std::less<>> mChannels;
  mutable std::shared_mutex mMutex;
  std::shared_ptr<const CryptoSystem> mCryptoSystem;
  std::shared_ptr<AbstractUserNotifier> mNotifier;
  std::shared_ptr<AbstractDespatchErrorsSink> mErrorSink;

  mutable std::shared_ptr<
      TimeoutedRrepository<std::shared_ptr<DeliveryHandler>,
                           std::pair<std::string, unsigned long>>>
      mRepo;
};

#endif  // MESSAGEDESPATCHER_H
