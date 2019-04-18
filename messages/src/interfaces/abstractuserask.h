#ifndef ABSTRACTUSERASK_H
#define ABSTRACTUSERASK_H
#include <string>

/**
 * @brief Интерфейс для класса, который запрашивает у пользователя подтверждение
 */
class AbstractUserAsk {
 public:
  virtual ~AbstractUserAsk() = default;
  /**
   * @brief Вызывается для того, чтобы запросить у пользователя выполнение той
   * или иной операции
   * @param question текст, который будет отображен в запросе
   * @return true - пользователь подтвердил выполнение операции, false -
   * отклонил
   */
  virtual bool ask(const std::string& question) const = 0;
};
#endif  // ABSTRACTUSERASK_H
