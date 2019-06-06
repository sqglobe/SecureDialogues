#ifndef GMAILOAUTH_H
#define GMAILOAUTH_H

#include "oauth-agents/interfaces/oauthagent.h"

#include <chrono>
#include <mutex>
#include "httprequest.h"

class GmailOauth : public OAuthAgent {
 public:
  GmailOauth(const std::string& clientId,
             const std::string& clientSecret,
             const std::string& redirectUri);

  GmailOauth(const std::string& clientId,
             const std::string& clientSecret,
             const std::string& redirectUri,
             const std::string& refreshToken);

 public:
  std::string getUserUrl() const override;
  void loadAccessToken(const std::string& userCode) noexcept(false) override;
  void refreshAccessToken() noexcept(false) override;
  std::pair<std::string, std::string> getAuthParam() const override;
  bool isExpired() const override;
  std::string getRefreshToken() const override;

 private:
  std::string mClientId;
  std::string mClientSecret;
  std::string mRedirectUri;
  std::string mGrantType;

  std::string mAccessToken;
  std::string mRefreshToken;
  std::string mTokenType;

  std::chrono::system_clock::time_point mExpiresAt;
  HttpRequest mRequest;
  mutable std::mutex mMutex;
};

#endif  // GMAILOAUTH_H
