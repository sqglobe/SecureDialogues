#ifndef SMTPCONNECTIONINFO_H
#define SMTPCONNECTIONINFO_H
#include <string>
#include "export/pluginconnectioninfo.h"

class SmtpConnectionInfo final : public PluginConnectionInfo {
 public:
  SmtpConnectionInfo() = default;
  SmtpConnectionInfo(std::string userName,
                     std::string password,
                     bool tlsUsed,
                     std::string from,
                     std::string smtpAddr,
                     int smtpPort,
                     std::string imapAddr,
                     int imapPort);

 public:
  const char* getPluginId() const noexcept override;
  int64_t getWaitPeriod() const noexcept override;

 public:
  std::string userName;
  std::string password;
  bool tlsUsed;
  std::string from;

  std::string smtpAddr;
  int smtpPort;

  std::string imapAddr;
  int imapPort;
};

#endif  // SMTPCONNECTIONINFO_H
