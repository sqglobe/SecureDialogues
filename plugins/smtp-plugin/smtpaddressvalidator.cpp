#include "smtpaddressvalidator.h"
#include <libintl.h>
#include <regex>

const char* SmtpAddressValidator::isValid(const char* address) const noexcept {
  static const std::regex regex(
      "(\\S+)@([a-z0-9-]+)(\\.)([a-z]{2,4})(\\.?)([a-z]{0,4})+");
  if (!std::regex_match(address, regex)) {
    return dgettext(
        "smtp-plugin",
        "Address has to be contained valid email like test@email.com");
  }
  return nullptr;
}

const char* SmtpAddressValidator::peelAddress(const char* address) const
    noexcept {
  return address;
}

const char* SmtpAddressValidator::getPlaceholder() const noexcept {
  return dgettext("smtp-plugin", "Set valid email address");
}

const char* SmtpAddressValidator::getToolTip() const noexcept {
  return dgettext("smtp-plugin", "Your contact's email address");
}
