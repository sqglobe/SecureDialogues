#ifndef FACTORIES_H
#define FACTORIES_H

#include <memory>
#include <string>

enum class AgentType { GMAIL, VK };

class ApiAgent;
class OAuthAgent;

class OauthFactory {
 public:
  virtual ~OauthFactory() = default;

 public:
  virtual std::unique_ptr<ApiAgent> makeApiAgent(
      const std::string& clientId) const = 0;
  virtual std::unique_ptr<OAuthAgent> makeOauthAgent() const = 0;
  virtual std::unique_ptr<OAuthAgent> makeOauthAgent(
      const std::string& refreshToken) const = 0;
};

std::unique_ptr<OauthFactory> makeFactory(AgentType type);

#endif  // FACTORIES_H
