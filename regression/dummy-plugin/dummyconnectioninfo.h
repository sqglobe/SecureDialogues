#ifndef DUMMYCONNECTIONINFO_H
#define DUMMYCONNECTIONINFO_H

#include "export/pluginconnectioninfo.h"

class DummyConnectionInfo final : public PluginConnectionInfo {
 public:
  const char* getPluginId() const noexcept override;
  int64_t getWaitPeriod() const noexcept override;
};

#endif  // DUMMYCONNECTIONINFO_H
