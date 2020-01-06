#include "gmailconnectioninfo.h"
#include "gmailplugindefs.h"

GmailConnectionInfo::GmailConnectionInfo(const std::string& email,
                                         const std::string& token) :
    email(email),
    accessToken(token) {}

const char* GmailConnectionInfo::getPluginId() const noexcept {
  return ::getPluginId();
}
