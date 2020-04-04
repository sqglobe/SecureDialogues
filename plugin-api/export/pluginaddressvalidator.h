#ifndef PLUGINADDRESSVALIDATOR_H
#define PLUGINADDRESSVALIDATOR_H

class PluginAddressValidator {
 public:
  virtual ~PluginAddressValidator() = default;

 public:
  [[nodiscard]] virtual const char* isValid(const char* address) const
      noexcept = 0;
  [[nodiscard]] virtual const char* peelAddress(const char* address) const
      noexcept = 0;  // function has to be used in a single thread
  [[nodiscard]] virtual const char* getPlaceholder() const noexcept = 0;
  [[nodiscard]] virtual const char* getToolTip() const noexcept = 0;
};

#endif  // PLUGINADDRESSVALIDATOR_H
