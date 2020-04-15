#include "dummyaddressvalidator.h"

const char* DummyAddressValidator::isValid(const char*) const noexcept {
  return nullptr;
}

const char* DummyAddressValidator::peelAddress(const char* address) const
    noexcept {
  return address;
}

const char* DummyAddressValidator::getPlaceholder() const noexcept {
  return "Test dummy plugin";
}

const char* DummyAddressValidator::getToolTip() const noexcept {
  return "Test dummy plugin";
}
