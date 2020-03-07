#ifndef PLUGINADDRESSVALIDATOR_H
#define PLUGINADDRESSVALIDATOR_H

class PluginAddressValidator {
 public:
  virtual ~PluginAddressValidator() = default;

 public:
  virtual const char* isValid(const char* address) const noexcept = 0;
  virtual const char* peelAddress(const char* address) const noexcept = 0;
  virtual const char* getPlaceholder() const noexcept = 0;
  virtual const char* getToolTip() const noexcept = 0;
};

#endif  // PLUGINADDRESSVALIDATOR_H
