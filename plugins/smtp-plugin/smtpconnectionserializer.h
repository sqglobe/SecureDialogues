#ifndef SMTPCONNECTIONSERIALIZER_H
#define SMTPCONNECTIONSERIALIZER_H

#include "export/pluginconnectionserializer.h"

class SmtpConnectionSerializer : public PluginConnectionSerializer {
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

#endif  // SMTPCONNECTIONSERIALIZER_H
