#ifndef GMAILOAUTH_H
#define GMAILOAUTH_H

#include <chrono>
#include <mutex>
#include "httprequest.h"

const std::string REDIRECT_URL = "urn:ietf:wg:oauth:2.0:oob";

class GmailOauth {
 public:
  GmailOauth(std::string clientId,
             std::string clientSecret,
             std::string redirectUri);

  GmailOauth(std::string clientId,
             std::string clientSecret,
             std::string redirectUri,
             std::string refreshToken);

 public:
  std::string getUserUrl() const;
  void loadAccessToken(const std::string& userCode) noexcept(false);
  void refreshAccessToken() noexcept(false);
  std::pair<std::string, std::string> getAuthParam() const;
  bool isExpired() const;
  std::string getRefreshToken() const;

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
