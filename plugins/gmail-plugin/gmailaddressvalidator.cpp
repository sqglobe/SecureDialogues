#include "gmailaddressvalidator.h"

#include <regex>

const char* GmailAddressValidator::isValid(const char* address) const noexcept {
  std::regex regex("(\\S+)@([a-z0-9-]+)(\\.)([a-z]{2,4})(\\.?)([a-z]{0,4})+");
  if (!std::regex_match(address, regex)) {
    return "Please, fill 'Address' with e-mail";
  }

  return nullptr;
}

const char* GmailAddressValidator::peelAddress(const char* address) const
    noexcept {
  return address;
}

const char* GmailAddressValidator::getPlaceholder() const noexcept {
  return "E-mail address Gmail";
}

const char* GmailAddressValidator::getToolTip() const noexcept {
  return "Please, specify valid e-mail address Gmail service";
}
