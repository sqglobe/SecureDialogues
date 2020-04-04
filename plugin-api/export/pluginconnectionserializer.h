#ifndef PLUGINCONNECTIONSERIALIZER_H
#define PLUGINCONNECTIONSERIALIZER_H

#include <cstdint>

class PluginConnectionInfo;

class PluginConnectionSerializer {
 public:
  virtual ~PluginConnectionSerializer() = default;

 public:
  [[nodiscard]] virtual uint32_t get_size(
      const PluginConnectionInfo* connInfo) const noexcept = 0;
  virtual bool serialize(void* buffer,
                         const PluginConnectionInfo* connInfo) const
      noexcept = 0;
  virtual bool deserialize(const void* buffer,
                           PluginConnectionInfo* connInfo) const noexcept = 0;
};

#endif  // PLUGINCONNECTIONSERIALIZER_H
