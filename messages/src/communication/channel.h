#ifndef CHANNEL_H
#define CHANNEL_H
#include <atomic>
#include <future>
#include <memory>
#include <mutex>

#include "eventpp/eventqueue.h"

class DialogMessage;
class AbstractChannelAdapter;
class AbstractMessageDespatcher;
class AbstractMessageMarshaller;

namespace spdlog {
class logger;
}

/**
 * Канал передачи сообщений.
 * Класс обеспечивает декодирование поступающих сообщений от
 * AbstractChannelAdapter в специальный формат DialogMessage и передает их для
 * дальнейшей обработки AbstractMessageDespatcher, а так же пересылку сообщений
 * DialogMessage в класс AbstractChannelAdapter для последующей отправки.
 * Используется как небольшая надстройка, которая обеспечивает цикл получения
 * сообщений, их декодирование и передачу AbstractMessageDespatcher
 *
 */
class Channel : public std::enable_shared_from_this<Channel> {
 public:
  /**
   * @brief Статус подключения канала
   */
  enum class ChannelStatus {
    UNDEFINED,
    CONNECTED,
    FAILED_CONNECT,
    AUTHORIZATION_FAILED
  };

  using EventQueue = eventpp::EventQueue<
      Channel::ChannelStatus,
      void(Channel::ChannelStatus, const std::string&, const std::string&)>;

 public:
  /**
   * @brief Конструктор класса
   * @param adater - адаптер для взаимодействия со средой передачи сообщений,
   * обеспечивает методы для отправки и получения сообщений
   * @param handler - класс-обработчик поступивших сообщений
   * @param name - идентификатор текущего канала
   */
  Channel(std::unique_ptr<AbstractChannelAdapter>&& adater,
          const std::shared_ptr<AbstractMessageDespatcher>& handler,
          std::shared_ptr<AbstractMessageMarshaller> marshaller,
          const std::string& name,
          const std::shared_ptr<EventQueue>& eventQueue,
          std::chrono::seconds waitAck);

  /**
   * @brief Конструктор класса
   * @param adater - адаптер для взаимодействия со средой передачи сообщений,
   * обеспечивает методы для отправки и получения сообщений
   * @param marshaller - объект, который выполняет сериализацию и десериализацию
   * сообщений.
   * @param handler - класс-обработчик поступивших сообщений
   * @param name - идентификатор текущего канала
   */
  Channel(AbstractChannelAdapter* adapter,
          const std::shared_ptr<AbstractMessageDespatcher>& handler,
          std::shared_ptr<AbstractMessageMarshaller> marshaller,
          const std::string& name,
          const std::shared_ptr<EventQueue>& eventQueue,
          std::chrono::seconds waitAck);
  ~Channel();

 public:
  Channel(const Channel&) = delete;
  Channel& operator=(const Channel&) = delete;

 public:
  /**
   * Метод, в котором происходит прием сообщений по сети. Выполняется в
   * отдельном потоке, так как в ожидании сообщений блокирует свой поток
   * @param shared указатель на класс Channel, для которого запускается поток
   * приема сообщений
   */
  void messsageCycle();

  /**
   * Выполняет отправку сообщения по сети через AbstractChannelAdapter
   * @param message -  исходящее сообщений
   */
  void sendMessage(const DialogMessage& message);

  /**
   * Запускает цикл приема входящих сообщений.
   * В методе создается отдельный поток
   */
  void startCycle();

  std::chrono::seconds getWaitAckInterval() const;

 private:
  std::unique_ptr<AbstractChannelAdapter> mAdapter;
  std::weak_ptr<AbstractMessageDespatcher> mHandler;
  std::atomic_bool mIsEnds{false};
  std::shared_ptr<AbstractMessageMarshaller> mMarshaller;
  std::string mName;
  std::shared_ptr<EventQueue> mEventQueue;
  std::thread mThread;
  std::mutex mMutex;
  std::chrono::seconds mWaitAckInterval;
};

#endif  // CHANNEL_H
