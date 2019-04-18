#ifndef DELIVERYHANDLER_H
#define DELIVERYHANDLER_H

#include <memory>

/**
 * @brief Абстрактный класс для использования в TimeoutedRrepository
 */
class DeliveryHandler {
 public:
  virtual ~DeliveryHandler() = default;

 public:
  /**
   * @brief Вызывается, когда было получено подтверждение о получении сообщения
   */
  virtual void removed() = 0;

  /**
   * @brief Вызывается, когда сообщение было удалено по таймауту
   */
  virtual void timeouted() = 0;
};
void removed(const std::shared_ptr<DeliveryHandler>& obj);
void timeouted(const std::shared_ptr<DeliveryHandler>& obj);

#endif  // DELIVERYHANDLER_H
