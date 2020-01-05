#ifndef PLUGINCONNECTIONINFO_H
#define PLUGINCONNECTIONINFO_H

class PluginConnectionInfo {
 public:
  virtual ~PluginConnectionInfo() = default;

 public:
  virtual const char* getPluginId() const noexcept = 0;
};

#endif  // PLUGINCONNECTIONINFO_H
