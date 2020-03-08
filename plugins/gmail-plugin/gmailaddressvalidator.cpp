#include "gmailaddressvalidator.h"

#include <libintl.h>
#include <regex>

const char* GmailAddressValidator::isValid(const char* address) const noexcept {
  std::regex regex("(\\S+)@([a-z0-9-]+)(\\.)([a-z]{2,4})(\\.?)([a-z]{0,4})+");
  if (!std::regex_match(address, regex)) {
    return dgettext("gmail-plugin", "Please, fill 'Address' with e-mail");
  }

  return nullptr;
}

const char* GmailAddressValidator::peelAddress(const char* address) const
    noexcept {
  return address;
}

const char* GmailAddressValidator::getPlaceholder() const noexcept {
  return dgettext("gmail-plugin", "E-mail address Gmail");
}

const char* GmailAddressValidator::getToolTip() const noexcept {
  return dgettext("gmail-plugin",
                  "Please, specify valid e-mail address Gmail service");
}
