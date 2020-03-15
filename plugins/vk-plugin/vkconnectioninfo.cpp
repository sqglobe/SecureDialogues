#include "vkconnectioninfo.h"
#include "vkplugindefs.h"

VkConnectionInfo::VkConnectionInfo(const std::string& id,
                                   const std::string& token) :
    vkId(id),
    accessToken(token) {}

const char* VkConnectionInfo::getPluginId() const noexcept {
  return ::getPluginId();
}

int64_t VkConnectionInfo::getWaitPeriod() const noexcept {
  return 30;
}
