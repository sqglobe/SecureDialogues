#include "widgetsutils.h"
#include <QWidget>
#include <sstream>
#include "correctnessinputerror.h"

void WigetUtils::test(
    const std::vector<std::pair<std::string, std::function<bool()> > >&
        checks) noexcept(false) {
  std::stringstream error;
  auto failedCnt = std::count_if(
      checks.cbegin(), checks.cend(),
      [&error](
          const std::pair<std::string, std::function<bool(void)> >& funcData)
          -> bool {
        if (!funcData.second()) {
          error << funcData.first << std::endl;
          return true;
        }
        return false;
      });
  if (failedCnt > 0) {
    throw CorrectnessInputError(error.str());
  }
}
