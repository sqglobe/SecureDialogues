#ifndef DUMMYADDRESSVALIDATOR_H
#define DUMMYADDRESSVALIDATOR_H

#include "export/pluginaddressvalidator.h"
class DummyAddressValidator final : public PluginAddressValidator {
 public:
  const char* isValid(const char* address) const noexcept override;
  const char* peelAddress(const char* address) const noexcept override;
  const char* getPlaceholder() const noexcept override;
  const char* getToolTip() const noexcept override;
};

#endif  // DUMMYADDRESSVALIDATOR_H
