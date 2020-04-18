#ifndef SMTPADDRESSVALIDATOR_H
#define SMTPADDRESSVALIDATOR_H

#include "export/pluginaddressvalidator.h"
class SmtpAddressValidator final : public PluginAddressValidator {
 public:
  const char* isValid(const char* address) const noexcept override;
  const char* peelAddress(const char* address) const noexcept override;
  const char* getPlaceholder() const noexcept override;
  const char* getToolTip() const noexcept override;
};

#endif  // SMTPADDRESSVALIDATOR_H
