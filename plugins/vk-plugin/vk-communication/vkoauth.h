#ifndef VKOAUTH_H
#define VKOAUTH_H

#include <string>

class VkOauth {
 public:
  VkOauth(std::string clientId, std::string redirectUri);
  VkOauth(std::string clientId,
          std::string redirectUri,
          std::string accessToken);

 public:
  std::string getUserUrl() const;
  void loadAccessToken(const std::string& userCode) noexcept(false);
  void refreshAccessToken() noexcept(false);
  std::pair<std::string, std::string> getAuthParam() const;
  bool isExpired() const;
  std::string getRefreshToken() const;

 private:
  std::string mClientId;
  std::string mRedirectUri;
  std::string mAccessToken;
};

#endif  // VKOAUTH_H
