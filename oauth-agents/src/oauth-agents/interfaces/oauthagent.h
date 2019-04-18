#ifndef OAUTHAGENT_H
#define OAUTHAGENT_H

#include <string>

class OAuthAgent {
 public:
  virtual ~OAuthAgent() = default;

 public:
  virtual std::string getUserUrl() const = 0;
  virtual void loadAccessToken(const std::string& userCode) noexcept(false) = 0;
  virtual void refreshAccessToken() noexcept(false) = 0;
  virtual std::pair<std::string, std::string> getAuthParam() const = 0;
  virtual bool isExpired() const = 0;
  virtual std::string getRefreshToken() const = 0;
};

#endif  // OAUTHAGENT_H
