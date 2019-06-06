#ifndef VKOAUTH_H
#define VKOAUTH_H

#include "oauth-agents/interfaces/oauthagent.h"

class VkOauth : public OAuthAgent {
 public:
  VkOauth(std::string clientId, std::string redirectUri);
  VkOauth(std::string clientId,
          std::string redirectUri,
          std::string accessToken);

 public:
  std::string getUserUrl() const override;
  void loadAccessToken(const std::string& userCode) noexcept(false) override;
  void refreshAccessToken() noexcept(false) override;
  std::pair<std::string, std::string> getAuthParam() const override;
  bool isExpired() const override;
  std::string getRefreshToken() const override;

 private:
  std::string mClientId;
  std::string mRedirectUri;
  std::string mAccessToken;
};

#endif  // VKOAUTH_H
