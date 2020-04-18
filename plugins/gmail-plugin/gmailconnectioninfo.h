#ifndef GMAILCONNECTIONINFO_H
#define GMAILCONNECTIONINFO_H

#include <string>
#include "export/pluginconnectioninfo.h"

class GmailConnectionInfo final : public PluginConnectionInfo {
 public:
  GmailConnectionInfo() = default;
  GmailConnectionInfo(const std::string& email, const std::string& token);

 public:
  virtual const char* getPluginId() const noexcept override;
  int64_t getWaitPeriod() const noexcept override;

 public:
  std::string email;
  std::string accessToken;
};

#endif  // GMAILCONNECTIONINFO_H
