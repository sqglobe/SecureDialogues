#ifndef GMAILCONNECTIONINFO_H
#define GMAILCONNECTIONINFO_H

#include <string>
#include "export/pluginconnectioninfo.h"

class GmailConnectionInfo : public PluginConnectionInfo {
 public:
  GmailConnectionInfo(const std::string& email, const std::string& token);

 public:
  virtual const char* getPluginId() const noexcept override;

 public:
  std::string email;
  std::string accessToken;
};

#endif  // GMAILCONNECTIONINFO_H
