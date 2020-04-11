#include "vkconnectionserializer.h"
#include "serialization-helpers.h"
#include "vkconnectioninfo.h"
uint32_t VkConnectionSerializer::get_size(
    const PluginConnectionInfo* connInfo) const noexcept {
  if (const auto* conn = dynamic_cast<const VkConnectionInfo*>(connInfo);
      conn == nullptr) {
    return 0;
  } else {
    uint32_t size = 0;
    size += sizeof(std::string::size_type) + conn->vkId.length();
    size += sizeof(std::string::size_type) + conn->accessToken.length();
    return size;
  }
}

bool VkConnectionSerializer::serialize(
    void* buffer,
    const PluginConnectionInfo* connInfo) const noexcept {
  if (const auto* conn = dynamic_cast<const VkConnectionInfo*>(connInfo);
      conn == nullptr) {
    return false;
  } else {
    buffer = serialization_helpers::save_str(conn->vkId, buffer);
    buffer = serialization_helpers::save_str(conn->accessToken, buffer);
    return true;
  }
}

bool VkConnectionSerializer::deserialize(const void* buffer,
                                         PluginConnectionInfo* connInfo) const
    noexcept {
  if (auto* conn = dynamic_cast<VkConnectionInfo*>(connInfo); conn == nullptr) {
    return false;
  } else {
    buffer = serialization_helpers::restore_str(conn->vkId, buffer);
    buffer = serialization_helpers::restore_str(conn->accessToken, buffer);
    return true;
  }
}
