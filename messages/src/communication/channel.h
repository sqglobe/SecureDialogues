#ifndef CHANNEL_H
#define CHANNEL_H
#include <atomic>
#include <future>
#include <memory>
#include <mutex>

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
   * @brief Конструктор класса
   * @param adater - адаптер для взаимодействия со средой передачи сообщений,
   * обеспечивает методы для отправки и получения сообщений
   * @param handler - класс-обработчик поступивших сообщений
   * @param name - идентификатор текущего канала
   */
  Channel(std::unique_ptr<AbstractChannelAdapter>&& adater,
          const std::shared_ptr<AbstractMessageDespatcher>& handler,
          const std::shared_ptr<AbstractMessageMarshaller>& marshaller,
          const std::string& name);

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
          const std::shared_ptr<AbstractMessageMarshaller>& marshaller,
          const std::string& name);
  ~Channel();

 public:
  /**
   * Метод, в котором происходит прием сообщений по сети. Выполняется в
   * отдельном потоке, так как в ожидании сообщений блокирует свой поток
   * @param shared указатель на класс Channel, для которого запускается поток
   * приема сообщений
   */
  void messsageCycle(std::weak_ptr<Channel>&& shared);

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

 private:
  void checkConnectionProcess(const std::weak_ptr<Channel>& channel);

 private:
  std::unique_ptr<AbstractChannelAdapter> mAdapter;
  std::weak_ptr<AbstractMessageDespatcher> mHandler;
  std::atomic_bool mIsEnds{false};
  std::shared_ptr<AbstractMessageMarshaller> mMarshaller;
  std::string mName;
  static std::shared_ptr<spdlog::logger> LOGGER;
};

#endif  // CHANNEL_H
