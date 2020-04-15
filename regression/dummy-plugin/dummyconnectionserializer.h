#ifndef DUMMYCONNECTIONSERIALIZER_H
#define DUMMYCONNECTIONSERIALIZER_H

#include "export/pluginconnectionserializer.h"

class DummyConnectionSerializer final : public PluginConnectionSerializer {
 public:
  uint32_t get_size(const PluginConnectionInfo* connInfo) const
      noexcept override;
  bool serialize(void* buffer, const PluginConnectionInfo* connInfo) const
      noexcept override;
  bool deserialize(const void* buffer, PluginConnectionInfo* connInfo) const
      noexcept override;
};

#endif  // DUMMYCONNECTIONSERIALIZER_H
