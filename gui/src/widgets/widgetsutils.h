#ifndef WIDGETSUTILS_H
#define WIDGETSUTILS_H

#include <functional>
#include <utility>
#include <vector>

class QWidget;

namespace WigetUtils {
/**
 * @brief Выполняет проверку предикатов.
 * Если проверка не прошла хотя бы для одного из них, то выбрасывается
 * исключение с текстом ошибки.
 * @param checks список предикатов вместе с описанием ошибки
 */
void test(const std::vector<std::pair<std::string, std::function<bool(void)>>>&
              checks) noexcept(false);
}  // namespace WigetUtils

#endif  // WIDGETSUTILS_H
