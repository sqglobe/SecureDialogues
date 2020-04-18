#ifndef VKCONNECTIONSERIALIZER_H
#define VKCONNECTIONSERIALIZER_H
#include "export/pluginconnectionserializer.h"

class VkConnectionSerializer final : public PluginConnectionSerializer {
 public:
  virtual uint32_t get_size(const PluginConnectionInfo* connInfo) const
      noexcept override;
  virtual bool serialize(void* buffer,
                         const PluginConnectionInfo* connInfo) const
      noexcept override;
  virtual bool deserialize(const void* buffer,
                           PluginConnectionInfo* connInfo) const
      noexcept override;
};

#endif  // VKCONNECTIONSERIALIZER_H
