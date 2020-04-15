#include "dummyconnectionserializer.h"

uint32_t DummyConnectionSerializer::get_size(const PluginConnectionInfo*) const
    noexcept {
  return 0;
}

bool DummyConnectionSerializer::serialize(void*,
                                          const PluginConnectionInfo*) const
    noexcept {
  return true;
}

bool DummyConnectionSerializer::deserialize(const void*,
                                            PluginConnectionInfo*) const
    noexcept {
  return true;
}
