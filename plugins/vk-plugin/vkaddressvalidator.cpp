#include "vkaddressvalidator.h"
#include <libintl.h>
#include <regex>

const char* VkAddressValidator::isValid(const char* address) const noexcept {
  static const std::regex regex("^(https://vk.com/id|id)?\\d+$");
  if (!std::regex_match(address, regex)) {
    return dgettext("vk-plugin",
                    "Address has to contains vk id (for example 123451) or "
                    "reference"
                    " to your contact page");
  }
  return nullptr;
}

const char* VkAddressValidator::peelAddress(const char* address) const
    noexcept {
  static const std::regex regex("^(?:https://vk.com/id|id)?(\\d+)$");
  static std::string returnRes;
  std::match_results<const char*> res;
  if (std::regex_match(address, res, regex)) {
    returnRes = res.str(1);
    return returnRes.c_str();
  }
  return address;
}

const char* VkAddressValidator::getPlaceholder() const noexcept {
  return dgettext("vk-plugin", "Set vk id or link to vk page");
}

const char* VkAddressValidator::getToolTip() const noexcept {
  return dgettext("vk-plugin",
                  "Your contact's address of vk page or vk id  (for example "
                  "123451 or id123451) ");
}
