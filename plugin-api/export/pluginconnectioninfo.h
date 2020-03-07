#ifndef PLUGINCONNECTIONINFO_H
#define PLUGINCONNECTIONINFO_H

#include <stdint.h>

class PluginConnectionInfo {
 public:
  virtual ~PluginConnectionInfo() = default;

 public:
  virtual const char* getPluginId() const noexcept = 0;
  virtual int64_t getWaitPeriod() const noexcept = 0;
};

#endif  // PLUGINCONNECTIONINFO_H
