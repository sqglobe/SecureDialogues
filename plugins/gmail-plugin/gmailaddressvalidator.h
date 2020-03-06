#ifndef GMAILADDRESSVALIDATOR_H
#define GMAILADDRESSVALIDATOR_H

#include "export/pluginaddressvalidator.h"

class GmailAddressValidator : public PluginAddressValidator {
 public:
  const char* isValid(const char* address) const noexcept override;
  const char* peelAddress(const char* address) const noexcept override;
  const char* getPlaceholder() const noexcept override;
  const char* getToolTip() const noexcept override;
};

#endif  // GMAILADDRESSVALIDATOR_H
