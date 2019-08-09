#ifndef ABSTRACTMESSAGEDESPATCHER_H
#define ABSTRACTMESSAGEDESPATCHER_H
#include <memory>
#include <string>
class DialogMessage;
class DeliveryHandler;
/**
 * @brief Класс-маршрутизатор сообщений
 * Обеспечивает функциональность обработке сообщений, полученных сообщений (в
 * реализации - передача обработчикам), и отправке сообщений.
 */
class AbstractMessageDespatcher {
 public:
  virtual ~AbstractMessageDespatcher() = default;

  /**
   * @brief Обработка входящего сообщения.
   * Вызывается, когда поступает новое сообщение
   * @param message поступившее сообщение
   */
  virtual void dispatch(DialogMessage&& message,
                        const std::string& channelName) noexcept = 0;

  /**
   * @brief Отправляет сообщение по через канал channelName. Отслеживает
   * подтверждение о получении.
   * @param message  сообщение, которое подлежит отправке
   * @param channelName канал, по которому необходимо отправить сообщение
   * @param deliveryHandler обработчик подтверждения о доставке, либо о том, что
   * подтверждение не доставлено
   */
  virtual void sendMessage(
      DialogMessage&& message,
      std::string_view channelName,
      std::shared_ptr<DeliveryHandler>&& deliveryHandler) const = 0;

  /**
   * @brief Отправляет сообщение по через канал channelName. Подтверждение о
   * получении не проверяется.
   * @param message  сообщение, которое подлежит отправке
   * @param channelName канал, по которому необходимо отправить сообщение
   */
  virtual void sendAndForget(DialogMessage&& message,
                             std::string_view channelName) const = 0;
};
#endif  // ABSTRACTMESSAGEDESPATCHER_H
