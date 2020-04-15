#include "dummyconnectioninfo.h"
#include "dummyplugindefs.h"
const char* DummyConnectionInfo::getPluginId() const noexcept {
  return ::getPluginId();
}

int64_t DummyConnectionInfo::getWaitPeriod() const noexcept {
  return 10;
}
