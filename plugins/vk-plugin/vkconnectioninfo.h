#ifndef VKCONNECTIONINFO_H
#define VKCONNECTIONINFO_H

#include <string>
#include "export/pluginconnectioninfo.h"

class VkConnectionInfo final : public PluginConnectionInfo {
 public:
  VkConnectionInfo() = default;
  VkConnectionInfo(const std::string& id, const std::string& token);

 public:
  const char* getPluginId() const noexcept override;
  int64_t getWaitPeriod() const noexcept override;

 public:
  std::string vkId;
  std::string accessToken;
};

#endif  // VKCONNECTIONINFO_H
