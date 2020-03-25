#include "smtpconnectioninfo.h"

#include "smtpplugindefs.h"
SmtpConnectionInfo::SmtpConnectionInfo(std::string userName,
                                       std::string password,
                                       bool tlsUsed,
                                       std::string from,
                                       std::string smtpAddr,
                                       int smtpPort,
                                       std::string imapAddr,
                                       int imapPort) :
    userName(std::move(userName)),
    password(std::move(password)), tlsUsed(tlsUsed), from(std::move(from)),
    smtpAddr(std::move(smtpAddr)), smtpPort(smtpPort),
    imapAddr(std::move(imapAddr)), imapPort(imapPort) {}

const char* SmtpConnectionInfo::getPluginId() const noexcept {
  return ::getPluginId();
}

int64_t SmtpConnectionInfo::getWaitPeriod() const noexcept {
  return 360;
}
