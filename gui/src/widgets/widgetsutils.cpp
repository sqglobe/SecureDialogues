#include "widgetsutils.h"
#include <QWidget>
#include <sstream>
#include "correctnessinputerror.h"

void WigetUtils::test(
    const std::vector<std::pair<QString, std::function<bool()> > >&
        checks) noexcept(false) {
  std::stringstream error;
  auto failedCnt = std::count_if(
      checks.cbegin(), checks.cend(),
      [&error](const std::pair<QString, std::function<bool(void)> >& funcData)
          -> bool {
        if (!funcData.second()) {
          error << funcData.first.toStdString() << std::endl;
          return true;
        }
        return false;
      });
  if (failedCnt > 0) {
    throw CorrectnessInputError(error.str());
  }
}
