#ifndef VKADDRESSVALIDATOR_H
#define VKADDRESSVALIDATOR_H

#include "export/pluginaddressvalidator.h"

class VkAddressValidator : public PluginAddressValidator {
 public:
  const char* isValid(const char* address) const noexcept override;
  const char* peelAddress(const char* address) const noexcept override;
  const char* getPlaceholder() const noexcept override;
  const char* getToolTip() const noexcept override;
};

#endif  // VKADDRESSVALIDATOR_H
