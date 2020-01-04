#ifndef ABSTRACTUSERNOTIFIER_H
#define ABSTRACTUSERNOTIFIER_H

#include <string>

/**
 * @brief Интерфейс, который используется для уведомления пользователя
 */
class AbstractUserNotifier {
 public:
  virtual ~AbstractUserNotifier() = default;
  /**
   * @brief Тип уведомления
   * ERROR - произошла ошибка
   * INFO - уведомление носит информационный характер
   * WARNING - сообщение о не критической ошибке
   * SUCCESS - уведомление об успехе, например об успешной операции
   */
  enum class Severity { Error, Info, Warning, Success };

  /**
   * @brief Выполняет уведомление пользователя о событии
   * @param sev тип события
   * @param message тест, который будет показан пользователю
   */
  virtual void notify(Severity sev, const std::string& message) = 0;
};
#endif  // ABSTRACTUSERNOTIFIER_H
