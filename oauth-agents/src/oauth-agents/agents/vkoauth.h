#ifndef VKOAUTH_H
#define VKOAUTH_H

#include "oauth-agents/interfaces/oauthagent.h"

class VkOauth : public OAuthAgent {
 public:
  VkOauth(const std::string& clientId, const std::string& redirectUri);
  VkOauth(const std::string& clientId,
          const std::string& redirectUri,
          const std::string& accessToken);

 public:
  virtual std::string getUserUrl() const override;
  virtual void loadAccessToken(const std::string& userCode) noexcept(
      false) override;
  virtual void refreshAccessToken() noexcept(false) override;
  virtual std::pair<std::string, std::string> getAuthParam() const override;
  virtual bool isExpired() const override;
  virtual std::string getRefreshToken() const override;

 private:
  std::string mClientId;
  std::string mRedirectUri;
  std::string mAccessToken;
};

#endif  // VKOAUTH_H
